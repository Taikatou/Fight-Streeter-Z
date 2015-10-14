#include "IwGx.h"
#include "IwHashString.h"

#include "game.h"
#include "main.h"
#include "input.h"
#include "audio.h"
#include "resources.h"
#include "pauseMenu.h"
#include "s3eVibra.h"
#include "explosion.h"
#include "hadoken.h"
#include <cmath>

#define FONT_DESIGN_WIDTH 320
#define FONT_HEIGHT 15
#define DASH_LIMIT 5
#define NUM_BUTTONS 4
#define PLAYER_ONE 0
#define PLAYER_TWO 1

Game::~Game()
{
	if (buttons != 0)
	{
		delete[] buttons;
	}
	if (player != 0)
	{
		delete[] player;
	}
	if (lifeBars != 0)
	{
		delete[] lifeBars;
	}
	delete g_pExplosionManager;

}
// Pause game handler that is called when the user taps the pause sprite
void Game::pauseGame()
{
	// Pause timers
	m_Timers.Pause();

	// Store old game state and change to paused game state

	// Switch to pause menu scene
	g_pSceneManager->SwitchTo(g_pSceneManager->Find("pausemenu"));

	//  explosion.stopExplosions()
}

void Game::CheckResetAnimation(bool isAI)
{
	if (player[isAI]->getPrevActive())
	{
		player[isAI]->SetImage(g_pResources->getSettingsButton());
		player[isAI]->setPrevActive(false);
		player[isAI]->setAttacking(false);
		player[isAI]->setPrevMovement(-1);
	}
}

void Game::Update(float deltaTime, float alphaMul)
{
	if (!m_IsActive)
		return;
	if (m_Manager->GetCurrent() == this)
	{
		// Update base scene
		Scene::Update(deltaTime, alphaMul);

		if (!gameOver)
		{
			if (m_IsInputActive)
			{
				if (!DBZBlast)
				{
					NormalGameLoop();
				}
				else
				{
					DBZLoop();
				}
			}
		}

		else
		{
			if ((g_pInput->m_Touched && !g_pInput->m_PrevTouched) || (g_pInput->m_Touched && g_pInput->m_PrevTouched))
			{
				restartButtonHit = gameOverSprite->HitTest(g_pInput->m_X, g_pInput->m_Y);
			}
			else if (!g_pInput->m_Touched && g_pInput->m_PrevTouched)
			{
				if (restartButtonHit && gameOverSprite->HitTest(g_pInput->m_X, g_pInput->m_Y))
				{
					RestartGame();
				}
				g_pInput->Reset();
			}
		}
	}
}

void Game::DBZLoop()
{
	float toMove;
	if (!g_pInput->m_Touched && g_pInput->m_PrevTouched)
	{
		toMove = (float)(ScreenHeight / 100);
		g_pInput->Reset();
	}
	else
	{
		toMove = -((float)ScreenHeight / 1500);
	}

	OutputLine(toMove);
	projectiles[PLAYER_ONE].at(0)->m_Y += toMove;
	ResizeLine(projectileLines[PLAYER_ONE].at(0), PLAYER_ONE, projectiles[PLAYER_ONE].back()->m_Y);

	projectiles[PLAYER_TWO].at(0)->m_Y = projectiles[PLAYER_ONE].at(0)->m_Y;
	ResizeLine(projectileLines[PLAYER_TWO].at(0), PLAYER_TWO, projectiles[PLAYER_TWO].back()->m_Y);

	if(toMove > 0)
	{
		if (player[PLAYER_TWO]->HitTest(projectiles[PLAYER_TWO].at(0)->m_X + 20, projectiles[PLAYER_TWO].at(0)->m_Y + projectiles[PLAYER_TWO].at(0)->getWidth()))
		{
			DBZWin(PLAYER_TWO);
		}
	}
	else
	{
		if (player[PLAYER_ONE]->HitTest(projectiles[PLAYER_ONE].at(0)->m_X + 20, projectiles[PLAYER_ONE].at(0)->m_Y - projectiles[PLAYER_ONE].at(0)->getWidth()))
		{
			DBZWin(PLAYER_ONE);
		}
	}
}

