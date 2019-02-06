#include "pch.h"
#include "Collision.h"

sf::FloatRect firstBox;
sf::FloatRect secondBox;

Collision::Collision() {
	firstBox = sf::FloatRect();
	secondBox = sf::FloatRect();
}

bool Collision::areCollided(sf::Sprite &sprite1, sf::Sprite &sprite2)
{
	firstBox = sf::FloatRect(sprite1.getPosition(), sprite1.getScale());
	secondBox = sf::FloatRect(sprite2.getPosition(), sprite2.getScale());
	return firstBox.intersects(secondBox);
}