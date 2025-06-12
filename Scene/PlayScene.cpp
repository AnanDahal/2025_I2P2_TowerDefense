#include <algorithm>
#include <allegro5/allegro.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include <random>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>

#include "Enemy/Enemy.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/ArmyEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Enemy/CarrierEnemy.h"
#include "Enemy/BiggerCarrierEnemy.h"
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/HealingTurret.hpp"
#include "Turret/SniperTurret.h"
#include "Turret/BuffTurret.h"
#include "Turret/SlowTurret.h"
#include "Turret/TurretButton.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "UI/Animation/Plane.hpp"
#include "UI/Component/Label.hpp"
#include "Turret/Turret.hpp"            // for Turret::GetPrice(), Position, CollisionRadius

// TODO HACKATHON-4 (1/3): Trace how the game handles keyboard input.
// TODO HACKATHON-4 (2/3): Find the cheat code sequence in this file.
// TODO HACKATHON-4 (3/3): When the cheat code is entered, a plane should be spawned and added to the scene.
// TODO HACKATHON-5 (1/4): There's a bug in this file, which crashes the game when you win. Try to find it.
// TODO HACKATHON-5 (2/4): The "LIFE" label are not updated when you lose a life. Try to fix it.

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = {
    ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN   
    // ,
    // ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT,
    // ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER
};
Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
    mapState.clear();
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 10;
    money = 150;
    Score = 0;
    SpeedMult = 1;

    // Add groups from bottom to top.
    AddNewObject(TileMapGroup        = new Group());
    AddNewObject(GroundEffectGroup   = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup          = new Group());
    AddNewObject(EnemyGroup          = new Group());
    AddNewObject(BulletGroup         = new Group());
    AddNewObject(EffectGroup         = new Group());
    AddNewControlObject(UIGroup      = new Group());

    // TrueType font add-ons
    al_init_ttf_addon();

    // Endless vs. static map
    endlessMode = (MapId == 2);
    endlessRound = 0;
    if (endlessMode) {
        // Endless: random start/end each round
        endlessRound = 1;
        GenerateRandomMap(endlessRound);
        GenerateEnemyWave(endlessRound);

    } else {
        // Static: load from file, but override spawn/exit for pathfinding
        ReadMap();

        // ─── For static maps, spawn at (0,0) and exit at (W-1,H-1) ───
        entryPoints.clear();
        entryPoints.emplace_back(0, 0);
        endPoint = Engine::Point(MapWidth - 1, MapHeight - 1);
        PlayScene::EndGridPoint = endPoint;

        ReadEnemyWave();
    }

    // Compute BFS distances (uses EndGridPoint)
    mapDistance = CalculateBFSDistance();

    // Build UI
    ConstructUI();

    // Chatbox only on MapId 1
    if (MapId == 1) {
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
        chatBox = std::make_shared<Engine::ChatBox>(
            w, h,
            "THIS IS HOW LONG THE TEXT CAN BE, IF YOU WANT TO TYPE A LONG MESSAGE, YOU CAN TYPE IT HERE. "
        );
        // AddNewControlObject(chatBox.get());
    }

    // Target preview for turret placement
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);

    // Preload lose scene assets
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");

    // Start background music
    bgmId = AudioHelper::PlayBGM("play.ogg");
}


void PlayScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

    // Properly remove and destroy chatbox
    if (chatBox) {
        // Defensive: Remove from UIGroup if present
        if (UIGroup) {
            UIGroup->RemoveObject(chatBox->GetObjectIterator());
        }
        chatBox->Terminate();
        chatBox.reset();
    }
    IScene::Terminate();
}

