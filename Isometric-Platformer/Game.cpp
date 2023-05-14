#include <iostream>
#include <fstream>
#include <random>
#include <cmath>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "UtilsDefs.h"
#include "Configs.h"
#include "UtilsMath.h"
#include "UtilsAssets.h"

Game::Game(const sf::Texture& pawnTexture, const sf::Texture& blockTexture, const sf::Texture& pointTexture, const sf::Font& font)
{
	srand((unsigned int)time(nullptr));

	initAssets(pawnTexture, blockTexture, pointTexture, font);

	m_player = std::unique_ptr<Player>(new Player(m_pawnSprite));

	setGameMode(Enum_GameMode::GM_0_MAINMENU);
}

void Game::update(sf::Time elapsedTime)
{
	if (!m_player)
		return;
	const float dt = elapsedTime.asSeconds();
	
	switch (m_gameMode)
	{
		case Enum_GameMode::GM_0_MAINMENU:
			updateMainMenu(dt);
			break;
		case Enum_GameMode::GM_1_GAMEPLAY:
			updateGameplay(dt);
			break;
		case Enum_GameMode::GM_2_LEVEL_WON:
			updateLevelWon(dt);
			break;
		case Enum_GameMode::GM_3_GAME_OVER:
			updateGameover(dt);
			break;
		case Enum_GameMode::GM_4_GAME_WON:
			updateGameWon(dt);
			break;
	}
}

void Game::draw(sf::RenderWindow& window)
{
	if (!m_player)
		return;
	switch (m_gameMode)
	{
		case Enum_GameMode::GM_0_MAINMENU:
			drawMainMenu(window);
			break;
		case Enum_GameMode::GM_1_GAMEPLAY:
			drawGameplay(window);
			break;
		case Enum_GameMode::GM_2_LEVEL_WON:
			drawLevelWon(window);
			break;
		case Enum_GameMode::GM_3_GAME_OVER:
			drawGameover(window);
			break;
		case Enum_GameMode::GM_4_GAME_WON:
			drawGameWon(window);
			break;
	}
}

void Game::onJumpAction()
{
	switch (m_gameMode)
	{
		case Enum_GameMode::GM_0_MAINMENU:
			//Start from level 1
			loadLevel(1, true);
			setGameMode(Enum_GameMode::GM_1_GAMEPLAY);
			break;
		case Enum_GameMode::GM_1_GAMEPLAY:
			//Jump
			m_player->jump();
			break;
		case Enum_GameMode::GM_2_LEVEL_WON:
			if (m_levelIdx < G_N_LEVELS)
			{
				//Go to the next level
				loadLevel(m_levelIdx + 1, false);
				setGameMode(Enum_GameMode::GM_1_GAMEPLAY);
			}
			else
			{
				//Go to the final page
				setGameMode(Enum_GameMode::GM_4_GAME_WON);
			}
			break;
		case Enum_GameMode::GM_3_GAME_OVER:
			if (m_nHearts < 0)
			{
				//Return to level 1 if player's out of hearts.
				loadLevel(1, true);
				setGameMode(Enum_GameMode::GM_1_GAMEPLAY);
			}
			break;
		case Enum_GameMode::GM_4_GAME_WON:
			//Restart from level 1
			loadLevel(1, true);
			setGameMode(Enum_GameMode::GM_1_GAMEPLAY);
			break;
	}
}

void Game::onJumpActionReleased()
{
}

void Game::setMovement(sf::Vector2i direction)
{
	if(m_gameMode != Enum_GameMode::GM_1_GAMEPLAY || m_player == nullptr) return;

	m_player->setAcceleration(Vec2f(G_SCREEN_WIDTH * direction));
}

void Game::cheat()
{
#ifndef FINAL_BUILD
	if(G_CHEAT_CODE_ENABLED && m_gameMode == Enum_GameMode::GM_1_GAMEPLAY)
		setGameMode(Enum_GameMode::GM_2_LEVEL_WON);
#endif
}

