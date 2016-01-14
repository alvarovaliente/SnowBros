#ifndef __HARDSHOT_H__
#define __HARDSHOT_H__


#include "Globals.h"
#include "Particle.h"
#include "Animation.h"

class Collider;

class HardShot : public Particle
{

public:

	HardShot(infoParticle info);
	~HardShot();

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

#endif // __HARDSHOT_H__