void PlayScene::Update(float deltaTime) {
    // 1) Move all existing objects (enemies, bullets, effects, etc.)
    IScene::Update(deltaTime * SpeedMult);

    // --- Round transition effect ---
    if (roundTransitionState != NONE) {
        // Only update non-enemy groups while the round label is showing
        EffectGroup->Update(deltaTime);
        BulletGroup->Update(deltaTime);
        GroundEffectGroup->Update(deltaTime);

        roundTransitionTimer -= deltaTime;
        if (roundTransitionTimer <= 0) {
            if (roundTransitionState == WAIT_BEFORE_ROUND_LABEL) {
                roundTransitionState = SHOW_ROUND_LABEL;
                roundTransitionTimer = 1.0f;
            }
            else if (roundTransitionState == SHOW_ROUND_LABEL) {
                roundTransitionState = WAIT_AFTER_ROUND_LABEL;
                roundTransitionTimer = 2.0f;
            }
            else if (roundTransitionState == WAIT_AFTER_ROUND_LABEL) {
                // Start next round
                if (endlessMode) endlessRound = nextRoundNumber;
                roundTransitionState = NONE;
                RemoveAllTurrets();
                EnemyGroup->Clear();
                GenerateRandomMap(nextRoundNumber);
                PlayScene::EndGridPoint = endPoint;
                mapDistance = CalculateBFSDistance();
                GenerateEnemyWave(nextRoundNumber);
                EarnMoney(100 * nextRoundNumber / 2);
                ticks = 0;  // reset spawn-timer
                if (roundLabel) roundLabel->Text = "Round: " + std::to_string(nextRoundNumber);
            }
        }
        return;  // skip normal spawning while transitioning
    }

    // --- Chat box update ---
    if (chatBox) {
        chatBox->Update(deltaTime);
        if (chatBox->finished) {
            if (UIGroup) UIGroup->RemoveObject(chatBox->GetObjectIterator());
            chatBox->Terminate();
            chatBox.reset();
        }
    }

    // --- Speed multiplier / death-countdown guard ---
    // if (SpeedMult == 0)
    //     deathCountDown = -1;
    // else if (deathCountDown != -1)
    //     SpeedMult = 1;

    // --- Danger indicator calculation ---
    std::vector<float> reachEndTimes;
    for (auto &obj : EnemyGroup->GetObjects())
        reachEndTimes.push_back(dynamic_cast<Enemy *>(obj)->reachEndTime);
    std::sort(reachEndTimes.begin(), reachEndTimes.end());

    float newDeathCountDown = -1;
    int danger = lives;
    for (float t : reachEndTimes) {
        if (t <= DangerTime) {
            danger--;
            if (danger <= 0) {
                float pos = DangerTime - t;
                if (t > deathCountDown) {
                    AudioHelper::StopSample(deathBGMInstance);
                    if (SpeedMult != 0)
                        deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
                }
                float alpha = pos / DangerTime;
                alpha = std::clamp(alpha * alpha * 255.0f, 0.0f, 255.0f);
                dangerIndicator->Tint = al_map_rgba(255, 255, 255, static_cast<int>(alpha));
                newDeathCountDown = t;
                break;
            }
        }
    }
    deathCountDown = newDeathCountDown;
    if (SpeedMult == 0 || (deathCountDown == -1 && lives > 0)) {
        AudioHelper::StopSample(deathBGMInstance);
        dangerIndicator->Tint.a = 0;
    }

    // --- TIMED Enemy spawn logic (uses each wave’s `wait`) ---
    ticks += deltaTime * SpeedMult;  // accumulate time (speeded up by SpeedMult)
    while (!enemyWaveData.empty() && ticks >= enemyWaveData.front().second) {
        auto wave = enemyWaveData.front();
        enemyWaveData.pop_front();
        ticks -= wave.second;  // consume the wait interval

        // 1) Pick a random entry grid point
        Engine::Point entry = entryPoints[rand() % entryPoints.size()];

        // 2) Compute off-screen spawn center
        float spawnX, spawnY;
        if (entry.x == 0) {                 // left edge → move right
            spawnX = -BlockSize/2.0f;
            spawnY = entry.y * BlockSize + BlockSize/2.0f;
        }
        else if (entry.x == MapWidth - 1) { // right edge → move left
            spawnX = MapWidth * BlockSize + BlockSize/2.0f;
            spawnY = entry.y * BlockSize + BlockSize/2.0f;
        }
        else if (entry.y == 0) {            // top edge → move down
            spawnX = entry.x * BlockSize + BlockSize/2.0f;
            spawnY = -BlockSize/2.0f;
        }
        else {                              // bottom edge → move up
            spawnX = entry.x * BlockSize + BlockSize/2.0f;
            spawnY = MapHeight * BlockSize + BlockSize/2.0f;
        }

        // 3) Instantiate the correct Enemy subclass
        Enemy* e = nullptr;
        switch (wave.first) {
            case 1: EnemyGroup->AddNewObject(e = new SoldierEnemy (spawnX, spawnY)); break;
            case 2: EnemyGroup->AddNewObject(e = new ArmyEnemy    (spawnX, spawnY)); break;
            case 3: EnemyGroup->AddNewObject(e = new TankEnemy    (spawnX, spawnY)); break;
            case 4: EnemyGroup->AddNewObject(e = new CarrierEnemy (spawnX, spawnY)); break;
            case 5: EnemyGroup->AddNewObject(e = new BiggerCarrierEnemy(spawnX, spawnY)); break;
            default:
                printf("[ERROR] Unknown enemy type %d\n", wave.first);
                continue;
        }

        if (e) {
            // Position and path
            e->Position.x = spawnX;
            e->Position.y = spawnY;
            printf("Spawning enemy at (%.1f, %.1f)\n", spawnX, spawnY);
            e->UpdatePath(mapDistance);
        }
    }

    // --- Turret-preview and shovel-preview updates ---
    if (preview) {
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        preview->Update(deltaTime);
    }
    if (Shoveling && shovelPreview) {
        auto mpos = Engine::GameEngine::GetInstance().GetMousePosition();
        shovelPreview->Position = mpos;
        shovelPreview->Update(deltaTime);
    }
        // ─── Endless mode: if queue empty & no live enemies, launch next‐round transition ───
    if (endlessMode
        && enemyWaveData.empty()
        && EnemyGroup->GetObjects().empty()
        && roundTransitionState == NONE)
    {
        nextRoundNumber = endlessRound + 1;
        roundTransitionState = WAIT_BEFORE_ROUND_LABEL;
        roundTransitionTimer = 1.0f;
    }

}



