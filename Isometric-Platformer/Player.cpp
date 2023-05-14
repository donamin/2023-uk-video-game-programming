#include "Player.h"
#include "UtilsAssets.h"

Player::Player(sf::Sprite sprite)
{
	this->sprite = sprite;

	U_initShadow(sprite, shadow);

	//Use squared velocity values for better efficiency
	m_squaredMinVelocity = G_PLAYER_MIN_VELOCITY * G_PLAYER_MIN_VELOCITY;
	m_squaredMaxVelocity = G_PLAYER_MAX_VELOCITY * G_PLAYER_MAX_VELOCITY;
}

//Stop movement of the player (without resetting the game)
void Player::stopMoving(const bool instantly)
{
	if(instantly) //if needed, reset the velocity as well.
		m_velocity = Vec2f(0, 0);
	m_acceleration = Vec2f(0, 0);
}

//Stop the player and reset all timers (usually called at the beginning of a level)
void Player::reset(const Vec2f& pos)
{
	m_position = Vec3f(pos.x, pos.y, 0);
	stopMoving(true);

	m_resetTimerSeconds = G_PLAYER_BLINK_AFTER_RESET_TIME;
	m_jumpTimerSeconds = 0;

	alive = true;
}

void Player::update(const float dt)
{
	if (alive)
	{
		if (isResetBlinking())
		{
			//Wait until the blinking is over
			m_resetTimerSeconds -= dt;
		}
		else
		{
			//Compute the movement on the XY plane
			updateKinematics(m_position, dt);
			//Compute z while jumping
			updateJump(m_position, dt);
		}
	}
	else
	{
		//Literally, drop dead :D
		m_position.z += 2 * G_PLAYER_MAX_VELOCITY * dt;
	}
}

void Player::updateKinematics(Vec3f& pos, const float dt)
{
	//Squared magnitudes of velocity and acceleration are used for better performance
	float velSqrMagnitude = U_SquaredMagnitude(m_velocity);
	float accSqrMagnitude = U_SquaredMagnitude(m_acceleration);

	//Update the velocity
	if (accSqrMagnitude > 0)
		//Apply the acceleration if WASD keys are pressed.
		m_velocity += m_acceleration * dt;
	else if (velSqrMagnitude > m_squaredMinVelocity)
		//Apply friction if WASD keys are not pressed.
		m_velocity -= G_PLAYER_FRICTION * m_velocity * dt;

	if (velSqrMagnitude > m_squaredMaxVelocity)
	{
		//Limit the velocity using maxVelocity
		m_velocity *= G_PLAYER_MAX_VELOCITY / sqrt(velSqrMagnitude);
	}
	else if (accSqrMagnitude < FLT_EPSILON && velSqrMagnitude < m_squaredMinVelocity)
	{
		//Stop the object if it is moving too slowly.
		m_velocity *= 0.0f;
	}

	//Update the position using the velocity.
	pos = m_position + Vec3f(m_velocity.x, m_velocity.y, 0) * dt;
}

void Player::updateJump(Vec3f& pos, const float dt)
{
	if (!isJumping()) return;

	m_jumpTimerSeconds -= dt;
	if (m_jumpTimerSeconds > 0)
	{
		//Jump movement is modelled using ax^2+bx formula.
		const float k = 4.f * G_PLAYER_JUMP_HEIGHT / U_Squared(G_PLAYER_JUMP_TIME);
		pos.z = k * m_jumpTimerSeconds * (m_jumpTimerSeconds - G_PLAYER_JUMP_TIME);
	}
	else
	{
		//Jump finished.
		m_jumpTimerSeconds = 0;
		pos.z = 0;
	}
	//Update the shadow scale to give a better intuition of its z value.
	U_updateShadowScaleWhileJumping(shadow, pos.z, G_PLAYER_JUMP_HEIGHT);
}

void Player::draw(sf::RenderWindow& window)
{
	if (isResetBlinking() && (int)(m_resetTimerSeconds * 10) % 2 == 0)
		//Skip the drawing after reseting the player to create a blinking effect.
		return;
	//Draw the shadow only if the player is not dead.
	if (alive)
		U_drawObject(shadow, getDrawPosition(false), window);
	//Draw the sprite
	U_drawObject(sprite, getDrawPosition(true), window);
}

//Override the acceleration
void Player::setAcceleration(const float x, const float y)
{
	if (isResetBlinking())
		return;
	m_acceleration.x = x;
	m_acceleration.y = y;
}