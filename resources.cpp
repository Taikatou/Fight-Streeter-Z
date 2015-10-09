
#include "resources.h"
#include "Iw2D.h"


Resources::Resources()
{
	PlayButton		=		Iw2DCreateImage("textures/play.png");
	// Load fonts

	PauseIcon		=		Iw2DCreateImage("textures/pause_icon.png");
	Font			=		Iw2DCreateFont("fonts/arial8.gxfont");
	NewGame			=		Iw2DCreateImage("textures/new_game.png");
	MenuBG			=		Iw2DCreateImage("textures/menu_bkg.jpg");
	MenuButton		=		Iw2DCreateImage("textures/button_bg.png");
	FloorImage		=		Iw2DCreateImage("textures/floor.png");
	settingsButtonImage = Iw2DCreateImage("textures/settings-button.png");
}


void Resources::loadGame()
{
	greenImage = Iw2DCreateImage("textures/green.png");
	playerSprite = Iw2DCreateImage("textures/settings-button.png");
	aiSprite = Iw2DCreateImage("textures/settings-button.png");
	buttonA_Image = Iw2DCreateImage("textures/a_button.png");
	buttonB_Image = Iw2DCreateImage("textures/b_button.png");
	buttonC_Image = Iw2DCreateImage("textures/x_button.png");
	buttonD_Image = Iw2DCreateImage("textures/y_button.png");
	// to get funtionality to load sprites

	aiBlockSprite = Iw2DCreateImage("textures/settings-button.png");
	playerBlockSprite = Iw2DCreateImage("textures/settings-button.png");

	Explosion = Iw2DCreateImage("textures/explosion.png");
	int frame_w = /*(int)(Explosion->GetWidth() / 8)*/ 256;
	int frame_h = /*(int)(Explosion->GetHeight() / 6)*/ 256;
	ExplosionAtlas = new CAtlas(frame_w, frame_h, 48, Explosion);
}


Resources::~Resources()
{

	delete MenuBG;
	delete MenuButton;
	delete PauseIcon;
	delete PlayButton;
	delete Font;
	delete NewGame;
	delete settingsButtonImage;
	delete FloorImage;
	delete greenImage;
	delete playerSprite;
	delete aiSprite;
	delete buttonA_Image;
	delete buttonB_Image;
	delete buttonC_Image;
	delete buttonD_Image;
	delete aiBlockSprite;
	delete playerBlockSprite;

	delete Explosion;
	delete ExplosionAtlas;

}

// Global resources
Resources* g_pResources = 0;



