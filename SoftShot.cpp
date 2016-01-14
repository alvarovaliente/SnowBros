
#include "SoftShot.h"
#include "Globals.h"
#include "Particle.h"
#include "Application.h"
#include "MRender.h"
#include "MCollision.h"


SoftShot::SoftShot(infoParticle info) : Particle( info)
{
	anim = info.anim;
	explosion = info.explo;
	texture = info.text;
	time.start();
	info.speed.x = speed.x;
	speed.y = 0.2f;
	lifetime = 500;
	timeGoingUp.start();
}

SoftShot::~SoftShot()
{

}

bool SoftShot::Start()
{
	
	return true;
}

update_status SoftShot::Update()
{

	
	
	if (time.getTicks() >= lifetime)
	{
		App->renderer->Blit(texture, position.x, position.y, &(explosion.GetCurrentFrame()));
		
	dead = true;
	collider->to_delete = true;
	}



	if (!dead)
	{
		collider->SetPos(position.x, position.y);

		if (timeGoingUp.isStarted() && timeGoingUp.getTicks() <= 245)
		{
			position.x += speed.x;
			position.y -= speed.y;
		}

		if (timeGoingUp.isStarted() && timeGoingUp.getTicks() >= 245)
		{
			timeGoingUp.stop();
			timeGoingDown.start();
		}

		if (timeGoingDown.isStarted() && timeGoingDown.getTicks() <= 245)
		{
			position.x += speed.x;
			position.y += speed.y + 2.0f;
		}

		if (timeGoingDown.isStarted() && timeGoingDown.getTicks() >= 245)
		{
			timeGoingDown.stop();
		}
		

	

		App->renderer->Blit(texture, position.x, position.y, &(anim.GetCurrentFrame()));

		
	}

	return UPDATE_CONTINUE;
}

bool const SoftShot::isDead()
{
	return this->dead;
}

void SoftShot::OnCollision(Collider* c1, Collider* c2)
{
	switch (c2->type)
	{
	case COLLIDER_WALL:
	{
		App->renderer->Blit(texture, position.x, position.y, &(explosion.GetCurrentFrame()));
		dead = true;
		collider->to_delete = true;
		//timeToDie.start();
	}

	case COLLIDER_ENEMY:
	{
		App->renderer->Blit(texture, position.x, position.y, &(explosion.GetCurrentFrame()));

		//timeToDie.start();
		dead = true;
		collider->to_delete = true;
	}
	break;
	default:
		break;
	}
}