void PlayScene::Draw() const {
    IScene::Draw();
    if (DebugMode) {
        // Draw reverse BFS distance on all reachable blocks.
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                if (mapDistance[i][j] != -1) {
                    // Not elegant nor efficient, but it's quite enough for debugging.
                    Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 32, (j + 0.5) * BlockSize, (i + 0.5) * BlockSize);
                    label.Anchor = Engine::Point(0.5, 0.5);
                    label.Draw();
                }
            }
        }
    }

    // --- Round transition overlay ---
    if (roundTransitionState == SHOW_ROUND_LABEL) {
        int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
        int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
        al_draw_filled_rectangle(0, 0, w, h, al_map_rgb(0, 0, 0));
        std::string text = "ROUND " + std::to_string(nextRoundNumber);
        ALLEGRO_FONT* font = al_load_ttf_font("Resource/fonts/pirulen.ttf", 72, 0);
        if (font) {
            al_draw_text(font, al_map_rgb(255, 255, 255), w / 2, h / 2 - 36, ALLEGRO_ALIGN_CENTRE, text.c_str());
            al_destroy_font(font);
        }
    }

}
void PlayScene::OnMouseDown(int button, int mx, int my) {
    // if ((button & 1) && !imgTarget->Visible && preview) {
    //     // Cancel turret construct.
    //     UIGroup->RemoveObject(preview->GetObjectIterator());
    //     preview = nullptr;
    // }
    IScene::OnMouseDown(button, mx, my);
}

void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}


