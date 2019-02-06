#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include "pch.h"
#include <SFML/Graphics.hpp>

class Collision
{

public:
	Collision();

private:
	sf::FloatRect firstBox;
	sf::FloatRect secondBox;

public:
	bool areCollided(sf::Sprite &sprite1, sf::Sprite &sprite2);
};

#endif 