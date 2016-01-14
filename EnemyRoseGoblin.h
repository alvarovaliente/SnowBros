#ifndef __ENEMYROSEGOBLIN_H__
#define __ENEMYROSEGOBLIN_H__

#include "GameObject.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Globals.h"
#include "MGameObject.h"
#include "Timer.h"




class Collider;

class EnemyRoseGoblin : public GameObject
{
public:

	//methods
	EnemyRoseGoblin(infoGameObject info);
	~EnemyRoseGoblin();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	void roll(DIRECTION dir);
	void die();
	bool CleanUp();
	bool const isDead();
	void OnCollision(Collider* c1, Collider* c2);

	//graphics
	SDL_Texture *graphicsEnemyRoseGoblin = nullptr;
	SDL_Texture *graphicsEnemyShotSnowball = nullptr;
	SDL_Texture *graphicsEnemyRollingSnowball = nullptr;

	//animation
	Animation idleRight;
	Animation idleLeft;
	Animation walkingLeft;
	Animation walkingRight;
	Animation jumpingLeft;
	Animation jumpingRight;
	Animation dying;
	Animation snowTrapped;
	Animation snowShotOne;
	Animation snowShotTwo;
	Animation snowShotThree;
	Animation snowShotFour;
	Animation snowRolling;

	//variables
	fPoint speed;
	ENEMY_STATUS status;
	ENEMY_STATUS previousStatus;
	int shotCount;
	int walking;
	int flying;
	int rebound;

	//timers
	Timer timeAnimationBeingTrapped;
	Timer timeAnimationDying;
	Timer timeAnimationJumping;
	Timer timerNotChangingDir;
	Timer timerGonnaBeRolling;
	Timer lifetime;
	Timer startTime;

	//sounds
	unsigned int reboundFx;
	unsigned int dieFx;
	
	
	


};


#endif // __ENEMYROSEGOBLIN_H__