void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);

    // ── 1) Shovel-mode removal (unchanged) ──
    if (Shoveling && (button & 1)) {
        int cellX = mx / BlockSize;
        int cellY = my / BlockSize;
        for (auto obj : TowerGroup->GetObjects()) {
            auto t = dynamic_cast<Turret*>(obj);
            if (!t) continue;
            int tx = int(t->Position.x / BlockSize);
            int ty = int(t->Position.y / BlockSize);
            if (tx == cellX && ty == cellY) {
                EarnMoney(t->GetPrice());
                TowerGroup->RemoveObject(t->GetObjectIterator());
                mapState[ty][tx] = TILE_FLOOR;
                break;
            }
        }
        if (shovelPreview) {
            UIGroup->RemoveObject(shovelPreview->GetObjectIterator());
            shovelPreview = nullptr;
        }
        Shoveling = false;
        return;
    }

    // ── 2) Only proceed on left-click when we actually have a turret preview and target is shown ──
    if (!(button & 1) || !preview || !imgTarget->Visible)
        return;

    int cellX = int(imgTarget->Position.x) / BlockSize;
    int cellY = int(imgTarget->Position.y) / BlockSize;

    // ── 3) Must be pure floor ──
    if (mapState[cellY][cellX] != TILE_FLOOR) {
        auto fx = new DirtyEffect(
            "play/target-invalid.png", 1,
            cellX * BlockSize + BlockSize/2,
            cellY * BlockSize + BlockSize/2
        );
        GroundEffectGroup->AddNewObject(fx);
        fx->Rotation = 0;
        return;
    }

    // ── 4) Must not block all enemy paths ──
    if (!CheckSpaceValid(cellX, cellY)) {
        auto fx = new DirtyEffect(
            "play/target-invalid.png", 1,
            cellX * BlockSize + BlockSize/2,
            cellY * BlockSize + BlockSize/2
        );
        GroundEffectGroup->AddNewObject(fx);
        fx->Rotation = 0;
        return;
    }

    // ── 5) Valid! Charge, finalize preview → real turret ──
    EarnMoney(-preview->GetPrice());

    // Detach and re-position
    preview->GetObjectIterator()->first = false;
    UIGroup->RemoveObject(preview->GetObjectIterator());
    preview->Position.x = cellX * BlockSize + BlockSize/2;
    preview->Position.y = cellY * BlockSize + BlockSize/2;
    preview->Enabled = true;
    preview->Preview = false;
    preview->Tint    = al_map_rgba(255,255,255,255);

    // Commit and update
    TowerGroup->AddNewObject(preview);
    preview->Update(0);
    mapState[cellY][cellX] = TILE_OCCUPIED;

    // Clear our preview pointer and refresh the cursor effect
    preview = nullptr;
    OnMouseMove(mx, my);
}



void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);

    if (chatBox) {
        chatBox->OnKeyDown(keyCode);
    }


    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    } else {
        keyStrokes.push_back(keyCode);
        if (keyStrokes.size() > code.size())
            keyStrokes.pop_front();
        if (keyStrokes.size() == code.size()) {
            auto it1 = keyStrokes.begin();
            auto it2  = code.begin();

            while (it1 != keyStrokes.end()) {
                if (*it1 == *it2) {
                    ++it1;
                    ++it2;
                }
                else {
                    break;
                }
                if (it2 == code.end()) {
                    UIGroup->AddNewObject(new Plane);
                    this->money += 10000;
                    UIMoney->Text = std::string("$") + std::to_string(this->money);
                }
            }
        }
    }
    if (keyCode == ALLEGRO_KEY_Q && money >= MachineGunTurret::Price) {
        // Hotkey for MachineGunTurret.
        UIBtnClicked(0);
    } else if (keyCode == ALLEGRO_KEY_W && money >= LaserTurret::Price) {
        // Hotkey for LaserTurret.
        UIBtnClicked(1);
    }
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        SpeedMult = keyCode - ALLEGRO_KEY_0;
    }
}
void PlayScene::Hit() {
    lives--;
    // not sure
    UILives->Text = std::string("Life ") + std::to_string(lives);
    if (lives <= 0) {
        if(endlessMode)
        {
            // If in endless mode, reset the round and money.
            endlessMode = false;
            endlessRound = 1;
            EarnMoney(-money);
            money = 0;
            Engine::GameEngine::GetInstance().ChangeScene("win");
        }
        else
        {
            Engine::GameEngine::GetInstance().ChangeScene("lose");
        }
        
    }
}
int PlayScene::GetMoney() const {
    return money;
}

