#include "EnemyRoseGoblin.h"
#include "Application.h"
#include "MTextures.h"
#include "MRender.h"
#include "MInput.h"
#include "MCollision.h"
#include "MParticle.h"
#include "Particle.h"
#include "MScene.h"
#include <time.h>
#include <stdlib.h>
#include "MAudio.h"


using namespace std;

EnemyRoseGoblin::EnemyRoseGoblin(infoGameObject info)
{
	
	type = ENEMY_ROSEGOBLIN;

	position = info.position;

	lifetime.start();


	walking = 1;
	flying = 3;

	status = ENEMY_IDLE_LEFT;

	//idle left
	idleLeft.frames.push_back({ 14, 10, 126, 125 });
	idleLeft.speed = 0.2f;

	//walking left
	walkingLeft.frames.push_back({ 160, 10, 135, 125});
	walkingLeft.frames.push_back({ 310, 5, 139, 130});
	walkingLeft.frames.push_back({ 460, 5, 135, 130});
	walkingLeft.speed = 0.25f;

	//walking right
	walkingRight.frames.push_back({ 755, 10, 135, 125 });
	walkingRight.frames.push_back({ 900, 5, 139, 130 });
	walkingRight.frames.push_back({ 1054, 5, 135, 130 });
	walkingRight.speed = 0.25f;

	//jumping left
	jumpingLeft.frames.push_back({24,300,107,140});
	jumpingLeft.frames.push_back({164,469,125,112});
	jumpingLeft.speed = 0.05f;

	//jumping right
	jumpingRight.frames.push_back({169,300,107,141});
	jumpingRight.frames.push_back({460,469,125,112});
	jumpingRight.speed = 0.05f;

	//jumping right

	//dying
	dying.frames.push_back({9,614,127,112});
	dying.frames.push_back({183,605,93,135});
	dying.frames.push_back({314,623,126,113});
	dying.frames.push_back({473,609,94,136});
	dying.frames.push_back({614,610,126,125});
	dying.frames.push_back({764,619,126,116});
	dying.frames.push_back({910,642,135,93});
	dying.loop = true;
	dying.speed = 0.2f;

	//snowball
	snowShotOne.frames.push_back({28, 70, 93, 80});
	snowShotOne.speed = 0.0f;

	snowShotTwo.frames.push_back({174, 37, 107, 113 });
	snowShotTwo.speed = 0.0f;

	snowShotThree.frames.push_back({ 319, 29, 116, 121 });
	snowShotThree.speed = 0.0f;

	snowShotFour.frames.push_back({469, 5, 116, 145 });
	snowShotFour.speed = 0.0f;

	//snowtrapped
	snowTrapped.frames.push_back({10,169,130,121});
	snowTrapped.frames.push_back({160,159,130,140});
	snowTrapped.speed = 0.2f;

	//snowRolling
	snowRolling.frames.push_back({19,5,116,145});
	snowRolling.frames.push_back({164,9,121,141});
	snowRolling.frames.push_back({314,5,117,145});
	snowRolling.frames.push_back({464,5,122,145});
	snowRolling.speed = 0.2f;


	shotCount = 0;
	rebound = 0;

	

	
}

EnemyRoseGoblin::~EnemyRoseGoblin()
{
	
}

bool EnemyRoseGoblin::Start()
{
	graphicsEnemyRoseGoblin = App->Ftextures->Load("roseGoblin.png");
	graphicsEnemyShotSnowball = App->Ftextures->Load("snowballfire.png");
	graphicsEnemyRollingSnowball = App->Ftextures->Load("snowball.png");

	reboundFx = App->Faudio->LoadFx("snowballrebound.ogg");
	dieFx = App->Faudio->LoadFx("enemydie.ogg");

	lifetime.start();
	startTime.start();
	return true;
}