//initialize all sprite and text assets for later use
void Game::initAssets(const sf::Texture& pawnTexture, const sf::Texture& blockTexture, const sf::Texture& pointTexture, const sf::Font& font)
{
	//sprites
	U_initSprite(m_pawnSprite, pawnTexture, 40, 85);
	U_initSprite(m_blockSprite, blockTexture, 80, 39);
	U_initSprite(m_pointSprite, pointTexture, 12, 23);

	//texts
	U_initText(m_mainMenuText, G_UI_MAIN_MENU_TEXT, font, 60, sf::Text::Regular, sf::Color::White);
	U_initText(m_levelStartText, font, 30, sf::Text::Regular, sf::Color::White);
	U_initText(m_levelWonText, font, 60, sf::Text::Bold || sf::Text::Italic, sf::Color::Blue, sf::Color::White, 2.0f);
	U_initText(m_gameOverText, G_UI_GAME_OVER_TEXT, font, 60, sf::Text::Bold, sf::Color::Red, sf::Color::White, 2.5f);
	U_initText(m_gameWonText, G_UI_GAME_WON_TEXT, font, 60, sf::Text::Bold, sf::Color::Green, sf::Color::White, 2.5f);
}

void Game::setGameMode(Enum_GameMode mode)
{
	m_gameMode = mode;
	switch (m_gameMode)
	{
		case Enum_GameMode::GM_0_MAINMENU:
			break;
		case Enum_GameMode::GM_1_GAMEPLAY:
			break;
		case Enum_GameMode::GM_2_LEVEL_WON:
			m_player->stopMoving(false);
			break;
		case Enum_GameMode::GM_3_GAME_OVER:
			m_player->die();
#ifndef FINAL_BUILD
			if(!G_GOD_MODE)
#endif
				m_nHearts--;
			break;
		case Enum_GameMode::GM_4_GAME_WON:
			break;
	}
}

void Game::loadLevel(const int levelIdx, const bool resetHearts)
{
	m_levelIdx = levelIdx;

	if(resetHearts)
		m_nHearts = G_PLAYER_N_HEARTS_START;

	m_rewards.clear();

	char buffer[500];
	sprintf_s(buffer, "Assets//Level%02d.txt\0", levelIdx);

	std::ifstream fin(buffer, std::ifstream::in);

	//read level dimensions
	fin >> m_nRows >> m_nCols;
	fin.get();

	//read level grid
	m_levelBlockGrid.resize(m_nRows);
	for_loop(r, m_nRows)
	{
		m_levelBlockGrid[r].resize(m_nCols);
		fin.getline(buffer, 500);
		for_loop(c, m_nCols)
		{
			switch (buffer[c])
			{
				case 'P':
					//Player
					m_levelBlockGrid[r][c] = true;
					initPlayer(r, c);
					break;
				case 'R':
					//Reward
					m_levelBlockGrid[r][c] = true;
					initReward(r, c);
					break;
				case '-':
					//Normal block
					m_levelBlockGrid[r][c] = true;
					break;
				default:
					//No block
					m_levelBlockGrid[r][c] = false;
					break;
			}
		}
	}

	//read level welcome string
	fin.getline(buffer, 500);
	char levelString[500];
	sprintf_s(levelString, "Level %02d: %s\0", levelIdx, buffer);
	m_levelStartText.setString(levelString);

	//read level final string
	fin.getline(buffer, 500);
	m_levelWonText.setString(buffer);
}

void Game::initPlayer(const int r, const int c)
{
	float x, y;
	RC2XY(r, c, x, y);
	m_player->reset(x, y);
}

void Game::initReward(const int r, const int c)
{
	float x, y;
	RC2XY(r, c, x, y);
	m_rewards.push_back(Reward(m_pointSprite, x, y, G_REWARD_GEM_OSCILATION_FREQUENCY, G_REWARD_GEM_OSCILATION_RANGE));
}

void Game::updateMainMenu(const float dt)
{
}