void PlayScene::EarnMoney(int money) {
    if (money < 0) Score += -money;
    this->money += money;
    UIMoney->Text = std::string("$") + std::to_string(this->money);
}
void PlayScene::ReadMap() {
    std::string filename = std::string("Resource/map") + std::to_string(MapId) + ".txt";
    // Read map file.
    char c;
    std::vector<bool> mapData;
    std::ifstream fin(filename);
    while (fin >> c) {
        switch (c) {
            case '0': mapData.push_back(false); break;
            case '1': mapData.push_back(true); break;
            case '\n':
            case '\r':
                if (static_cast<int>(mapData.size()) / MapWidth != 0)
                    throw std::ios_base::failure("Map data is corrupted.");
                break;
            default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    // Validate map data.
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    // Store map in 2d array.
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            if (num)
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            else
                TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
}
void PlayScene::ReadEnemyWave() {
    std::string filename = std::string("Resource/enemy") + std::to_string(MapId) + ".txt";
    // Read enemy file.
    float type, wait, repeat;
    enemyWaveData.clear();
    std::ifstream fin(filename);
    while (fin >> type && fin >> wait && fin >> repeat) {
        for (int i = 0; i < repeat; i++)
            enemyWaveData.emplace_back(type, wait);
    }
    fin.close();
}
void PlayScene::ConstructUI() {
    // Background
    UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
    TurretButton *btn;
    // Button 1
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/tower-base.png", 1294, 136, 0, 0, 0, 0),
                           Engine::Sprite("play/turret-1.png", 1294, 136 - 8, 0, 0, 0, 0), 1294, 136, MachineGunTurret::Price);
    // Reference: Class Member Function Pointer and std::bind.
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
    UIGroup->AddNewControlObject(btn);
    // Button 2
    btn = new TurretButton("play/floor.png", "play/dirt.png",
                           Engine::Sprite("play/tower-base.png", 1370, 136, 0, 0, 0, 0),
                           Engine::Sprite("play/turret-2.png", 1370, 136 - 8, 0, 0, 0, 0), 1370, 136, LaserTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
    UIGroup->AddNewControlObject(btn);

    // Add a new button for the Healing Turret in ConstructUI()
    // In PlayScene.cpp, add the healing turret button and logic if not already done

    btn = new TurretButton("play/floor.png", "play/dirt.png",
                       Engine::Sprite("play/tower-base.png", 1370 + 76, 136, 0, 0, 0, 0),
                       Engine::Sprite("play/turret-3.png", 1370 + 76, 136 - 8, 0, 0, 0, 0), 1370 + 76, 136, HealingTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2)); // Healing turret
    UIGroup->AddNewControlObject(btn);

    if (MapId == 2) {
        roundLabel = new Engine::Label("Round: " + std::to_string(endlessRound), "pirulen.ttf", 32, 1050, 50);
        UIGroup->AddNewObject(roundLabel);
    }


    {
        const int SHOVEL_ID = -1;
        const float sx = 1370 + 76 + 76;
        const float sy = 36;

        // TurretButton’s second parameter is the hover-image
        auto btn = new TurretButton(
            "play/floor.png",
            "play/dirt.png",     // ← correct hover asset
            Engine::Sprite("play/tool-base.png", sx, sy),
            Engine::Sprite("play/shovel.png",       sx, sy, 0, 0, 0, 0),
            sx, sy,
            0
        );
        btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, SHOVEL_ID));
        UIGroup->AddNewControlObject(btn);

        Engine::ImageButton *btnn;
        btnn = new Engine::ImageButton("play/home.png", "play/home.png",  1300, 750, 75, 75);
        btnn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 20));
        AddNewControlObject(btnn);

        btnn = new Engine::ImageButton("play/retry.png", "play/retry.png",  1375, 750, 75, 75);
        btnn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 21));
        AddNewControlObject(btnn);
    }




    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::UIBtnClicked(int id) {
    // 1) Shovel selected?
    if (id < 0) {
        Shoveling = true;
        imgTarget->Visible = false;
        // spawn the preview‐shovel if needed
        if (!shovelPreview) {
            shovelPreview = new Engine::Sprite("play/shovel.png", 0, 0);
            shovelPreview->Tint = al_map_rgba(255,255,255,200);
            UIGroup->AddNewObject(shovelPreview);
        }

        // cancel any turret‐preview
        if (preview) {
            UIGroup->RemoveObject(preview->GetObjectIterator());
            preview = nullptr;
        }
        return;
    }

    // 2) Any real turret button clears shovel
    Shoveling = false;
    if (shovelPreview) {
        UIGroup->RemoveObject(shovelPreview->GetObjectIterator());
        shovelPreview = nullptr;
    }

    if (preview)
        UIGroup->RemoveObject(preview->GetObjectIterator());
    if (id == 0 && money >= MachineGunTurret::Price)
        preview = new MachineGunTurret(0, 0);
    else if (id == 1 && money >= LaserTurret::Price)
        preview = new LaserTurret(0, 0);
    else if (id == 2 && money >= HealingTurret::Price)
        preview = new HealingTurret(0, 0);
    else if (id == 20) {
        Engine::GameEngine::GetInstance().ChangeScene("stage-select");
    }
    else if (id == 21) {
        Engine::GameEngine::GetInstance().ChangeScene("play");
    }
    if (!preview)
        return;
    preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
    preview->Tint = al_map_rgba(255, 255, 255, 200);
    preview->Enabled = false;
    preview->Preview = true;
    UIGroup->AddNewObject(preview);
    OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}


