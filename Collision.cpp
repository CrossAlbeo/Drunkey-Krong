#include "pch.h"
#include "Collision.h"

sf::FloatRect firstBox;
sf::FloatRect secondBox;

Collision::Collision() {
	firstBox = sf::FloatRect();
	secondBox = sf::FloatRect();
}

bool Collision::areCollided(std::shared_ptr<Entity> entity1, EntityType otherEntityType)
{
	sf::Sprite sprite1 = entity1->m_sprite;
	firstBox = sprite1.getGlobalBounds();
	for (std::shared_ptr<Entity> otherEntity : EntityManager::m_Entities)
	{
		if (otherEntity->m_type == otherEntityType)
		{
			secondBox = otherEntity->m_sprite.getGlobalBounds();
			if (firstBox.intersects(secondBox))
			{
				return true;
			}
		}
	}
	return false;
}

//TODO
sf::Vector2f collideBlock(std::shared_ptr<Entity> player, std::shared_ptr<Entity> block)
{
	return player->m_position;
}

//NONEED
bool collideEchelle(std::shared_ptr<Entity> player, std::shared_ptr<Entity> echelle)
{
	return true;
}
//TODO GAMEOVER
sf::Vector2f collideBillBall(std::shared_ptr<Entity> player, std::shared_ptr<Entity> billBall)
{
	return player->m_position;
}