update_status EnemyRoseGoblin::PreUpdate()
{
	if (startTime.getTicks() >= LEVELFROZENTIME)
	{

	
	while (!App->scene->pause)
	{
		GameObject *wherePLayerIs = App->FGameObject->returnPlayer();


		//is rolling
		if (status == ENEMY_ROLLING)
		{
			position.x += walking;
		}



		//state machine
		position.y += 2;

		if (status != ENEMY_TRAPPED && status != ENEMY_SNOWBALL && status != ENEMY_ROLLING && status != ENEMY_JUMPING_LEFT && status != ENEMY_JUMPING_RIGHT)
		{

			position.x += walking;
		}

		if (walking > 0 && status != ENEMY_TRAPPED && status != ENEMY_SNOWBALL && status != ENEMY_ROLLING && status != ENEMY_DYING && status != ENEMY_JUMPING_LEFT && status != ENEMY_JUMPING_RIGHT) //positive speed -> right
		{
			status = ENEMY_WALKING_RIGHT;
		}
		else if (walking < 0 && status != ENEMY_TRAPPED && status != ENEMY_SNOWBALL && status != ENEMY_ROLLING && status != ENEMY_DYING && status != ENEMY_JUMPING_LEFT && status != ENEMY_JUMPING_RIGHT) //negative speed -> left
		{
			status = ENEMY_WALKING_LEFT;
		}

		if (shotCount == 0 && status == ENEMY_TRAPPED)
		{
			if (walking > 0)
			{
				status = ENEMY_WALKING_RIGHT;
			}
			else
			{
				status = ENEMY_WALKING_LEFT;
			}

		}


		//jumping
		if (status == ENEMY_JUMPING_LEFT || status == ENEMY_JUMPING_RIGHT)
		{
			position.y -= 5;

			if (timeAnimationJumping.getTicks() > ENEMY_JUMPING_TIME)
			{
				//position.y -= ENEMY_COLLIDER_BODY_HEIGHT + 10;

				status = previousStatus;
				timeAnimationJumping.stop();

			}
		}


		if (timeAnimationBeingTrapped.isStarted() && timeAnimationBeingTrapped.getTicks() >= ENEMY_TRAPPED_TIME)
		{

			timeAnimationBeingTrapped.stop();

			if (shotCount > 0)
			{
				shotCount--;
			}

			timeAnimationBeingTrapped.start();

		}


		//gonna be rolling
		if (timerGonnaBeRolling.isStarted() && timerGonnaBeRolling.getTicks() > 100)
		{
			this->colliderBody->rolling = true;
		}

		//dying
		if (timeAnimationDying.isStarted() && timeAnimationDying.getTicks() >5000)
		{

			dead = true;
			colliderBody->to_delete = true;
			colliderFoot->to_delete = true;
			App->scene->nEnemiesLeft--;
		}

		if (timeAnimationDying.isStarted() && timeAnimationDying.getTicks() < 5000)
		{
			position.x -= flying;
			position.y -= flying;
		}

		if (rebound == 4)
		{
			//drop a random potion
			srand(lifetime.getTicks());
			int randomDropPotion = rand() % 4 + 1;
			if (randomDropPotion == 2)
			{
				infoGameObject inf;
				inf.position.x = this->position.x;
				inf.position.y = this->position.y;

				srand(lifetime.getTicks());
				int randomPotion = rand() % 3 + 1;

				switch (randomPotion)
				{
				case 1:
					App->FGameObject->createGameObject(inf, RED_POTION, COLLIDER_POTION_RED);
					break;

				case 2:
					App->FGameObject->createGameObject(inf, BLUE_POTION, COLLIDER_POTION_BLUE);
					break;

				case 3:
					App->FGameObject->createGameObject(inf, YELLOW_POTION, COLLIDER_POTION_YELLOW);
					break;
				default:
					break;
				}

			}

			GameObject *player = App->FGameObject->returnPlayer();
			player->stopRolling();
			dead = true;
			colliderBody->to_delete = true;
			colliderFoot->to_delete = true;
			App->scene->nEnemiesLeft--;

		}

		if (status != ENEMY_ROLLING && status != ENEMY_DYING)
		{
			switch (shotCount)
			{

			case 0:
				colliderBody->active = true;
				colliderBody->launchable = false;
				break;
			case 1:
			{
				status = ENEMY_TRAPPED;
				colliderBody->active = false;
				colliderBody->launchable = false;
			}

			case 2:
			{
				status = ENEMY_TRAPPED;
				colliderBody->active = false;
				colliderBody->launchable = false;
			}
			break;

			case 3:
			{
				status = ENEMY_TRAPPED;

				colliderBody->active = false;
				colliderBody->launchable = false;


			}

			break;

			case 4:
			{
				status = ENEMY_SNOWBALL;
				colliderBody->active = false;
				colliderBody->launchable = true;
			}

			break;

			default:
				break;
			}
		}

		return UPDATE_CONTINUE;
	}

	}

	
	return UPDATE_CONTINUE;

}

