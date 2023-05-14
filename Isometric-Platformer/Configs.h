//Configuration parameters for customizing the game

#pragma once

//#define FINAL_BUILD

//General setup (resolution, debug, playtest, etc.)
constexpr int G_SCREEN_WIDTH = 1280, G_SCREEN_HEIGHT = 720;
constexpr int G_N_LEVELS = 4;
#ifndef FINAL_BUILD
constexpr bool G_GOD_MODE = true;
constexpr bool G_CHEAT_CODE_ENABLED = true;
#endif

//UI
#define G_UI_MAIN_MENU_TEXT sf::String("Press JUMP to start!")
#define G_UI_GAME_OVER_TEXT sf::String("Press JUMP to try again!")
#define G_UI_GAME_WON_TEXT sf::String("Press JUMP to start over!")

//Player
constexpr int G_PLAYER_N_HEARTS_START = 5;
constexpr float G_PLAYER_BLINK_AFTER_RESET_TIME = 1;
constexpr float G_PLAYER_MAX_VELOCITY = G_SCREEN_WIDTH / 2;
constexpr float G_PLAYER_MIN_VELOCITY = 0.01f * G_PLAYER_MAX_VELOCITY;
constexpr float G_PLAYER_FRICTION = 5.0f;
constexpr float G_PLAYER_JUMP_TIME = 0.35f;
constexpr float G_PLAYER_JUMP_HEIGHT = 0.15f * G_SCREEN_HEIGHT;

//Reward
constexpr float G_REWARD_GEM_OSCILATION_FREQUENCY = 1.0f, G_REWARD_GEM_OSCILATION_RANGE = 0.025f * G_SCREEN_HEIGHT;
constexpr float G_REWARD_GEM_COLLIDER_RADIUS = 50.0f;

//Shadow (shared across player and reward)
constexpr float G_SHADOW_RADIUS_SCALER = 0.5f, G_SHADOW_HEIGHT_SCALER = 0.6f;
#define G_SHADOW_COLOR sf::Color(0, 0, 0, 127)