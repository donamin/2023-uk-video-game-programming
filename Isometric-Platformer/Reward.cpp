#include "Reward.h"
#include "UtilsAssets.h"

Reward::Reward(sf::Sprite sprite, const float x, const float y, const float oscillationFrequency, const float oscillationRange)
{
	this->sprite = sprite;
	
	m_position = Vec3f(x, y, 0);

	U_initShadow(sprite, shadow);

	m_lifeTimeSeconds = 0;
	m_oscillationFrequency = oscillationFrequency;
	m_oscillationRange = oscillationRange;
}

void Reward::update(const float dt)
{
	m_lifeTimeSeconds += dt;
	//Oscilate the z component using a simple sine wave formula A sin(wt)
	m_position.z = -m_oscillationRange * (static_cast<float>(sin(2.0f * PI * m_oscillationFrequency * m_lifeTimeSeconds)) + 1.0f) / 2.0f;
	U_updateShadowScaleWhileJumping(shadow, m_position.z, m_oscillationRange);
}

void Reward::draw(sf::RenderWindow& window)
{
	//Draw the shadow
	U_drawObjectXY(shadow, m_position.x, m_position.y, window);
	//Draw the sprite
	U_drawObjectXY(sprite, m_position.x, m_position.y + m_position.z, window);
}