update_status EnemyRoseGoblin::Update()
{

	

		colliderBody->SetPos(position.x, position.y);
		colliderFoot->SetPos(position.x, (position.y + ENEMY_COLLIDER_BODY_HEIGHT));



		switch (status)
		{
		case ENEMY_IDLE_LEFT:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y, &(idleLeft.GetCurrentFrame()), 1.0f);
			break;

		case ENEMY_TRAPPED:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y, &(snowTrapped.GetCurrentFrame()), 1.0f);
			break;

		case ENEMY_WALKING_LEFT:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y, &(walkingLeft.GetCurrentFrame()), 1.0f);
			break;

		case ENEMY_WALKING_RIGHT:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y, &(walkingRight.GetCurrentFrame()), 1.0f);
			break;

		case ENEMY_DYING:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y, &(dying.GetCurrentFrame()), 1.0f);
			break;

		case ENEMY_ROLLING:
			App->renderer->Blit(graphicsEnemyRollingSnowball, position.x, position.y - 6, &(snowRolling.GetCurrentFrame()), 1.0f);
			break;
		case ENEMY_JUMPING_LEFT:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y - 6, &(jumpingLeft.GetCurrentFrame()), 1.0f);
			break;
		case ENEMY_JUMPING_RIGHT:
			App->renderer->Blit(graphicsEnemyRoseGoblin, position.x, position.y - 6, &(jumpingRight.GetCurrentFrame()), 1.0f);
			break;
		

		default:
			break;
		}

		if (status != ENEMY_ROLLING && status != ENEMY_DYING)
		{
			switch (shotCount)
			{
			case 1:
			{
				App->renderer->Blit(graphicsEnemyShotSnowball, position.x, position.y + 15, &(snowShotOne.GetCurrentFrame()), 1.0f);

			}
			break;
			case 2:
			{
				App->renderer->Blit(graphicsEnemyShotSnowball, position.x, position.y + 5, &(snowShotTwo.GetCurrentFrame()), 1.0f);

			}
			break;
			case 3:
			{
				App->renderer->Blit(graphicsEnemyShotSnowball, position.x, position.y, &(snowShotThree.GetCurrentFrame()), 1.0f);

			}
			break;
			case 4:
			{
				App->renderer->Blit(graphicsEnemyShotSnowball, position.x, position.y - 6, &(snowShotFour.GetCurrentFrame()), 1.0f);

			}
			break;

			default:
				break;
			}
		}


		return UPDATE_CONTINUE;
	
}

update_status EnemyRoseGoblin::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool const EnemyRoseGoblin::isDead()
{
	return dead;
}

void EnemyRoseGoblin::roll(DIRECTION dir)
{
	status = ENEMY_ROLLING;
	this->colliderBody->launchable = false;

	timerGonnaBeRolling.start();
	

	if (dir == LEFT)
	{
		walking = -5;
	}
	else if (dir == RIGHT)
	{
		walking = 5;
	}
	
}

void EnemyRoseGoblin::die()
{
	if (status != ENEMY_ROLLING && status != ENEMY_SNOWBALL && status != ENEMY_TRAPPED && status != ENEMY_DYING)
	{
		//drop a random potion
		srand(lifetime.getTicks());
		int randomDropPotion = rand() % 4 + 1;
		if (randomDropPotion == 2)
		{
			infoGameObject inf;
			inf.position.x = this->position.x;
			inf.position.y = this->position.y;

			srand(lifetime.getTicks());
			int randomPotion = rand() % 3 + 1;

			switch (randomPotion)
			{
			case 1:
				App->FGameObject->createGameObject(inf, RED_POTION, COLLIDER_POTION_RED);
				break;

			case 2:
				App->FGameObject->createGameObject(inf, BLUE_POTION, COLLIDER_POTION_BLUE);
				break;

			case 3:
				App->FGameObject->createGameObject(inf, YELLOW_POTION, COLLIDER_POTION_YELLOW);
				break;
			default:
				break;
			}

		}

		App->Faudio->PlayFx(dieFx);

		timeAnimationDying.start();
		status = ENEMY_DYING;
		colliderFoot->active = false;
		colliderBody->active = false;
	}
}


