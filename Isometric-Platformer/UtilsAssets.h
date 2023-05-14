//Asset utility functions

#pragma once

#include <SFML/Graphics.hpp>
#include "Configs.h"
#include "UtilsMath.h"

//Setup shadow for an object. The shadow is a simple transparent circle below the object.
static inline void U_initShadow(const sf::Sprite& sprite, sf::CircleShape& shadow)
{
	float radius = sprite.getGlobalBounds().width * G_SHADOW_RADIUS_SCALER;
	shadow = sf::CircleShape(radius, 20);
	shadow.setScale(1, G_SHADOW_HEIGHT_SCALER);
	shadow.setOrigin(radius, radius);
	shadow.setFillColor(G_SHADOW_COLOR);
}

//Compute the scale of the shadow to give show character's distance from the ground while it is jumping or flying
static inline void U_updateShadowScaleWhileJumping(sf::CircleShape& shadow, const float z, const float jumpHeight)
{
	const float jumpRatio = abs(z) / jumpHeight;
	const float shadowScale = U_Lerp(1.f, 0.5f, jumpRatio);
	shadow.setScale(shadowScale * Vec2f(1.0f, G_SHADOW_HEIGHT_SCALER));
}

//Init sprite parameters
static inline void U_initSprite(sf::Sprite& sprite, const sf::Texture& texture, const float originX, const float originY)
{
	sprite.setTexture(texture);
	sprite.setOrigin(originX, originY);
}

//Init text parameters
static inline void U_initText(sf::Text& text, const sf::Font& font, const int fontSize, uint32_t style, const sf::Color& fillColor)
{
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setStyle(style);
	text.setFillColor(fillColor);
}

//Init text parameters
static inline void U_initText(sf::Text& text, sf::String str, const sf::Font& font, const int fontSize, uint32_t style, const sf::Color& fillColor)
{
	U_initText(text, font, fontSize, style, fillColor);
	text.setString(str);
}

//Init text parameters
static inline void U_initText(sf::Text& text, const sf::Font& font, const int fontSize, uint32_t style, const sf::Color& fillColor, const sf::Color& outlineColor, const float outlineThickness)
{
	U_initText(text, font, fontSize, style, fillColor);
	text.setOutlineColor(outlineColor);
	text.setOutlineThickness(outlineThickness);
}

//Init text parameters
static inline void U_initText(sf::Text& text, sf::String str, const sf::Font& font, const int fontSize, uint32_t style, const sf::Color& fillColor, const sf::Color& outlineColor, const float outlineThickness)
{
	U_initText(text, font, fontSize, style, fillColor, outlineColor, outlineThickness);
	text.setString(str);
}

//Set the position of sf::Drawable and draw it!
#define U_drawObject(object, pos, window) { object.setPosition(pos); window.draw(object); }
#define U_drawObjectXY(object, x, y, window) U_drawObject(object, Vec2f(x, y), window)

//Draw some text on the screen
static inline void U_drawText(sf::RenderWindow& window, sf::Text& text, const Vec2f& pos)
{
	sf::FloatRect textRect = text.getGlobalBounds();
	text.setOrigin(textRect.width / 2, textRect.height / 2);
	text.setPosition(pos.x, pos.y);
	window.draw(text);
}

static inline void U_drawText(sf::RenderWindow& window, sf::Text& text, const float x, const float y)
{
	U_drawText(window, text, Vec2f(x, y));
}