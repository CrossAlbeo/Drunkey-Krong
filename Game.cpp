#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "Collision.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);
Collision collision = Collision();
const int lPxSheet = 24;

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Drunkey Krong", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
	, mIsJumping(false)
	, mLastState(true)
	, mCanClimb(false)
{
	mWindow.setFramerateLimit(160);

	// Draw blocks

	_TextureBlock.loadFromFile("Media/Textures/Block.png");
	_sizeBlock = _TextureBlock.getSize();

	for (int i = 0; i < BLOCK_COUNT_X; i++)
	{
		for (int j = 0; j < BLOCK_COUNT_Y; j++)
		{
			_Block[i][j].setTexture(_TextureBlock);
			_Block[i][j].setPosition(100.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));

			std::shared_ptr<Entity> se = std::make_shared<Entity>();
			se->m_sprite = _Block[i][j];
			se->m_type = EntityType::block;
			se->m_size = _TextureBlock.getSize();
			se->m_position = _Block[i][j].getPosition();
			EntityManager::m_Entities.push_back(se);
		}
	}

	// Draw Echelles

	_TextureEchelle.loadFromFile("Media/Textures/Echelle.png");

	for (int i = 0; i < ECHELLE_COUNT; i++)
	{
		_Echelle[i].setTexture(_TextureEchelle);
		_Echelle[i].setPosition(100.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y );

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Echelle[i];
		se->m_type = EntityType::echelle;
		se->m_size = _TextureEchelle.getSize();
		se->m_position = _Echelle[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	// Draw Luigi

	mTexture.loadFromFile("Media/Textures/luigi_spritesheet.png");
	//mTexture.loadFromFile("Media/Textures/Mario_small_transparent.png");
	sf::IntRect rectSourceSprite;

	rectSourceSprite.height = lPxSheet;
	rectSourceSprite.width = lPxSheet;
	rectSourceSprite.left = 4 * lPxSheet;
	rectSourceSprite.top = 0 * lPxSheet;

	_sizeLugi = mTexture.getSize();
	mPlayer.setTexture(mTexture);
	mPlayer.setTextureRect(rectSourceSprite);
	mPlayer.setScale(2, 2);
	sf::Vector2f posLuigi;
	posLuigi.x = 100.f + 70.f;
	posLuigi.y = BLOCK_SPACE * 5 - _sizeLugi.y;

	mPlayer.setPosition(posLuigi);

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{

	sf::Vector2f movement(0.f, 0.f);
	//TODO Replace With Jump And Gravity

	if (mIsJumping)
	{
		movement.y -= mJumpState;
		mJumpState -= 25.;
		if (mJumpState < -400.)
			mJumpState = -400.;
	}
		
	
	if (mIsCrouching)
	{
		mIsJumping = false;
		movement.y = 0;
	}
	if (mIsMovingLeft)
	{
		movement.x -= PlayerSpeed;
	}
	if (mIsClimbingUp)
	{
		movement.y -= PlayerSpeed;
	}
	if (mIsClimbingDown)
	{
		movement.y += PlayerSpeed;
	}
	if (mIsMovingRight)
	{
		movement.x += PlayerSpeed;
	}

	

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::player)
		{
			continue;
		}
		else 
		{
			//Collisions here
			std::shared_ptr<Entity> collidedBlock = collision.areCollided(entity, block);
			if (collidedBlock!=NULL && mIsJumping)
			{
				mIsJumping = collision.collideBlock(entity, collidedBlock);
				if (!mIsJumping)
				{
					//entity->m_position.y = collidedBlock->m_position.y - collidedBlock->m_size.y - 10.0;
					movement.y = -50.0;
				}
			}
			if (collision.areCollided(entity, echelle) != NULL)
			{
				//TODO State Climbing
				mCanClimb = true;
			}
			else mCanClimb = false;
			if (collision.areCollided(entity, billBall) != NULL)
			{
				//TODO GameOver
			}
			if (collision.areCollided(entity, waluigi) != NULL)
			{
				//TODO Win
			}
		}

		entity->m_sprite.move(movement * elapsedTime.asSeconds());
	}
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_enabled == false)
			{
				continue;
			}

			if (entity->m_type != EntityType::player)
			{
				continue;
			}

			mStatisticsText.setString(
				"Drunkey Krong is Krongy enough\nFrames / Second = " + toString(mStatisticsNumFrames) + "\n" +
				"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us\n" +
				"Position x = " + toString(entity->m_sprite.getPosition().x) + "\t y = " + toString(entity->m_sprite.getPosition().y) + "\n" +
				"jumpstate" + toString(mJumpState)
			);
		}
		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}

	//
	// Handle collision
	//

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		// Handle collision weapon enemies
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::Up && mIsJumping == false)
	{
		mIsJumping = true;
		mJumpState = 400.f;
	}

	else if (key == sf::Keyboard::Up && mCanClimb)
		mIsClimbingUp = true;
		
	else if (key == sf::Keyboard::Down)
		mIsCrouching = isPressed;
	else if (key == sf::Keyboard::Down && mCanClimb)
		mIsClimbingDown = true;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;

	if (key == sf::Keyboard::Space)
	{
		mIsJumping = isPressed;
	}
}
