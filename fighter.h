#if !defined(__FIGHTER_H)
#define __FIGHTER_H

#include "Iw2DSceneGraph.h"
#include "IwTween.h"

using namespace IwTween;
using namespace Iw2DSceneGraphCore;
using namespace Iw2DSceneGraph;


class Fighter : public CSprite
{
protected:
	bool			prevActive;
	bool			attacking;
	bool			kick;
	bool			prevHit;
	int				prevMovement;
	int				life;

public:
	bool			animActiv;
	bool			getAnimActiv() { return animActiv; }
	void			setAnimActiv(bool a) { animActiv = a; }
	bool			getPrevActive() { return prevActive; }
	void			setPrevActive(bool a) { prevActive = a; }

	bool			getPrevHit() { return prevHit; }
	void			setPrevHit(bool a) { prevHit = a; }

	int				getPrevMovement() { return prevMovement; }
	void			setPrevMovement(int a) { prevMovement = a; }

	bool			getAttacking() { return attacking; }
	void			setAttacking(bool a) { attacking = a; }

	bool			getKick() { return kick; }
	void			setKick(bool a) { kick = a; }

	int			getLife() { return life; }
	void			subtractFromLife(int a) { life -= a;
	}
	void			resetLife() { life = 100; }

public:
	Fighter()  { prevActive = false; 
	animActiv = false;
	attacking = false;
	prevMovement = -1;
	}
	~Fighter();


};

#endif