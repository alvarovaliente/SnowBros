#ifndef __SOFTSHOT_H__
#define __SOFTSHOT_H__


#include "Globals.h"
#include "Particle.h"
#include "Animation.h"

class Collider;

class SoftShot : public Particle
{

public:

	SoftShot(infoParticle info);
	~SoftShot();

	bool Start();
	update_status Update();
	bool const isDead();
	void OnCollision(Collider* c1, Collider* c2);

	Animation anim;
	Animation explosion;
	SDL_Texture *texture;

	Timer timeToDie;
	Timer timeGoingUp;
	Timer timeGoingDown;


};

#endif // __SOFTSHOT_H__