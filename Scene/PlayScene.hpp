#ifndef PLAYSCENE_HPP
#define PLAYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <list>
#include <memory>
#include <utility>
#include <vector>
#include "Turret/Turret.hpp"          // for dynamic_cast<Turret*>
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "UI/Component/ChatBox.hpp"  // for chatbox
#include <map>

extern int OnStage;
extern int core_memories;
extern int endless_score;
extern int whichPower;

enum RoundTransitionState {
    NONE,
    WAIT_BEFORE_ROUND_LABEL,
    SHOW_ROUND_LABEL,
    WAIT_AFTER_ROUND_LABEL
};

class Turret;
namespace Engine {
    class Group;
    class Image;
    class Label;
    class Sprite;
}   // namespace Engine

class PlayScene final : public Engine::IScene {
private:
    std::shared_ptr<Engine::ChatBox> chatBox;
    enum TileType {
        TILE_DIRT,
        TILE_FLOOR,
        TILE_OCCUPIED,
        TILE_VOID,
    };
    ALLEGRO_SAMPLE_ID bgmId;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> deathBGMInstance;

    bool AIPlacedThisRound = false;
    void AutoPlaceTurrets();

protected:
    int lives;
    int money;
    int SpeedMult;
    int Score;
    bool endlessMode = false;
    int endlessRound = 0;
    RoundTransitionState roundTransitionState = NONE;
    float roundTransitionTimer = 0.0f;
    int nextRoundNumber = 0;

public:
    static bool DebugMode;
    static const std::vector<Engine::Point> directions;
    static const int MapWidth, MapHeight;
    static const int BlockSize;
    static const float DangerTime;
    static Engine::Point SpawnGridPoint;
    static Engine::Point EndGridPoint;
    static const std::vector<int> code;
    int MapId;
    float ticks;
    float deathCountDown;
    // Map tiles.
    Group *TileMapGroup;
    Group *GroundEffectGroup;
    Group *DebugIndicatorGroup;
    Group *BulletGroup;
    Group *TowerGroup;
    Group *EnemyGroup;
    Group *EffectGroup;
    Group *UIGroup;
    Engine::Label *UIMoney;
    Engine::Label *UILives;
    Engine::Image *imgTarget;
    Engine::Sprite *dangerIndicator;
    Turret *preview;
    std::vector<std::vector<TileType>> mapState;
    std::vector<std::vector<int>> mapDistance;
    std::list<std::pair<int, float>> enemyWaveData;
    std::list<int> keyStrokes;
    void FreeTile(int gridX, int gridY);
    static Engine::Point GetClientSize();
    explicit PlayScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void OnMouseDown(int button, int mx, int my) override;
    void OnMouseMove(int mx, int my) override;
    void OnMouseUp(int button, int mx, int my) override;
    void OnKeyDown(int keyCode) override;
    void Hit();
    int GetMoney() const;
    void EarnMoney(int money);
    void ReadMap();
    void ReadEnemyWave();
    void ConstructUI();
    void UIBtnClicked(int id);
    bool CheckSpaceValid(int x, int y);
    void GenerateRandomMap(int round);
    void GenerateEnemyWave(int round);
    void RemoveAllTurrets();
    std::vector<std::vector<int>> CalculateBFSDistance();
    int GetScore() const;
    // shovel
    Engine::Sprite* shovelPreview = nullptr;
    bool Shoveling = false;

    std::vector<Engine::Point> entryPoints; 
    Engine::Point endPoint;                 // End point (grid coordinate)
    Engine::Point spawnPoint;

Engine::Label* roundLabel = nullptr;
    std::map<std::pair<int, int>, std::pair<int, int>> parent; // For reconstructing the path
    // void ModifyReadMapTiles();
    float bombCooldown = 0.0f;
    float moneyRainCooldown = 0.0f;

    const float bombCooldownTime = 0.0f;       // 10 seconds
    const float moneyRainCooldownTime = 15.0f;  // 15 seconds
};
#endif   // PLAYSCENE_HPP