int PlayScene::GetScore() const {
    return Score;
}


bool PlayScene::CheckSpaceValid(int x, int y) {
    // 1) Out-of-bounds?
    if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
        return false;

    // 2) Temporarily occupy this tile and recompute distances
    TileType old = mapState[y][x];
    mapState[y][x] = TILE_OCCUPIED;
    auto distMap = CalculateBFSDistance();
    mapState[y][x] = old;

    // 3) Every entry point must still have a path to the exit
    for (auto &ep : entryPoints) {
        if (ep.x < 0 || ep.x >= MapWidth
         || ep.y < 0 || ep.y >= MapHeight
         || distMap[ep.y][ep.x] == -1)
            return false;
    }

    // 4) All on-grid enemies must still be on a reachable tile
    for (auto &obj : EnemyGroup->GetObjects()) {
        auto *e = dynamic_cast<Enemy *>(obj);
        int ex = int(std::floor(e->Position.x / BlockSize));
        int ey = int(std::floor(e->Position.y / BlockSize));

        // ignore enemies not yet on the play grid
        if (ex < 0 || ex >= MapWidth
         || ey < 0 || ey >= MapHeight)
            continue;

        // if any on-grid enemy has no path, reject placement
        if (distMap[ey][ex] == -1)
            return false;
    }

    // 5) Commit the new distance map and update each enemy’s path
    mapDistance = std::move(distMap);
    for (auto &obj : EnemyGroup->GetObjects())
        dynamic_cast<Enemy *>(obj)->UpdatePath(mapDistance);

    return true;
}


