#ifndef __SOFTSHOT_H__
#define __SOFTSHOT_H__


#include "Globals.h"
#include "Particle.h"
#include "Animation.h"

class Collider;

class SoftShot : public Particle
{

public:

	//methods
	SoftShot(infoParticle info);
	~SoftShot();

	bool Start();
	update_status Update();
	bool const isDead();
	void OnCollision(Collider* c1, Collider* c2);

	//animation
	Animation anim;
	Animation explosion;

	//graphics
	SDL_Texture *texture;

	//timers
	Timer timeToDie;
	Timer timeGoingUp;
	Timer timeGoingDown;


};

#endif // __SOFTSHOT_H__