#pragma once

#include <SFML/Graphics.hpp>
#include "UtilsDefs.h"

//Class for the reward objects in the level
class Reward
{
public:
	Reward(sf::Sprite sprite, const float x, const float y, const float m_oscillationFrequency, const float m_oscillationRange);
	Reward(sf::Sprite sprite, const Vec2f& pos, const float m_oscillationFrequency, const float m_oscillationRange)
		: Reward(sprite, pos.x, pos.y, m_oscillationFrequency, m_oscillationRange) {}
	void update(const float dt);
	void draw(sf::RenderWindow& window);
	Vec3f getPosition() const { return m_position; }
private:
	sf::Sprite sprite;
	sf::CircleShape shadow;

	//Parameters for customizing the oscillation movement.
	float m_lifeTimeSeconds, m_oscillationFrequency, m_oscillationRange;

	Vec3f m_position;
};