std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    // Initialize all distances to -1
    std::vector<std::vector<int>> dist(MapHeight,
                                       std::vector<int>(MapWidth, -1));
    std::queue<Engine::Point> q;

    // Clamp EndGridPoint to valid grid bounds
    int sx = EndGridPoint.x < 0 ? 0 : (EndGridPoint.x > MapWidth - 1 ? MapWidth - 1 : EndGridPoint.x);
    int sy = EndGridPoint.y < 0 ? 0 : (EndGridPoint.y > MapHeight - 1 ? MapHeight - 1 : EndGridPoint.y);

    // Always start BFS from the exit cell
    dist[sy][sx] = 0;
    q.push(Engine::Point(sx, sy));

    // Standard 4-way BFS over TILE_DIRT cells
    while (!q.empty()) {
        auto p = q.front(); q.pop();
        int cd = dist[p.y][p.x];
        for (auto &d : PlayScene::directions) {
            int nx = p.x + d.x, ny = p.y + d.y;
            if (nx < 0 || nx >= MapWidth || ny < 0 || ny >= MapHeight)
                continue;
            if (dist[ny][nx] != -1)             // already visited
                continue;
            if (mapState[ny][nx] != TILE_DIRT)  // only traverse the path
                continue;
            dist[ny][nx] = cd + 1;
            q.push(Engine::Point(nx, ny));
        }
    }

    return dist;
}


void PlayScene::FreeTile(int gridX, int gridY) {
    // restore to “floor” so it's buildable again but still blocks enemies
    mapState[gridY][gridX] = TILE_FLOOR;
}


void PlayScene::GenerateRandomMap(int round) {
    // 1. Fill every cell with FLOOR
    mapState.assign(MapHeight, std::vector<TileType>(MapWidth, TILE_FLOOR));

    // 2. Build lists of valid edge points (excluding corners)
    std::vector<Engine::Point> topEdge, bottomEdge, leftEdge, rightEdge;
    for (int x = 1; x < MapWidth - 1; x++) {
        topEdge   .emplace_back(x, 0);
        bottomEdge.emplace_back(x, MapHeight - 1);
    }
    for (int y = 1; y < MapHeight - 1; y++) {
        leftEdge .emplace_back(0, y);
        rightEdge.emplace_back(MapWidth - 1, y);
    }

    // 3. Randomly pick start+end on opposite sides
    int side = rand() % 4;
    Engine::Point startP, endP;
    if      (side == 0) { startP = leftEdge [rand() % leftEdge .size()]; endP = rightEdge[rand() % rightEdge.size()]; }
    else if (side == 1) { startP = rightEdge[rand() % rightEdge.size()]; endP = leftEdge [rand() % leftEdge .size()]; }
    else if (side == 2) { startP = topEdge  [rand() % topEdge  .size()]; endP = bottomEdge[rand() % bottomEdge.size()]; }
    else                { startP = bottomEdge[rand() % bottomEdge.size()]; endP = topEdge   [rand() % topEdge  .size()]; }

    std::cout << "[DEBUG] Start: (" << startP.x << "," << startP.y
              << ") → End: (" << endP.x << "," << endP.y << ")\n";

    // 4. Register entry & BFS target
    entryPoints.clear();
    entryPoints.push_back(startP);
    endPoint = endP;
    PlayScene::EndGridPoint = endP;
    spawnPoint = startP;

    std::cout
    << "[DEBUG] GenerateRandomMap: startP=("
    << startP.x << "," << startP.y << ")  "
    << "endP=("   << endP.x   << "," << endP.y   << ")\n";
    std::cout
    << "[DEBUG]   spawnPoint=("
    << spawnPoint.x << "," << spawnPoint.y << ")\n";

    // 5. Carve one continuous path, reserving exactly one dirt at start-edge and one at end-edge
    //    a) Determine orientation of start-edge
    bool isHorizontal = (startP.x == 0 || startP.x == MapWidth - 1);

    //    b) Prepare RNG
    std::mt19937 rng{ std::random_device{}() };

    //    c) First move: step off the start-edge
    std::vector<std::pair<int,int>> moves;
    if (isHorizontal) {
        int dx = (endP.x > startP.x ? 1 : -1);
        moves.emplace_back(dx, 0);
    } else {
        int dy = (endP.y > startP.y ? 1 : -1);
        moves.emplace_back(0, dy);
    }

    //    d) Compute remaining delta after first move
    Engine::Point cur = startP;
    cur.x += moves[0].first;
    cur.y += moves[0].second;
    int remDX = endP.x - cur.x;
    int remDY = endP.y - cur.y;

    //    e) Last move: step onto the end-edge
    std::pair<int,int> lastMove;
    if (isHorizontal) {
        int dx = (remDX > 0 ? 1 : -1);
        lastMove = {dx, 0};
        remDX  -= dx;
    } else {
        int dy = (remDY > 0 ? 1 : -1);
        lastMove = {0, dy};
        remDY  -= dy;
    }

    //    f) Build the “middle” moves (all remaining Manhattan steps)
    std::vector<std::pair<int,int>> middle;
    for (int i = 0; i < std::abs(remDX); ++i)
        middle.emplace_back(remDX > 0 ? 1 : -1, 0);
    for (int i = 0; i < std::abs(remDY); ++i)
        middle.emplace_back(0, remDY > 0 ? 1 : -1);

    //    g) Shuffle middle steps to create winding corridor
    std::shuffle(middle.begin(), middle.end(), rng);

    //    h) Stitch together: first, then middle, then last
    moves.insert(moves.end(), middle.begin(), middle.end());
    moves.push_back(lastMove);

    //    i) Carve path into mapState
    mapState[startP.y][startP.x] = TILE_DIRT;
    cur = startP;
    for (auto &m : moves) {
        cur.x += m.first;
        cur.y += m.second;
        mapState[cur.y][cur.x] = TILE_DIRT;
    }

    // 6. Refresh the visual tile map
    TileMapGroup->Clear();
    for (int y = 0; y < MapHeight; y++) {
        for (int x = 0; x < MapWidth; x++) {
            const char* img = (mapState[y][x] == TILE_DIRT ? "play/dirt.png" : "play/floor.png");
            TileMapGroup->AddNewObject(
                new Engine::Image(img, x * BlockSize, y * BlockSize, BlockSize, BlockSize)
            );
        }
    }

    // 7. Compute BFS distances for enemy pathfinding
    mapDistance = CalculateBFSDistance();
}



