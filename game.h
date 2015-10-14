/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#if !defined(__GAME_H__)
#define __GAME_H__

#include "fighter.h"
#include "scene.h"
#include "hadoken.h"
// Constants that are used to fit the game to different screen sizes

typedef std::vector<Hadoken*> hadoken_vec;
typedef std::vector<CSprite*> sprite_vec;

/**
 * @enum  eGameState
 *
 * @brief List of game states.
 */
class Game : public Scene
{
protected:
	bool gameOver;
	bool DBZBlast;
	bool restartButtonHit;
	uint32 ScreenWidth;
	uint32 ScreenHeight;
	int				touchCounter;
	CSprite**		buttons;
	CSprite**		lifeBars;
	CSprite*		gameOverSprite;
	Fighter**		player;
	CLabel*			MessageLabel;
	CSprite*		newGame;
	hadoken_vec*	projectiles;
	sprite_vec*		projectileLines;
	float			pushBack;
public:

private:
	void            initUI();
public:
    Game() {}
    ~Game();

    // initialise the game
    void            Init();

    // Update the game
    void            Update(float deltaTime = 0.0f, float alphaMul = 1.0f);

	void			pauseGame();
    // Render the game
	void            Render();
	void			NewGame();
	void			OutputLine(int i);
	void			OutputLine(float i);

	void			CheckInputEvents(int i);
	void			Animate(bool isAI);
	void			Move(float temp);
	bool			CheckButtonsStillTouched();
	bool			CheckButtons();
	void			Collision(bool isAI, int checkPos, float pushBack);
	void			Dash(bool isAI, float distance);
	void			CheckAINextMove();

	int				CheckPlayer1Collision(float temp);
	void			PushBack(bool isAI, float position);
	void			Kick(bool isAI, float delay);
	void			Punch(bool isAI, float delay);
	void			CheckResetAnimation(bool isAI);
	void			CheckLife(bool isAI);

	void			MakeHadoken(bool isAI);
	bool			CheckProjectilesCollision(CSprite* toHit, bool isAI, int checker);
	void			ShootHadoken(bool isAI, Hadoken* temp);
	void			Block(bool isAI);
	void			RemoveProjectile(bool player, int checker);

	void			CreateButton(int i, float x, float y);

	void			CreateFighter(bool isAI, float y);
	void			CreateLifeBar(bool isAI, float y);
	void			CheckProjectiles(bool player);

	void			FingerUp();
	void			ResizeLine(CSprite* line, bool isAI, float position);
	bool			HadokenOnHadoken();
	bool			CheckHadokenRelease();
	void			InitialiseHadoken(CSprite* previous, bool isAI);
	void			NormalGameLoop();
	void			DBZLoop();
	void			DBZWin(bool isAI);
	void			RestartGame();
	void			ResetFighter(bool isAI, float y);
	void			ResetLine(bool isAI);
};


#endif  // __GAME_H__


