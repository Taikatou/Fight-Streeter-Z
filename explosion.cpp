
#include "explosion.h"
#include "hadoken.h"
#include "resources.h"
#include "game.h"

#define EXPLOSION_ANIM_DURATION     1.0f

ExplosionManager*   g_pExplosionManager = 0;

Explosion::Explosion() : InUse(false)
{
    SetAtlas(g_pResources->getExplosionAtlas());
    SetAnimDuration(EXPLOSION_ANIM_DURATION);
    SetAnimRepeat(1);
    m_AnchorX = 0.5;
    m_AnchorY = 0.5;
}

void Explosion::Update(float deltaTime, float alphaMul)
{
    int repeat = m_AnimRepeat;
    CSprite::Update(deltaTime, alphaMul);

    // If animation has finished then disable explosion and return it back to pool
    if (repeat == 1 && m_AnimRepeat == 0)
    {
        InUse = false;
        m_IsVisible = false;
    }
}

ExplosionManager::~ExplosionManager()
{
}

void ExplosionManager::CreateExplosion(Explosion* exp, Hadoken* previous)
{
	exp->m_ScaleX = (previous->getWidth() * 8) / exp->GetImage()->GetWidth();
	exp->m_ScaleY = (previous->getWidth() * 6) / exp->GetImage()->GetHeight();
	float half = (previous->getWidth() / 2);
	exp->m_X = (float)(previous->m_X) + half;
	exp->m_Y = (float)(previous->m_Y);
	exp->m_IsVisible = true;
	exp->setUsed(true);
	exp->SetAnimDuration(EXPLOSION_ANIM_DURATION);
	exp->SetAnimRepeat(1);
	exp->m_Angle = 270.0f;

}

Explosion* ExplosionManager::Create(Hadoken* previous)
{
    // Search for a spare explosion slot
    for (std::list<Explosion*>::iterator it = Explosions.begin(); it != Explosions.end(); ++it)
    {
        // Found a free explosion so re-use it
        if (!(*it)->isInUse())
        {
			CreateExplosion(*it, previous);
            return *it;
        }
    }

    // No explosion found so allocate a new one
    Game* game = (Game*)g_pSceneManager->Find("game");
    Explosion* exp = new Explosion();
    game->AddChild(exp);

	CreateExplosion(exp, previous);
	
    return exp;
}

void ExplosionManager::ReleaseAll()
{
    // Release all explosions
    for (std::list<Explosion*>::iterator it = Explosions.begin(); it != Explosions.end(); ++it)
    {
        (*it)->setUsed(false);
        (*it)->m_IsVisible = false;
    }
}
