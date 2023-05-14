#pragma once

#include "UtilsDefs.h"
#include "Configs.h"

//Main class for handling the player
class Player
{
public:
	Player(sf::Sprite sprite);
	void reset(const Vec2f& pos);
	void reset(const float x, const float y) { reset(Vec2f(x, y)); }
	void reset() { reset(0, 0); }
	//The position is modeled using a 3D vector to also account for the jumping height offset.
	Vec3f getPosition() const { return m_position; }
	//The draw position is the 2D version of the original position. For the sprite, it returns (x, y + z). For the shadow, it returns (x, y)
	Vec2f getDrawPosition(const bool sprite) const { return sprite ? Vec2f(m_position.x, m_position.y + m_position.z) : Vec2f(m_position.x, m_position.y); }
	void update(const float dt);
	void draw(sf::RenderWindow& window);
	bool isJumping() const { return m_jumpTimerSeconds > 0; }
	//When jumping, this function computes a ratio between 0 and 1, to determine what portion of max jumping height the character is in (0: ground level -> 1: max jump height).
	float getJumpHeightProgressRatio() const { return abs(m_position.z / G_PLAYER_JUMP_HEIGHT); }
	bool isOnTheGround() const { return getJumpHeightProgressRatio() < 0.25f; }
	void jump() { if (!isResetBlinking() && !isJumping()) m_jumpTimerSeconds = G_PLAYER_JUMP_TIME; };
	void stopMoving(const bool instantly);
	void setAcceleration(const float x, const float y);
	void setAcceleration(const Vec2f& acc) { setAcceleration(acc.x, acc.y); }
	void die() { alive = false; }
private:
	sf::Sprite sprite;
	sf::CircleShape shadow;

	float m_squaredMinVelocity, m_squaredMaxVelocity;

	float m_resetTimerSeconds;

	Vec3f m_position;
	Vec2f m_velocity, m_acceleration;
	float m_jumpTimerSeconds;

	bool alive;

	bool isResetBlinking() const { return m_resetTimerSeconds > 0; }
	sf::FloatRect getRect() const { return sprite.getGlobalBounds(); }
	void updateKinematics(Vec3f &pos, const float dt);
	void updateJump(Vec3f& pos, const float dt);
};