void Game::DBZWin(bool isAI)
{
	g_pExplosionManager->Create(projectiles[isAI].at(0));
	RemoveProjectile(PLAYER_ONE, 0);
	RemoveProjectile(PLAYER_TWO, 0);
	DBZBlast = false;

	player[isAI]->subtractFromLife(45);
	CheckLife(isAI);
}

void Game::NormalGameLoop()
{
	//check if the player's charcter is moving (We do not allow any movement on top of animation)
	if (!(player[PLAYER_ONE]->getAnimActiv()))
	{
		CheckResetAnimation(PLAYER_ONE);
		//check if there was previously an animation that has not been restored

		//check for finger down
		if (g_pInput->m_Touched && !g_pInput->m_PrevTouched)
		{
			if (CheckButtons())
			{
				touchCounter = 0;
				CheckInputEvents(player[PLAYER_ONE]->getPrevMovement());
			}

			if (player[PLAYER_ONE]->getPrevMovement() == 4)
			{
				MakeHadoken(false);
			}
		}

		//check for hold
		else if (g_pInput->m_Touched && g_pInput->m_PrevTouched)
		{
			if (CheckButtonsStillTouched())
			{
				if (player[PLAYER_ONE]->getPrevMovement() == 4)
				{
					//resize square
					if (projectileLines[PLAYER_ONE].size() > 0)
					{
						ResizeLine(projectileLines[PLAYER_ONE].back(), PLAYER_ONE, projectiles[PLAYER_ONE].back()->m_Y);
					}
				}
				else if (touchCounter < DASH_LIMIT)
				{
					touchCounter++;
				}
				CheckInputEvents(player[PLAYER_ONE]->getPrevMovement());
			}
		}

		//check for finger up
		else if (!g_pInput->m_Touched && g_pInput->m_PrevTouched)
		{
			FingerUp();
		}

		//set the previous state to the current state
		//this is done explicitly as this was removed from Input->Update() for optimisation
		g_pInput->m_PrevTouched = g_pInput->m_Touched;

	}
	else if (player[PLAYER_ONE]->getAttacking()) //else you are punching or kicking check if the player two gets hit. :)
	{
		Collision(PLAYER_TWO, 20, pushBack);
	}

	if (!(player[PLAYER_TWO]->getAnimActiv()))
	{
		CheckResetAnimation(PLAYER_TWO);
		//This is where the code for AI goes.

		if (player[PLAYER_TWO]->getPrevMovement() == 4)
		{
			if (projectiles[PLAYER_TWO].size() == 0)
			{
				player[PLAYER_TWO]->setPrevMovement(-1);
			}
		}
		else
		{
			CheckAINextMove();
		}
	}
	else if (player[PLAYER_TWO]->getAttacking())
	{
		Collision(PLAYER_ONE, -20, -pushBack);
	}

	bool exploded = HadokenOnHadoken();


	if (!exploded)
	{
		CheckProjectiles(PLAYER_ONE);
		CheckProjectiles(PLAYER_TWO);
	}
}

