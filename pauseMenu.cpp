
#include "PauseMenu.h"
#include "IwGx.h"
#include "input.h"
#include "resources.h"
#include "game.h"
#include "scene.h"

#define GRAPHIC_DESIGN_WIDTH    768

PauseMenu::~PauseMenu()
{
}

void PauseMenu::Update(float deltaTime, float alphaMul)
{
	if (!m_IsActive)
		return;

	Scene::Update(deltaTime, alphaMul);

	// Detect screen tap
	if (m_IsInputActive && m_Manager->GetCurrent() == this && !g_pInput->m_Touched && g_pInput->m_PrevTouched)
	{
		g_pInput->Reset();

		if (continueGameButton->HitTest(g_pInput->m_X, g_pInput->m_Y))
		{
			// Switch to game scene
			Game* game = (Game*)g_pSceneManager->Find("game");
			g_pSceneManager->SwitchTo(game);

			// Restore button alpha state
			/*PauseMenu* menu = (PauseMenu*)g_pSceneManager->Find("pausemenu");
			menu->getContinueGameButton()->m_Alpha = 1.0f;*/
		}
		else if (newGameButton->HitTest(g_pInput->m_X, g_pInput->m_Y))
		{
			// Switch to game scene
			Game* game = (Game*)g_pSceneManager->Find("game");
			game->RestartGame();
			g_pSceneManager->SwitchTo(game);
		}
	}
}

void PauseMenu::Render()
{
	Scene::Render();
}

void PauseMenu::Init()
{

	// Create menu background
	CSprite* background = new CSprite();
	background->m_X = (float)IwGxGetScreenWidth() / 2;
	background->m_Y = (float)IwGxGetScreenHeight() / 2;
	background->SetImage(g_pResources->getMenuBG());
	background->m_AnchorX = 0.5;
	background->m_AnchorY = 0.5;
	// Fit background to screen size
	background->m_ScaleX = (float)IwGxGetScreenWidth() / background->GetImage()->GetWidth();
	background->m_ScaleY = (float)IwGxGetScreenHeight() / background->GetImage()->GetHeight();
	AddChild(background);

	// Create Continue Game button


	continueGameButton = new CSprite();
	continueGameButton->m_X = (float)IwGxGetScreenWidth() / 2;
	continueGameButton->m_Y = (float)IwGxGetScreenHeight() / 3;
	continueGameButton->m_AnchorX = 0.5f;
	continueGameButton->m_AnchorY = 0.5f;
	continueGameButton->SetImage(g_pResources->getMenuButton());

	continueGameButton->m_ScaleX = (float)(IwGxGetScreenWidth() / 4) / continueGameButton->GetImage()->GetWidth();
	continueGameButton->m_ScaleY = continueGameButton->m_ScaleX;
	continueGameButton->m_Angle = 270.0f;
	AddChild(continueGameButton);


	// Create New  Game button
	newGameButton = new CSprite();
	newGameButton->SetImage(g_pResources->getNewButton());
	newGameButton->m_X = (float)IwGxGetScreenWidth() / 2;
	newGameButton->m_Y = (float)IwGxGetScreenHeight() - continueGameButton->m_Y;
	newGameButton->m_W = newGameButton->GetImage()->GetWidth();
	newGameButton->m_H = newGameButton->GetImage()->GetHeight();
	newGameButton->m_AnchorX = 0.5f;
	newGameButton->m_AnchorY = 0.5;
	newGameButton->m_ScaleX = (float)(IwGxGetScreenWidth() / 4) / newGameButton->GetImage()->GetWidth();
	newGameButton->m_ScaleY = newGameButton->m_ScaleX;
	newGameButton->m_Angle = 270.0f;
	AddChild(newGameButton);


}

