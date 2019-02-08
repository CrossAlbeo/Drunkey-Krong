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
	bool areCollided(std::shared_ptr<Entity> entity1, EntityType entity2);
	sf::Vector2f collideBlock(std::shared_ptr<Entity> player, std::shared_ptr<Entity> block);
	sf::Vector2f collideEchelle(std::shared_ptr<Entity> player, std::shared_ptr<Entity> echelle);
	sf::Vector2f collideBillBall(std::shared_ptr<Entity> player, std::shared_ptr<Entity> billBall);
};

#endif 