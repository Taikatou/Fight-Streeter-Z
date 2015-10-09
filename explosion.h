
#if !defined(__EXPLOSION_H__)
#define __EXPLOSION_H__

#include <list>
#include "Iw2DSceneGraph.h"
#include "hadoken.h"

using namespace Iw2DSceneGraphCore;
using namespace Iw2DSceneGraph;

/**
 * @class Explosion
 *
 * @brief A explosion.
 *
 * An explosion game object. Each explosion has its an InUse flag and sprite node associated with it
 */
class Explosion : public CSprite
{
protected:
    bool        InUse;
public:
    bool        isInUse() const         { return InUse; }
    void        setUsed(bool used)      { InUse = used; }
public:
    Explosion();
    ~Explosion()        {}

    void        Update(float deltaTime, float alphaMul);

};

/**
 * @class ExplosionManager
 *
 * @brief Manages a collection of re-usable explosions.
 */
class ExplosionManager
{
protected:
    std::list<Explosion*>       Explosions; // Explosions list
	void	CreateExplosion(Explosion* exp, Hadoken* previous);

public:
    Explosion*      Create(Hadoken* previous);
    void            ReleaseAll();

    ~ExplosionManager();

};

extern ExplosionManager*    g_pExplosionManager;


#endif  // __GEM_H__


