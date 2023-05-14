#pragma once

#include "Player.h"
#include "Reward.h"

//This enum is used for implementing the game management FSM
enum class Enum_GameMode
{
	GM_0_MAINMENU = 0, GM_1_GAMEPLAY, GM_2_LEVEL_WON, GM_3_GAME_OVER, GM_4_GAME_WON
};

//The game manager class
class Game
{
public:
	Game(const sf::Texture& pawnTexture, const sf::Texture& blockTexture, const sf::Texture& pointTexture, const sf::Font& font);
	~Game() = default;

	void update(const sf::Time elapsedTime);
	void draw(sf::RenderWindow& window);

	void onJumpAction();
	void onJumpActionReleased();
	void setMovement(sf::Vector2i direction);
	void cheat();

	static sf::String getName()
	{
		return "Isometric Platformer (by Amin Babadi, June 2022)";
	}

private:
	//general variables for processing the game state machine
	Enum_GameMode m_gameMode;
	int m_levelIdx, m_nHearts;

	//sprites
	sf::Sprite m_pawnSprite;
	sf::Sprite m_blockSprite;
	sf::Sprite m_pointSprite;

	//texts
	sf::Text m_mainMenuText, m_levelStartText, m_levelWonText, m_gameOverText, m_gameWonText;

	//level grid
	int m_nRows, m_nCols;
	std::vector<std::vector<bool>> m_levelBlockGrid; //true means block exists, false means no block
	
	//player and rewards
	std::unique_ptr<Player> m_player;
	std::vector<Reward> m_rewards;

	//initialization
	void setGameMode(Enum_GameMode mode);
	void loadLevel(const int levelIdx, const bool resetHearts);
	void initAssets(const sf::Texture& pawnTexture, const sf::Texture& blockTexture, const sf::Texture& pointTexture, const sf::Font& font);
	void initPlayer(const int r, const int c);
	void initReward(const int r, const int c);
	//update
	void updateMainMenu(const float dt);
	void updateGameplay(const float dt);
	void updateLevelWon(const float dt);
	void updateGameover(const float dt);
	void updateGameWon(const float dt);
	//draw
	void drawMainMenu(sf::RenderWindow& window);
	void drawGameplay(sf::RenderWindow& window);
	void drawLevelWon(sf::RenderWindow& window);
	void drawGameover(sf::RenderWindow& window);
	void drawGameWon(sf::RenderWindow& window);
	void drawLevel(sf::RenderWindow& window, const bool drawPlayerWithinLevel);
	void drawPlayer(sf::RenderWindow& window);
	//translate row-column coordinates into x-y coordinates
	void RC2XY(const int r, const int c, float& x, float& y) const;
	void RC2XY(const int r, const int c, Vec2f& p) const { RC2XY(r, c, p.x, p.y); }
	//translate x-y coordinates into row-column coordinates
	void XY2RC(const float x, const float y, int& r, int& c) const;
	void XY2RC(const Vec2f p, int& r, int& c) const { XY2RC(p.x, p.y, r, c); }
	//check if some point is within the level boundaries (assuming no holes in the level)
	bool isPointWithinLevelRect(const float x, const float y) const;
	bool isPointWithinLevelRect(const Vec2f p)  const { return isPointWithinLevelRect(p.x, p.y); }
	//check if some point is within the level boundaries and on the ground
	bool isPointOnGround(const float x, const float y) const;
	bool isPointOnGround(const Vec2f p) const { return isPointOnGround(p.x, p.y); }
	//check if the player should fall down and die!
	bool isPlayerFalling() const;
	bool isGameOver() const { return m_gameMode == Enum_GameMode::GM_3_GAME_OVER; }
};