void EnemyRoseGoblin::OnCollision(Collider* c1, Collider* c2)
{

	switch (c2->type)
	{

	case COLLIDER_PLATFORM:
	{
		if (colliderFoot->active == true)
		{
			if (c1->rect.y <= c2->rect.y)
			{
				position.y -= 2;
			}
		}
		
	

	}
	break;

	case COLLIDER_WALL:
	{
		//borders
		
		walking = (walking * -1);

		if (status == ENEMY_ROLLING)
		{
			rebound++;
			App->Faudio->PlayFx(reboundFx);
		}

		if (status == ENEMY_DYING)
		{
			flying = (flying * -1);
		
		}

		
	}
	break;

	case COLLIDER_PLATFORM_BORDER_LEFT:
	{
		if (status == ENEMY_WALKING_LEFT)
		{
			GameObject *wherePLayerIs = App->FGameObject->returnPlayer();
			if (wherePLayerIs != nullptr)
			{
				if (wherePLayerIs->position.y < position.y)
				{
					srand(lifetime.getTicks());
					int randomNoJumpPlayerIsUp = rand() % 2 + 1;

					if (randomNoJumpPlayerIsUp == 2)//exist possibility that the enemy doesnt jump even if the player is down him
					{
						break;
					}

					if (c2->rect.x >= c1->rect.x)
					{
						previousStatus = status;
						status = ENEMY_JUMPING_LEFT;

						timeAnimationJumping.start();
						break;
					}
				}
			}
			
		}

	}
	break;

	case COLLIDER_PLATFORM_BORDER_LEFT_SPEC:
	{
		if (status == ENEMY_WALKING_LEFT)
		{

			GameObject *wherePLayerIs = App->FGameObject->returnPlayer();
			if (wherePLayerIs != nullptr)
			{
				if (wherePLayerIs->position.y < position.y)
				{
					srand(lifetime.getTicks());
					int randomNoJumpPlayerIsUp = rand() % 2 + 1;

					if (randomNoJumpPlayerIsUp == 2)//exist possibility that the enemy doesnt jump even if the player is down him
					{
						break;
					}

					if (c2->rect.x >= (c1->rect.x))
					{
						previousStatus = status;
						status = ENEMY_JUMPING_LEFT;

						timeAnimationJumping.start();
						break;
					}
				}
			}
		}
	}

	break;

	case COLLIDER_PLATFORM_BORDER_RIGHT:
	{
		if (status == ENEMY_WALKING_RIGHT)
		{

			GameObject *wherePLayerIs = App->FGameObject->returnPlayer();
			if (wherePLayerIs != nullptr)
			{
				if (wherePLayerIs->position.y < position.y)
				{
					srand(lifetime.getTicks());
					int randomNoJumpPlayerIsUp = rand() % 2 + 1;

					if (randomNoJumpPlayerIsUp == 2)//exist possibility that the enemy doesnt jump even if the player is down him
					{
						break;
					}

					if (c2->rect.x <= c1->rect.x)
					{
						previousStatus = status;
						status = ENEMY_JUMPING_RIGHT;

						timeAnimationJumping.start();
						break;
					}
				}
			}
		}
	}

	break;

	case COLLIDER_PLATFORM_BORDER_RIGHT_SPEC:
	{
		if (status == ENEMY_WALKING_RIGHT)
		{
			GameObject *wherePLayerIs = App->FGameObject->returnPlayer();
			if (wherePLayerIs != nullptr)
			{
				if (wherePLayerIs->position.y < position.y)
				{
					srand(lifetime.getTicks());
					int randomNoJumpPlayerIsUp = rand() % 2 + 1;

					if (randomNoJumpPlayerIsUp == 2)//exist possibility that the enemy doesnt jump even if the player is down him
					{
						break;
					}

					if (c2->rect.x <= c1->rect.x)
					{
						previousStatus = status;
						status = ENEMY_JUMPING_RIGHT;

						timeAnimationJumping.start();
						break;
					}
				}
			}
		}
	}

	break;


	case COLLIDER_ENEMY:
	{

		if (status == ENEMY_ROLLING)
		{
			GameObject *aux = App->FGameObject->Owner(c2);

			if (aux != nullptr)
			{
				

				c2->active = false;
				aux->die();

			}
			
		}

		if (!timerNotChangingDir.isStarted() && status != ENEMY_DYING && status != ENEMY_ROLLING && status != ENEMY_SNOWBALL && status != ENEMY_JUMPING_LEFT && status != ENEMY_JUMPING_RIGHT)
		{
			walking = (walking * -1);
			timerNotChangingDir.start();
		}

		if (timerNotChangingDir.getTicks() >2000)
		{
			timerNotChangingDir.stop();
		}

	}

	break;


	case COLLIDER_PLAYER_SOFT_SHOT:
	{
		

		if (shotCount < 4)
		{
			timeAnimationBeingTrapped.stop();
			shotCount++;
			timeAnimationBeingTrapped.start();
		}

		
	}
	break;

	case COLLIDER_PLAYER_HARD_SHOT:
	{


		if (shotCount < 4)
		{
			timeAnimationBeingTrapped.stop();
			shotCount+=2;
			if (shotCount > 4)
			{
				shotCount = 4;
			}
			timeAnimationBeingTrapped.start();
		}

	}
	break;
	default:
		break;
	}
}


bool EnemyRoseGoblin::CleanUp()
{
	App->Ftextures->Unload(graphicsEnemyRoseGoblin);
	App->Ftextures->Unload(graphicsEnemyShotSnowball);
	App->Ftextures->Unload(graphicsEnemyRollingSnowball);
	

	return true;
}
