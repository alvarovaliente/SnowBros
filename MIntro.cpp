#include "Globals.h"
#include "Application.h"
#include "MCollision.h"
#include "MRender.h"
#include "MTextures.h"
#include "MIntro.h"
#include "MGameObject.h"
#include "Player.h"
#include "MInput.h"
#include "MScene.h"
#include "Timer.h"
#include "MAudio.h"

MIntro::MIntro(bool start_enabled) : Module(start_enabled)
{
	background = NULL;


}

MIntro::~MIntro()
{

}

// Load assets
bool MIntro::Start()
{
	background = App->Ftextures->Load("intro.png");
	start = App->Ftextures->Load("start.png");

	introFx = App->Faudio->LoadFx("init.ogg");
	startFx = App->Faudio->LoadFx("start.ogg");

	App->Faudio->PlayFx(introFx);

	return true;
}

// UnLoad assets
bool MIntro::CleanUp()
{

	App->Ftextures->Unload(background);
	App->Ftextures->Unload(start);

	return true;
}

// Update: draw background
update_status MIntro::Update()
{
	
	if (!timeStart.isStarted())
	{
		App->renderer->Blit(background, 0, 0, NULL);
	}
	else
	{
		App->renderer->Blit(start, 0, 0, NULL);
	}
	
	
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !timeStart.isStarted())
	{
		App->renderer->Blit(start, 0, 0, NULL);
		timeStart.start();
		
	}

	if (timeStart.isStarted() && timeStart.getTicks() > 1000)
	{

		App->Faudio->PlayFx(startFx);
		this->Disable();

		App->FGameObject->Enable();
		App->FCollision->Enable();
		App->scene->Enable();
		timeStart.stop();
	}
		

	return UPDATE_CONTINUE;
}