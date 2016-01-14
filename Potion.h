#ifndef __POTION_H__
#define __POTION_H__

#include "GameObject.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Globals.h"
#include "MGameObject.h"
#include "Timer.h"


class Collider;

class Potion : public GameObject
{
public:

	//Methods
	Potion(infoGameObject inf);
	~Potion();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool const isDead();
	bool CleanUp();



	//Textures
	SDL_Texture *graphicsPotion = nullptr;


	//Animations
	Animation redPotion;
	Animation bluePotion;
	Animation yellowPotion;
	//Timers
	int lifeTime;
	Timer start;



};


#endif // __POTION_H__