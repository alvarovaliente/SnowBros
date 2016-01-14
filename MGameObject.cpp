#include "Globals.h"
#include "Application.h"
#include "MGameObject.h"
#include "GameObject.h"
#include "Player.h"
#include "EnemyRoseGoblin.h"
#include "EnemyYellowMonkey.h"
#include "Collider.h"
#include "MCollision.h"
#include "Potion.h"



MGameObject::MGameObject(){

}

MGameObject::~MGameObject()
{

}

bool MGameObject::Start()
{

	for (auto& it : gameObjects)
	{
		if (it != nullptr)
		{
			it->Start();
		}

	}

	return true;
}


update_status MGameObject::PreUpdate()
{

	for (auto& it : gameObjects)
	{
		if (it != nullptr)
		{
			it->PreUpdate();
		}

	}

	return UPDATE_CONTINUE;
}

update_status MGameObject::Update()
{
	for (auto& it : gameObjects)
	{

		if (it != nullptr)
		{
			if (it->isDead())
			{

				RELEASE(it);
			}
		}


	}

	for (auto& it : gameObjects)
	{
		if (it != nullptr)
		{
			it->Update();
		}

	}

	return UPDATE_CONTINUE;
}

update_status MGameObject::PostUpdate()
{

	for (auto& it : gameObjects)
	{
		if (it != nullptr)
		{
			it->PostUpdate();
		}

	}

	return UPDATE_CONTINUE;
}


bool MGameObject::CleanUp()
{

	for (list<GameObject*>::reverse_iterator it = gameObjects.rbegin(); it != gameObjects.rend(); ++it)
		RELEASE(*it);

	gameObjects.clear();

	return true;
}

void MGameObject::OnCollision(Collider* c1, Collider* c2)
{
	for (auto& it : gameObjects)
	{
		if (it != nullptr)
		{

			if ((it)->colliderBody != nullptr)
			{
				if ((it)->colliderBody->to_delete != true)
				{
					if ((it)->colliderBody == c1)
					{
						it->OnCollision(c1, c2);
					}

				}
			}
		
			if ((it)->colliderFoot != nullptr)
			{
				if ((it)->colliderFoot->to_delete != true)
				{
					if ((it)->colliderFoot == c1)
					{
						it->OnCollision(c1, c2);
					}
				}
				
			}
			
		}	
		
	}
}

GameObject* MGameObject::Owner(Collider const *collider)
{
	for (list<GameObject*>::reverse_iterator it = gameObjects.rbegin(); it != gameObjects.rend(); ++it)
	{

			if (*it != nullptr)
			{
				if (!(*it)->isDead())
				{
					if ((*it)->colliderBody == collider)
					{
						return *it;
					}
				}
			}
		}

		
}

GameObject* MGameObject::returnPlayer()
{
	for (list<GameObject*>::reverse_iterator it = gameObjects.rbegin(); it != gameObjects.rend(); ++it)
	{	
		if (*it != nullptr)
		{
			if (!(*it)->isDead())
			{
				if ((*it)->type == PLAYER)
				{
					return *it;
				}
			}
			
		}
	}
}

void MGameObject::createGameObject(infoGameObject info, GAMEOBJECT_TYPE type, COLLIDER_TYPE collider_type)
{

	switch (type)
	{
	case PLAYER:
	{
		Player* player = new Player(info);

		if (collider_type != COLLIDER_NONE)
		{
			
			player->colliderBody = App->FCollision->AddCollider({ player->position.x, player->position.y, PLAYER_COLLIDER_BODY_WIDTH,PLAYER_COLLIDER_BODY_HEIGHT}, COLLIDER_PLAYER, this);
			player->colliderFoot = App->FCollision->AddCollider({ player->position.x, PLAYER_COLLIDER_BODY_HEIGHT, PLAYER_COLLIDER_FOOT_WIDTH, PLAYER_COLLIDER_FOOT_HEIGHT }, COLLIDER_FOOT, this);
		}

		player->Start();

		gameObjects.push_back(player);
	}

		break;
	case ENEMY_ROSEGOBLIN:
	{

		EnemyRoseGoblin* roseGoblin = new EnemyRoseGoblin(info);

		if (collider_type != COLLIDER_NONE)
		{
			roseGoblin->colliderBody = App->FCollision->AddCollider({ roseGoblin->position.x, roseGoblin->position.y, ENEMY_COLLIDER_BODY_WIDTH, ENEMY_COLLIDER_BODY_HEIGHT }, COLLIDER_ENEMY, this);
			roseGoblin->colliderFoot = App->FCollision->AddCollider({ roseGoblin->position.x, ENEMY_COLLIDER_BODY_HEIGHT, ENEMY_COLLIDER_FOOT_WIDTH, ENEMY_COLLIDER_FOOT_HEIGHT }, COLLIDER_FOOT, this);
		}

		roseGoblin->Start();

		gameObjects.push_back(roseGoblin);
	}
		break;

	case ENEMY_YELLOWMONKEY:
	{

		EnemyYellowMonkey* yellowMonkey = new EnemyYellowMonkey(info);

		if (collider_type != COLLIDER_NONE)
		{
			yellowMonkey->colliderBody = App->FCollision->AddCollider({ yellowMonkey->position.x, yellowMonkey->position.y, ENEMY_COLLIDER_BODY_WIDTH, ENEMY_COLLIDER_BODY_HEIGHT }, COLLIDER_ENEMY, this);
			yellowMonkey->colliderFoot = App->FCollision->AddCollider({ yellowMonkey->position.x, ENEMY_COLLIDER_BODY_HEIGHT, ENEMY_COLLIDER_FOOT_WIDTH, ENEMY_COLLIDER_FOOT_HEIGHT }, COLLIDER_FOOT, this);
		}

		yellowMonkey->Start();

		gameObjects.push_back(yellowMonkey);
	}
		break;

	case RED_POTION:
	{
		Potion *potion = new Potion(info);
		potion->type = RED_POTION;

		if (collider_type != COLLIDER_NONE)
		{
			potion->colliderBody = App->FCollision->AddCollider({ potion->position.x, potion->position.y, POTION_COLLIDER_BODY_WIDTH, POTION_COLLIDER_BODY_HEIGHT }, COLLIDER_POTION_RED, this);
			potion->colliderFoot = nullptr;
		}

		potion->Start();

		gameObjects.push_back(potion);
	}
	break;

	case BLUE_POTION:
	{

		Potion *potion = new Potion(info);
		potion->type = BLUE_POTION;

		if (collider_type != COLLIDER_NONE)
		{
			potion->colliderBody = App->FCollision->AddCollider({ potion->position.x, potion->position.y, POTION_COLLIDER_BODY_WIDTH, POTION_COLLIDER_BODY_HEIGHT }, COLLIDER_POTION_BLUE, this);
			potion->colliderFoot = nullptr;
		}

		potion->Start();

		gameObjects.push_back(potion);
	}
	break;

	case YELLOW_POTION:
	{

		Potion *potion = new Potion(info);
		potion->type = YELLOW_POTION;

		if (collider_type != COLLIDER_NONE)
		{
			potion->colliderBody = App->FCollision->AddCollider({ potion->position.x, potion->position.y, POTION_COLLIDER_BODY_WIDTH, POTION_COLLIDER_BODY_HEIGHT }, COLLIDER_POTION_YELLOW, this);
			potion->colliderFoot = nullptr;
		}

		potion->Start();

		gameObjects.push_back(potion);
	}
	break;

	default:
		break;
	}
}
