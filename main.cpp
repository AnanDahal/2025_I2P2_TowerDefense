// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/AfterScene.h"
#include "Scene/BeforeScene.h"
#include "Scene/CampaignScene.h"
#include "Scene/ClosingScene.h"
#include "Scene/CreditScene.h"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "Scene/LeaderBoardScene.h"
#include "Scene/OpeningScene.h"
#include "Scene/LogInScene.h"
#include "Scene/PasswordScene.h"
#include "Scene/PowersScene.h"
#include "Scene/ShopScene.h"

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

	// Register all scenes
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("settings", new SettingsScene());
	game.AddNewScene("opening", new OpeningScene());
	game.AddNewScene("closing", new ClosingScene());
	game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("scoreboard-scene", new LeaderBoardScene());
	game.AddNewScene("log-in", new LogInScene());
	game.AddNewScene("password", new PasswordScene());
	game.AddNewScene("powers", new PowersScene());
	game.AddNewScene("campaign", new CampaignScene());
	game.AddNewScene("before", new BeforeScene());
	game.AddNewScene("after", new AfterScene());  // Add the AfterScene
	game.AddNewScene("shop", new ShopScene());
	game.AddNewScene("credits", new CreditScene());
	// Start the game
	game.Start("opening", 60, 1600, 832);
	return 0;
}