bool Game::HadokenOnHadoken()
{
	if (projectiles[PLAYER_ONE].size() > 0 && projectiles[PLAYER_TWO].size() > 0)
	{
		if (CheckProjectilesCollision(projectiles[PLAYER_TWO].back(), PLAYER_ONE, 0))
		{
			projectiles[PLAYER_ONE].back()->m_AnchorY = (float)(PLAYER_ONE);
			projectiles[PLAYER_TWO].back()->m_AnchorY = (float)(PLAYER_TWO);

			//projectiles[PLAYER_ONE].back()->m_Y += projectiles[PLAYER_TWO].back()->getWidth();
			InitialiseHadoken(projectiles[PLAYER_ONE].back(), PLAYER_ONE);
			projectiles[PLAYER_TWO].back()->m_Y = projectiles[PLAYER_ONE].back()->m_Y;
			InitialiseHadoken(projectiles[PLAYER_TWO].back(), PLAYER_TWO);


			RemoveProjectile(PLAYER_ONE, 0);
			RemoveProjectile(PLAYER_TWO, 0);


			player[PLAYER_ONE]->setPrevMovement(-1);
			player[PLAYER_TWO]->setPrevMovement(-1);


			MessageLabel->m_Text = "Show Down";
			MessageLabel->m_Alpha = 100.0f;
			m_Tweener.Tween(3.0f,
				FLOAT, &MessageLabel->m_Alpha, 0.0f,
				END);
			DBZBlast = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool Game::CheckHadokenRelease()
{
	if(player[PLAYER_ONE]->getPrevMovement() == 4)
	{
		//explode hadoken
		if (projectiles[PLAYER_ONE].size() > 0)
		{
			g_pExplosionManager->Create(projectiles[PLAYER_ONE].at(0));
			RemoveProjectile(PLAYER_ONE, 0);
		}
		return true;
	}
}

void Game::ResizeLine(CSprite* line, bool isAI, float position)
{
	line->m_ScaleY = (float)(abs(player[isAI]->m_Y - position) / line->GetImage()->GetWidth());
}

void Game::FingerUp()
{
	if (CheckHadokenRelease())
	{

	}
	else if (touchCounter < DASH_LIMIT)
	{
		//Jump left or right
		float temp;
		if (player[PLAYER_ONE]->getPrevMovement() == 1)
		{
			temp = (float)(player[PLAYER_ONE]->m_Y + (ScreenHeight / 20));
			if (temp > player[PLAYER_TWO]->m_Y)
			{
				temp = player[PLAYER_TWO]->m_Y;
			}

		}
		else if (player[PLAYER_ONE]->getPrevMovement() == 0)
		{
			temp = (float)(player[PLAYER_ONE]->m_Y - (ScreenHeight / 20));
			if (temp < 0)
			{
				temp = 0;
			}
		}

		Dash(PLAYER_ONE, temp);
	}

	player[PLAYER_ONE]->setPrevMovement(-1);
}

void Game::CheckProjectiles(bool p)
{
	if (projectiles[p].size() > 0)
	{
		//check if two projectiles collide
		if (CheckProjectilesCollision(player[!p], p, 0))
		{
			bool playtwo = !p;
			if (player[playtwo]->getPrevMovement() == 5)
			{
				MakeHadoken(playtwo);
			}
			else
			{
				g_pExplosionManager->Create(projectiles[p].at(0));
				player[playtwo]->subtractFromLife(10);
				CheckLife(playtwo);
			}

			RemoveProjectile(p, 0);
		}
		else
		{
			if (projectiles[p].at(0)->m_Y == projectiles[p].at(0)->getEndY())
			{
				RemoveProjectile(p, 0);
			}
		}
	}
}

void Game::RemoveProjectile(bool player, int checker)
{
	RemoveChild(projectiles[player].at(checker));
	projectiles[player].erase(projectiles[player].begin() + checker);

	RemoveChild(projectileLines[player].at(checker));
	projectileLines[player].erase(projectileLines[player].begin() + checker);
}
//returns if hit
bool Game::CheckProjectilesCollision(CSprite* toHit, bool playOne, int checker)
{
	if (toHit->HitTest(projectiles[playOne].at(checker)->m_X + 20, projectiles[playOne].at(checker)->m_Y))
	{
		return true;
	}
	else //not hit check for
	{
		return false;
	}
}

int  Game::CheckPlayer1Collision(float temp)
{
	if (temp < player[PLAYER_ONE]->m_Y)
	{
		temp = player[PLAYER_ONE]->m_Y;
	}
	return temp;
}

void Game::CheckAINextMove()
{
	//  if we should left dash

	int distance = player[PLAYER_TWO]->m_Y - player[PLAYER_ONE]->m_Y;
	if (!(player[PLAYER_TWO]->getPrevHit()))
	{
		// check if we should kick
		if (distance == 0)
		{
			if (player[PLAYER_ONE]->m_Y > ScreenHeight / 15)
			{
				Kick(PLAYER_TWO, 1.0f);
				player[PLAYER_TWO]->setPrevMovement(2);
			}
			// check if we should punch
			else
			{
				Punch(PLAYER_TWO, 1.0f);
				player[PLAYER_TWO]->setPrevMovement(3);
			}
		}
		else
		{
			//check if we should dash left
			if (player[PLAYER_ONE]->getPrevMovement() != 4)
			{
				if (ScreenHeight / 6 < distance)
				{
					float temp = CheckPlayer1Collision((float)(player[PLAYER_TWO]->m_Y - (ScreenHeight / 15)));
					Dash(PLAYER_TWO, temp);
					player[PLAYER_TWO]->setPrevMovement(0);
				}
				else if (projectiles[PLAYER_ONE].size() > 0)
				{
					MakeHadoken(PLAYER_TWO);
					player[PLAYER_TWO]->setPrevMovement(4);
				}
				// check if we should move left
				else
				{
					float temp = CheckPlayer1Collision(player[PLAYER_TWO]->m_Y - (ScreenHeight / 160));
					player[PLAYER_TWO]->m_Y = temp;
					player[PLAYER_TWO]->setPrevMovement(0);
				}
			}
			else
			{
				if (projectiles[PLAYER_ONE].size() > 0)
				{
					if (projectiles[PLAYER_ONE].at(0)->m_Y - player[PLAYER_TWO]->m_Y < ScreenHeight / 10)
					{
						Block(PLAYER_TWO);
						player[PLAYER_TWO]->setPrevMovement(5);
					}
				}
			}
		}
	}
	else
	{
		// check if we should right dash
		if (player[PLAYER_TWO]->getPrevHit())
		{
			player[PLAYER_TWO]->setPrevMovement(1);
		}
		// check if we should move right
		else if (0)
		{
			player[PLAYER_TWO]->setPrevMovement(1);
		}
	}
	player[PLAYER_TWO]->setPrevHit(false);
}

void Game::Block(bool isAI)
{
	player[isAI]->SetImage(g_pResources->getBlockSprite(isAI));
}

void Game::Dash(bool isAI, float distance)
{
	Animate(isAI);
	float distanceTraveled = (float)(abs(player[isAI]->m_Y - distance));


	float speed = (ScreenHeight / 8);


	float time = distanceTraveled / speed;


	m_Tweener.Tween(time,
		FLOAT, &player[isAI]->m_Y, distance,
		BOOL, &player[isAI]->animActiv, false,
		EASING, Ease::sineInOut,
		END);

}

void Game::PushBack(bool isAI, float position)
{
	if (position < 0)
	{
		position = 0;
	}
	else if (position > ScreenHeight)
	{
		position = ScreenHeight;
	}

	Animate(isAI);
	m_Tweener.Tween(0.5f,
		FLOAT, &player[isAI]->m_Y, position,
		BOOL, &player[isAI]->animActiv, false,
		END);
}

void Game::CheckLife(bool isAI)
{
	if (player[isAI]->getLife() < 0)
	{
		lifeBars[isAI]->m_ScaleY = 0;
		gameOver = true;

		MessageLabel->m_Alpha = 100.0f;
		if (!isAI)
		{
			//display you lose
			MessageLabel->m_Text = "You lose!!!";
			MessageLabel->m_Color = CColor(0xff, 0x00, 0x00, 0xff);
		}
		else
		{
			//display victory
			MessageLabel->m_Text = "You won!!!";
			MessageLabel->m_Color = CColor(0x00, 0xff, 0x00, 0xff);
		}

		if (gameOverSprite == NULL)
		{
			gameOverSprite = new CSprite();
			gameOverSprite->m_X = MessageLabel->m_X * 1.5f;
			gameOverSprite->m_Y = (float)(ScreenHeight / 2);
			gameOverSprite->SetImage(g_pResources->getPauseIcon());
			gameOverSprite->m_ScaleX = (float)((ScreenWidth / 8) / gameOverSprite->GetImage()->GetWidth());
			gameOverSprite->m_ScaleY = gameOverSprite->m_ScaleX;
			gameOverSprite->m_AnchorX = 0.5;
			gameOverSprite->m_AnchorY = 0.5;
		}
		AddChild(gameOverSprite);
	}
	else
	{
		float percent = ((float)player[isAI]->getLife() / 100);
		lifeBars[isAI]->m_ScaleY = (float)((ScreenHeight / 4) * (percent) / lifeBars[isAI]->GetImage()->GetWidth());
	}
}

void Game::Collision(bool isAI, int checkPos, float pushBack)
{
	bool notAI = !isAI;
	if (player[isAI]->HitTest(player[notAI]->m_X + 20, player[notAI]->m_Y + checkPos))
	{
		if (player[notAI]->getKick())
		{
			player[isAI]->subtractFromLife(15);
			PushBack(isAI, player[isAI]->m_Y + (pushBack * 2));
		}
		else //punching
		{
			player[isAI]->subtractFromLife(10);
			PushBack(isAI, player[isAI]->m_Y + pushBack);
		}
		player[isAI]->setPrevMovement(-1);
		CheckLife(isAI);
	}
	else
	{
		player[isAI]->setPrevHit(false);
	}
}

bool Game::CheckButtons()
{
	int i;
	for (i = 0; i < NUM_BUTTONS + 2; i++)
	{
		if (buttons[i]->HitTest(g_pInput->m_X, g_pInput->m_Y))
		{
			player[PLAYER_ONE]->setPrevMovement(i);
			return true;
		}
	}
	return false;
}

bool Game::CheckButtonsStillTouched()
{
	if (player[PLAYER_ONE]->getPrevMovement() != -1)
	{
		if (buttons[player[PLAYER_ONE]->getPrevMovement()]->HitTest(g_pInput->m_X, g_pInput->m_Y))
		{
			return true;
		}
		else
		{
			CheckHadokenRelease();
			player[PLAYER_ONE]->setPrevMovement(-1);
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Game::CheckInputEvents(int i)
{
	float temp;
	switch (i)
	{
	case 0:
		temp = player[PLAYER_ONE]->m_Y - ScreenHeight / 160;
		if (temp > ScreenHeight / 50)
		{
			player[PLAYER_ONE]->m_Y = temp;
		}
		break;
	case 1:
		temp = player[PLAYER_ONE]->m_Y + ScreenHeight / 160;
		if (temp < player[PLAYER_TWO]->m_Y)
		{
			player[PLAYER_ONE]->m_Y = temp;
		}
		break;
	case 2:
		Punch(PLAYER_ONE, 0.1f);
		break;
	case 3:
		Kick(PLAYER_ONE, 0.1f);
		break;
	case 4:
		//hadoken
		break;
	case 5:
		//Block
		break;
	default:
		break;
	}
}

void Game::InitialiseHadoken(CSprite* previous, bool isAI)
{
	Hadoken* temp = new Hadoken();
	temp->m_X = previous->m_X;
	temp->m_Y = previous->m_Y;
	temp->SetImage(g_pResources->getSettingsButton());
	temp->m_AnchorX = previous->m_AnchorX;
	temp->m_AnchorY = previous->m_AnchorY;
	// Fit player to Device size
	temp->m_ScaleX = (float)(ScreenWidth / 8) / temp->GetImage()->GetWidth();
	temp->m_ScaleY = temp->m_ScaleX;
	temp->setWidth(ScreenWidth / 8);
	projectiles[isAI].push_back(temp);

	CSprite* tempLine = new CSprite();
	tempLine->m_X = player[isAI]->m_X + (temp->getWidth() / 2);
	tempLine->m_Y = player[isAI]->m_Y;
	tempLine->m_AnchorX = 0.5;
	tempLine->m_AnchorY = (float)(isAI); /*1.0 for AI, 0.0 fir player*/
	tempLine->SetImage(g_pResources->getGreenImage());
	tempLine->m_ScaleX = (float)(ScreenWidth / 50) / lifeBars[isAI]->GetImage()->GetWidth();

	projectileLines[isAI].push_back(tempLine);
	AddChild(projectileLines[isAI].back());

	ResizeLine(tempLine, isAI, projectiles[isAI].back()->m_Y);

	//we add teh hadoken after to remove overlap
	AddChild(projectiles[isAI].back());
}

void Game::MakeHadoken(bool isAI)
{
	if (projectiles[isAI].size() < 1)
	{
		InitialiseHadoken(player[isAI], isAI);
		Hadoken* temp = projectiles[isAI].back();
		ShootHadoken(isAI, temp);
	}
}

void Game::ShootHadoken(bool isAI, Hadoken* temp)
{
	float distance;
	if (isAI)
	{
		temp->m_Y -= (ScreenWidth / 8);
		distance = temp->m_Y - ScreenHeight;
	}
	else
	{
		temp->m_Y += (ScreenWidth / 8);
		distance = temp->m_Y + ScreenHeight;
	}

	m_Tweener.Tween(3.0f,
		FLOAT, &temp->m_Y, distance,
		END);

	temp->setEndY(distance);
}

void Game::Punch(bool isAI, float delay)
{
	Animate(isAI);
	player[isAI]->SetImage(g_pResources->getPlayButton());
	player[isAI]->setAttacking(true);
	player[isAI]->setKick(false);
	m_Tweener.Tween(0.3f,
		BOOL, &player[isAI]->animActiv, false,
		DELAY, delay,
		END);
}

void Game::Kick(bool isAI, float delay)
{
	Animate(isAI);
	player[isAI]->SetImage(g_pResources->getPlayButton());
	player[isAI]->setAttacking(true);
	player[isAI]->setKick(true);
	m_Tweener.Tween(0.5f,
		BOOL, &player[isAI]->animActiv, false,
		END);
}

void Game::Animate(bool isAI)
{
	player[isAI]->setAnimActiv(true);
	player[isAI]->setPrevActive(true);
}

void Game::RestartGame()
{
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < projectiles[j].size(); i++)
		{
			RemoveChild(projectiles[j].at(i));
		}
		projectiles[j].clear();
	}

	MessageLabel->m_Alpha = 0.0f;
	RemoveChild(gameOverSprite);
	NewGame();

	ResetFighter(PLAYER_ONE, (float)(ScreenHeight / 3));
	ResetFighter(PLAYER_TWO, (float)(player[PLAYER_ONE]->m_Y * 2));

	ResetLine(PLAYER_ONE);
	ResetLine(PLAYER_TWO);
}

void Game::Render()
{
	Scene::Render();
}

// Initialise the games user interface
void Game::initUI()
{
	CSprite* background = new CSprite();
	background->m_X = (float)(ScreenWidth / 2);
	background->m_Y = (float)(ScreenHeight / 2);
	background->SetImage(g_pResources->getMenuBG());
	background->m_AnchorX = 0.5f;
	background->m_AnchorY = 0.5f;
	// Fit background to Device size
	background->m_ScaleX = (float)(ScreenWidth) / background->GetImage()->GetWidth();
	background->m_ScaleY = (float)ScreenHeight / background->GetImage()->GetHeight();
	AddChild(background);

	CSprite* FloorSprite = new CSprite();
	FloorSprite->m_X = (float)(background->m_X / 4);
	FloorSprite->m_Y = (float)(ScreenHeight / 2);
	FloorSprite->SetImage(g_pResources->getFloorImage());
	FloorSprite->m_AnchorX = 0.5f;
	FloorSprite->m_AnchorY = 0.5f;
	// Fit FloorSprite to Device size
	FloorSprite->m_ScaleX = (float)(ScreenWidth / 4) / FloorSprite->GetImage()->GetWidth();
	FloorSprite->m_ScaleY = (float)ScreenHeight / FloorSprite->GetImage()->GetHeight();
	AddChild(FloorSprite);

	player = new Fighter*[2];
	lifeBars = new CSprite*[2];

	CreateFighter(PLAYER_ONE, (float)(ScreenHeight / 3));
	CreateFighter(PLAYER_TWO, (float)(player[PLAYER_ONE]->m_Y * 2));
	CreateLifeBar(PLAYER_ONE, 0);
	CreateLifeBar(PLAYER_TWO, ScreenHeight);


	//2 special buttons
	buttons = new CSprite*[NUM_BUTTONS + 2];

	int i;
	for (i = 0; i < NUM_BUTTONS; i++)
	{
		CreateButton(i, FloorSprite->m_X, (float)(ScreenHeight / 5) * (i + 1));
	}

	CreateButton(i, FloorSprite->m_X + (ScreenWidth / 8), buttons[i - 2]->m_Y + (float)(ScreenHeight / 8));

	i++;

	CreateButton(i, buttons[i - 1]->m_X, buttons[i - 2]->m_Y + (float)(ScreenHeight / 8));

	//flip the first button as it is the same as the second
	buttons[0]->m_Angle += 180.0f;

	float fontScale = (float)IwGxGetScreenWidth() / FONT_DESIGN_WIDTH;
	float actualFontHeight = FONT_HEIGHT * fontScale;

	MessageLabel = new CLabel();
	MessageLabel->m_X = IwGxGetDisplayWidth() / 2;
	MessageLabel->m_Y = ScreenHeight / 2;
	MessageLabel->m_W = FONT_DESIGN_WIDTH;
	MessageLabel->m_H = actualFontHeight;
	MessageLabel->m_AlignHor = IW_2D_FONT_ALIGN_LEFT;
	MessageLabel->m_AlignVer = IW_2D_FONT_ALIGN_TOP;
	MessageLabel->m_Font = g_pResources->getFont();
	MessageLabel->m_ScaleX = fontScale;
	MessageLabel->m_ScaleY = fontScale;
	MessageLabel->m_Angle = 270.0f;
	AddChild(MessageLabel);
	MessageLabel->m_Alpha = 0.0f;
}

void Game::CreateLifeBar(bool isAI, float y)
{
	lifeBars[isAI] = new CSprite();
	lifeBars[isAI]->m_X = (float)(ScreenWidth - (ScreenWidth / 15));
	lifeBars[isAI]->m_Y = y;
	lifeBars[isAI]->SetImage(g_pResources->getGreenImage());
	lifeBars[isAI]->m_AnchorX = 0.0f;
	lifeBars[isAI]->m_AnchorY = (float)(isAI); /*1.0 for AI, 0.0 fir player*/
	// Fit player to Device size
	ResetLine(isAI);
	lifeBars[isAI]->m_ScaleX = (float)(ScreenWidth / 50) / lifeBars[isAI]->GetImage()->GetWidth();
	AddChild(lifeBars[isAI]);
}

void Game::ResetLine(bool isAI)
{
	lifeBars[isAI]->m_ScaleY = (float)(ScreenHeight / 4) / lifeBars[isAI]->GetImage()->GetWidth();
}

void Game::CreateFighter(bool isAI, float y)
{
	player[isAI] = new Fighter();
	player[isAI]->m_X = (float)(ScreenWidth / 4);
	ResetFighter(isAI, y);
	player[isAI]->m_AnchorX = 0.0f;
	player[isAI]->m_AnchorY = (float)(!(isAI)) /*1.0 for player 0.0 for AI*/;
	// Fit player to Device size
	player[isAI]->m_ScaleX = (float)(ScreenWidth / 8) / player[isAI]->GetImage()->GetWidth();
	player[isAI]->m_ScaleY = player[isAI]->m_ScaleX;
	AddChild(player[isAI]);
}

void Game::ResetFighter(bool isAI, float y)
{
	player[isAI]->m_Y = y;
	player[isAI]->resetLife();
	player[isAI]->SetImage(g_pResources->getPlayerSprite());
}

void Game::CreateButton(int i, float x, float y)
{
	buttons[i] = new CSprite();
	//sets where it anchors to the window
	buttons[i]->m_X = x;
	buttons[i]->m_Y = y;
	buttons[i]->SetImage(g_pResources->getButtons(i));
	//sets the point that will act as the anchor on the image
	buttons[i]->m_AnchorX = 0.5f;
	buttons[i]->m_AnchorY = 0.5f;
	// Fit buttons[i] to Device size
	buttons[i]->m_ScaleX = (float)(ScreenWidth / 10) / buttons[i]->GetImage()->GetWidth();
	buttons[i]->m_ScaleY = buttons[i]->m_ScaleX;
	buttons[i]->m_Angle = 270.0f;
	AddChild(buttons[i]);
}

void Game::Init()
{
	//m_Angle = 90.0f;

	ScreenWidth = IwGxGetDeviceWidth();
	ScreenHeight = IwGxGetDeviceHeight();

	gameOverSprite = NULL;

	g_pResources->loadGame();

	initUI();

	pushBack = (ScreenHeight / 20);

	projectiles = new hadoken_vec[2];

	projectileLines = new sprite_vec[2];
	
	g_pExplosionManager = new ExplosionManager();

	NewGame();
}

void Game::OutputLine(int i)
{
	char str[16];
	snprintf(str, 16, "%d", i);
	s3eDebugOutputString(str);
}

void Game::OutputLine(float i)
{
	char str[16];
	snprintf(str, 16, "%f", i);
	s3eDebugOutputString(str);
}

void Game::NewGame()
{
	gameOver = false;
	DBZBlast = false;
}