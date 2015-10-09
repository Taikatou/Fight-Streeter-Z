
#if !defined(__RESOURCES_H__)
#define __RESOURCES_H__

#define MAX_GEM_TYPES   5

#include "Iw2D.h"
#include "Iw2DSceneGraph.h"

using namespace Iw2DSceneGraph;

/**
* @class Resources
*
* @brief Resources class
*
* The that manages the lifetime of the following types of resources:
* - Images
* - Fonts
* - Atlases
*/
class Resources
{
protected:

	CIw2DImage*     MenuBG;
	CIw2DImage*		FloorImage;
	CIw2DImage*     MenuButton;
	CIw2DImage*     PauseIcon;
	CIw2DImage*     PlayButton;
	CIw2DFont*      Font;
	CIw2DImage*		NewGame;

	CIw2DImage*		greenImage;
	
	CIw2DImage*		settingsButtonImage;

	CIw2DImage*		playerSprite;
	CIw2DImage*		aiSprite;

	CIw2DImage*		playerBlockSprite;
	CIw2DImage*		aiBlockSprite;


	CIw2DImage*		highScoreImage;
	CIw2DImage*		buttonA_Image;
	CIw2DImage*		buttonB_Image;
	CIw2DImage*		buttonC_Image;
	CIw2DImage*		buttonD_Image;

	CIw2DImage*     Explosion;
	CAtlas*         ExplosionAtlas;


public:
	CIw2DImage*     getMenuBG()                 { return MenuBG; }
	CIw2DImage*     getBlockSprite(bool isAI)                 {
		if (isAI)
		{
			return aiBlockSprite;
		}
		else
		{
			return playerBlockSprite;
		}
	}
	CIw2DImage*     getPlayerSprite()                 { return playerSprite; }
	CIw2DImage*     getGreenImage()                 { return greenImage; }
	CIw2DImage*     getMenuButton()             { return MenuButton; }
	CIw2DImage*     getPlayButton()             { return PlayButton; }
	CIw2DImage*     getPauseIcon()              { return PauseIcon; }
	CIw2DFont*      getFont()                   { return Font; }
	CIw2DImage*     getNewButton()				{ return NewGame; }
	CIw2DImage*     getHighScoreImage()			{ return highScoreImage; }
	CIw2DImage*		getAiSprite()				{ return aiSprite; }
	CIw2DImage*     getSettingsButton()			{ return settingsButtonImage; }
	CIw2DImage*		getFloorImage()				{ return FloorImage; }
	CIw2DImage*		getButtons(int x)		{
		switch (x)
		{
		case 0: return MenuButton;
			break;
		case 1: return MenuButton;
			break;
		case 2: return buttonA_Image;
			break;
		case 3: return buttonB_Image;
			break;
		case 4: return buttonC_Image;
			break;
		case 5: return buttonD_Image;
			break;
		default: return buttonD_Image;
			s3eDebugOutputString("error, I wasn't suppose to get here");
			break;
		}
	}

	CAtlas*         getExplosionAtlas()         { return ExplosionAtlas; }


public:
	Resources();
	~Resources();

	void loadGame();
};

extern Resources* g_pResources;




#endif  // __RESOURCES_H__


