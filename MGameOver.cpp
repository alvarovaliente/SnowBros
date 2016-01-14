#include "Globals.h"
#include "Application.h"
#include "MCollision.h"
#include "MRender.h"
#include "MTextures.h"
#include "MGameOver.h"
#include "MGameObject.h"
#include "Player.h"
#include "MInput.h"
#include "MScene.h"
#include "MIntro.h"
#include "MAudio.h"


MGameOver::MGameOver(bool start_enabled) : Module(start_enabled)
{
	background = NULL;
}

MGameOver::~MGameOver()
{

}

// Load assets
bool MGameOver::Start()
{
	gameOverFx = App->Faudio->LoadFx("gameover.ogg");
	background = App->Ftextures->Load("gameover.png");
	

	App->Faudio->PlayFx(gameOverFx);

	return true;
}

// UnLoad assets
bool MGameOver::CleanUp()
{

	App->Ftextures->Unload(background);

	return true;
}

// Update: draw background
update_status MGameOver::Update()
{

	App->renderer->Blit(background, 0, 0, NULL);

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		this->Disable();
		App->intro->Enable();
	}


	return UPDATE_CONTINUE;
}