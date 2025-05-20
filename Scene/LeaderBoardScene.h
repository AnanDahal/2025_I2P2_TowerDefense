#ifndef LEADERBOARDSCENE_H
#define LEADERBOARDSCENE_H

#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include <allegro5/allegro_audio.h>

class LeaderBoardScene final : public Engine::IScene {
private:
    Engine::ImageButton* backControl = nullptr;
    Engine::Label* backLabel = nullptr;
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
    std::vector<std::tuple<std::string, int, std::string>> scores;
    int currentPage = 0;
    static const int ENTRIES_PER_PAGE = 7;

    void LoadScores();
    void DisplayCurrentPage();
    void ClearScoreObjects();

public:
    explicit LeaderBoardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick(int stage);
    void NextPageOnClick();
    void PrevPageOnClick();
};

#endif // LEADERBOARDSCENE_H
