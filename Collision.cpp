#include "pch.h"
#include "Collision.h"

sf::FloatRect firstBox;
sf::FloatRect secondBox;

Collision::Collision() {
	firstBox = sf::FloatRect();
	secondBox = sf::FloatRect();
}

std::shared_ptr<Entity> Collision::areCollided(std::shared_ptr<Entity> entity1, EntityType otherEntityType)
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
				return otherEntity;
			}
		}
	}
	
	return NULL;
}

//Change Jumping State if player is above the block during the collision
bool Collision::collideBlock(std::shared_ptr<Entity> player, std::shared_ptr<Entity> block)
{
	if (player->m_position.y <= block->m_position.y - block->m_size.y)
	{
		return false;
	}
	else return true;
}