void PlayScene::GenerateEnemyWave(int round) {
    // Example: Increase difficulty each round
    enemyWaveData.clear();
    // Adjust these numbers for your game's balance
    int numBiggerCarriers = std::max(0, round / 5);
    int numCarriers       = std::max(0, round / 4);
    int numTanks          = std::max(0, round / 3);
    int numArmies         = std::max(1, round / 2);
    int numSoldiers       = 5 + round * 2;

    float wait = std::max(0.5f, 2.0f - round * 0.05f);

    for (int i = 0; i < numBiggerCarriers; ++i)
        enemyWaveData.emplace_back(5, wait + 0.8f);
    for (int i = 0; i < numCarriers; ++i)
        enemyWaveData.emplace_back(4, wait + 0.6f);
    for (int i = 0; i < numTanks; ++i)
        enemyWaveData.emplace_back(3, wait + 0.4f);
    for (int i = 0; i < numArmies; ++i)
        enemyWaveData.emplace_back(2, wait + 0.2f);
    for (int i = 0; i < numSoldiers; ++i)
        enemyWaveData.emplace_back(1, wait);

}

void PlayScene::RemoveAllTurrets() {
    // Remove all turrets from the TowerGroup and mark mapState as not occupied
    for (auto obj : TowerGroup->GetObjects()) {
        Turret* t = dynamic_cast<Turret*>(obj);
        if (t) {
            int tx = int(t->Position.x / BlockSize);
            int ty = int(t->Position.y / BlockSize);
            if (tx >= 0 && tx < MapWidth && ty >= 0 && ty < MapHeight)
                mapState[ty][tx] = TILE_FLOOR;
            EarnMoney(t->GetPrice()); // Refund turret price
        }
    }
    TowerGroup->Clear();
    if (roundLabel) roundLabel->Text = "Round: " + std::to_string(endlessRound);
}