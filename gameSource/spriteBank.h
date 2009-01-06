#ifndef SPRITE_BANK_INCLUDED
#define SPRITE_BANK_INCLUDED


#include "minorGems/graphics/Color.h"


#define numSprites 1

enum SpriteHandle {
    gridSpace = 0 
    };




void initSpriteBank();


void freeSpriteBank();



void drawSprite( SpriteHandle inSpriteHandle,
                 float inCenterX, float inCenterY, 
                 float inXRadius, float inYRadius,
                 Color *inColor = NULL, double inAlpha = 1.0 );



#endif

