#include "Potion.h"
#include "Application.h"
#include "MTextures.h"
#include "MRender.h"
#include "MInput.h"
#include "MCollision.h"
#include "MParticle.h"
#include "Particle.h"
#include "Timer.h"
#include "MScene.h"
#include "MAudio.h"

Potion::Potion(infoGameObject inf)
{
	//Initialization
	

	position = inf.position;
	lifeTime = 5000;

	//animation

	//red potion
	redPotion.frames.push_back({14,5,39,55});
	redPotion.frames.push_back({78,5,39,55});
	redPotion.speed = 0.2f;

	//blue potion
	bluePotion.frames.push_back({ 14, 69, 39, 55 });
	bluePotion.frames.push_back({ 78, 68, 39, 55 });
	bluePotion.speed = 0.2f;

	//yellow potion
	yellowPotion.frames.push_back({ 134, 5, 39, 55 });
	yellowPotion.frames.push_back({ 198, 5, 39, 55 });
	yellowPotion.speed = 0.2f;



}

Potion::~Potion()
{

}

bool Potion::Start()
{
	graphicsPotion = App->Ftextures->Load("potions.png");


	return true;
}

update_status Potion::PreUpdate()
{
	
	if (start.isStarted() && start.getTicks() >= lifeTime)
	{
		dead = true;
		colliderBody->to_delete = true;
	}

	return UPDATE_CONTINUE;

}

update_status Potion::Update()
{

	switch (type)
	{
	case RED_POTION:
		App->renderer->Blit(graphicsPotion, position.x, position.y, &(redPotion.GetCurrentFrame()), 1.0f);
		break;
		case BLUE_POTION:
			App->renderer->Blit(graphicsPotion, position.x, position.y, &(bluePotion.GetCurrentFrame()), 1.0f);
		break;
	case YELLOW_POTION:
		App->renderer->Blit(graphicsPotion, position.x, position.y, &(yellowPotion.GetCurrentFrame()), 1.0f);
		break;
	default:
		break;
	}

	
		
	return UPDATE_CONTINUE;

}


bool const Potion::isDead()
{
	return dead;
}



bool Potion::CleanUp()
{
	App->Ftextures->Unload(graphicsPotion);
	

	return true;
}
