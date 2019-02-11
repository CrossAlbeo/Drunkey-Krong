#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include "pch.h"
#include "Entity.h"
#include "EntityManager.h"
#include <SFML/Graphics.hpp>

class Collision
{

public:
	Collision();

private:
	sf::FloatRect firstBox;
	sf::FloatRect secondBox;

public:
	std::shared_ptr<Entity> areCollided(std::shared_ptr<Entity> entity1, EntityType entity2);
	bool collideBlock(std::shared_ptr<Entity> player, std::shared_ptr<Entity> block);
};

#endif 