void Game::updateGameplay(const float dt)
{
	//Update the player
	m_player->update(dt);
	//Game logic
	if (isPlayerFalling())
	{
		//The player has fallen down
		setGameMode(Enum_GameMode::GM_3_GAME_OVER);
	}
	else if(m_player->isOnTheGround()) //Check if player is on the ground or near the ground (to check collision with reward objects)
	{
		//check if player's collected any reward objects
		auto hasPlayerCollectedReward = [&](const Reward& reward) -> bool
		{
			return U_Distance(U_GetXY(m_player->getPosition()), U_GetXY(reward.getPosition())) < G_REWARD_GEM_COLLIDER_RADIUS;
		};
		int nRewardsBefore = (int)m_rewards.size();
		m_rewards.erase(std::remove_if(m_rewards.begin(), m_rewards.end(), hasPlayerCollectedReward), m_rewards.end());
		if (nRewardsBefore > 0 && m_rewards.size() == 0)
		{
			//all rewards have been collected. Game won!
			setGameMode(Enum_GameMode::GM_2_LEVEL_WON);
		}
	}
	//Update all remaining rewards.
	for (Reward& r : m_rewards)
		r.update(dt);
}

void Game::updateLevelWon(const float dt)
{
	//Update the player
	m_player->update(dt);
	//Update the reward
	for (Reward& r : m_rewards)
		r.update(dt);
}

void Game::updateGameover(const float dt)
{
	//Update the player
	m_player->update(dt);
	//game over animation is over when the player goes out of the screen
	if (m_player->getDrawPosition(true).y > G_SCREEN_HEIGHT)
	{
		//restart the current level if the player still has hearts
		if (m_nHearts >= 0)
		{
			setGameMode(Enum_GameMode::GM_1_GAMEPLAY);
			loadLevel(m_levelIdx, false);
		}
	}
	//Update the rewards
	for (Reward &r : m_rewards)
		r.update(dt);
}

void Game::updateGameWon(const float dt)
{
	//Update the player
	m_player->update(dt);
	//Update the rewards
	for (Reward& r : m_rewards)
		r.update(dt);
}

void Game::drawMainMenu(sf::RenderWindow& window)
{
	U_drawText(window, m_mainMenuText, G_SCREEN_WIDTH / 2, G_SCREEN_HEIGHT / 2);
}

void Game::drawGameplay(sf::RenderWindow& window)
{
	drawLevel(window, false);
	drawPlayer(window);
	U_drawText(window, m_levelStartText, G_SCREEN_WIDTH / 2, G_SCREEN_HEIGHT - 50);
}

void Game::drawLevelWon(sf::RenderWindow& window)
{
	drawLevel(window, false);
	drawPlayer(window);
	U_drawText(window, m_levelWonText, G_SCREEN_WIDTH / 2, G_SCREEN_HEIGHT / 2);
}

void Game::drawGameover(sf::RenderWindow& window)
{
	//When the player is falling down, we should re-arrange the order of drawing block sprites to avoid visual bugs.

	//translate's player's x-y coordinate into row-column coordinate
	int playerR = 0, playerC = 0;
	Vec2f playerPosXY = U_GetXY(m_player->getPosition());
	XY2RC(playerPosXY, playerR, playerC);
	
	//this is used to make sure that we don't draw the player twice
	bool drewPlayer = false;

	//Player should be drawn behind the level blocks
	if (playerR < 0 || playerC >= m_nCols)
	{
		drawPlayer(window);
		drewPlayer = true;
	}

	//Player should be drawn within the level blocks
	bool playerDeadBetweenBlocks = isPointWithinLevelRect(playerPosXY);
	drawLevel(window, playerDeadBetweenBlocks);
	if (playerDeadBetweenBlocks)
		drewPlayer = true;

	//Player should be drawn on top of the level blocks
	if (!isGameOver() || !drewPlayer)
	{
		drawPlayer(window);
		drewPlayer = true;
	}

	if (m_nHearts >= 0) //Draw small hearts on top of the screen
		U_drawText(window, m_levelStartText, G_SCREEN_WIDTH / 2, G_SCREEN_HEIGHT - 50);
	else // Draw the game over text
		U_drawText(window, m_gameOverText, G_SCREEN_WIDTH / 2, G_SCREEN_HEIGHT / 2);
}

