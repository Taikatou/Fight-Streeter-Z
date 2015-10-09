#if !defined(__HADOKEN_H)
#define __HADOKEN_H

#include "Iw2DSceneGraph.h"
#include "IwTween.h"

using namespace IwTween;
using namespace Iw2DSceneGraphCore;
using namespace Iw2DSceneGraph;


class Hadoken : public CSprite
{
protected:
	float endY;
	int width;
	int counter;

public:
	void setEndY(int y) { endY = y; }
	int  getEndY() { return endY; }
	void setWidth(int w) { width = w; }
	int  getWidth() { return width; }

public:
	Hadoken()  {
	}
	~Hadoken();


};

#endif