void Game::drawGameWon(sf::RenderWindow& window)
{
	//drawLevel(window, false);
	//drawPlayer(window);
	U_drawText(window, m_gameWonText, G_SCREEN_WIDTH / 2, G_SCREEN_HEIGHT / 2);
}

void Game::drawLevel(sf::RenderWindow& window, const bool drawPlayerWithinLevel)
{
	//if the player's drawing layer is between the blocks' layers, first compute its row-column coordinate
	int playerR = 0, playerC = 0;
	if(drawPlayerWithinLevel)
		XY2RC(U_GetXY(m_player->getPosition()), playerR, playerC);
	for_loop(r, m_nRows)
	{
		for_loop_reverse(c, m_nCols)
		{
			if (drawPlayerWithinLevel && r == playerR && c == playerC)
			{
				//Draw the player between the blocks if its falling down from within the level
				drawPlayer(window);
			}

			if (m_levelBlockGrid[r][c])
			{
				//Draw the block
				float x, y;
				RC2XY(r, c, x, y);
				U_drawObjectXY(m_blockSprite, x, y, window);
			}
		}
	}

	//Draw the rewards
	for (Reward r : m_rewards)
		r.draw(window);
}

void Game::drawPlayer(sf::RenderWindow& window)
{
	if (!m_player)
		return;

	//Draw the player
	m_player->draw(window);

	//Draw small copies of the player on top of screen to show the remaining number of hearts
	m_pawnSprite.setScale(0.5f, 0.5f);
	sf::FloatRect playerRect = m_pawnSprite.getGlobalBounds();
	for_loop(h, m_nHearts)
		U_drawObjectXY(m_pawnSprite, (float)G_SCREEN_WIDTH - (h + 0.5f) * (1.1f * playerRect.width), (float)playerRect.height, window)
}

//translate row-column coordinates into x-y coordinates
void Game::RC2XY(const int r, const int c, float& x, float& y) const
{
	sf::FloatRect refBlockRect = m_blockSprite.getGlobalBounds();
	float levelWidth = (float)(m_nRows + m_nCols) / 2 * refBlockRect.width;
	// float levelHeight = (1 + (float)(m_nRows + m_nCols - 2) / 4) * refBlockRect.height;
		
	x = G_SCREEN_WIDTH / 2
		- levelWidth / 2
		+ (float)(r + c + 1) * refBlockRect.width / 2;
	y = G_SCREEN_HEIGHT / 2
		+ (float)(m_nCols - m_nRows + r - c) * refBlockRect.height / 4;
}

//translate x-y coordinates into row-column coordinates
void Game::XY2RC(const float x, const float y, int& r, int& c) const
{
	sf::FloatRect refBlockRect = m_blockSprite.getGlobalBounds();
	float levelWidth = (float)(m_nRows + m_nCols) / 2 * refBlockRect.width;
	// float levelHeight = (1 + (float)(m_nRows + m_nCols - 2) / 4) * refBlockRect.height;
	float tempR = ((y - G_SCREEN_HEIGHT / 2.0f) * 4.0f / refBlockRect.height - m_nCols + m_nRows + (2 * x - G_SCREEN_WIDTH + levelWidth) / refBlockRect.width - 1) / 2.0f;
	float tempC = (2.0f * x - G_SCREEN_WIDTH + levelWidth) / refBlockRect.width - tempR - 1;
	r = static_cast<int>(round(tempR));
	c = static_cast<int>(round(tempC));
}

//check if some point is within the level boundaries (assuming no holes in the level)
bool Game::isPointWithinLevelRect(const float x, const float y) const
{
	int r = 0, c = 0;
	XY2RC(x, y, r, c);
	return r >= 0 && r < m_nRows && c >= 0 && c < m_nCols;
}

//check if some point is within the level boundaries and on the ground
bool Game::isPointOnGround(const float x, const float y) const
{
	int r = 0, c = 0;
	XY2RC(x, y, r, c);
	return isPointWithinLevelRect(x, y) && m_levelBlockGrid[r][c];
}

//check if the player should fall down and die!
bool Game::isPlayerFalling() const
{
	return !m_player->isJumping() && !isPointOnGround(U_GetXY(m_player->getPosition()));
}