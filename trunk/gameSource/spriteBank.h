#ifndef SPRITE_BANK_INCLUDED
#define SPRITE_BANK_INCLUDED


#include "minorGems/graphics/Color.h"


#define numSprites 7

enum SpriteHandle {
    gridLineTop = 0,
    gridLineBottom,
    gridLineLeft,
    gridLineRight,
    piece,
    pieceHalo,
    pieceCenter
    };




void initSpriteBank();


void freeSpriteBank();



void drawSprite( SpriteHandle inSpriteHandle,
                 float inCenterX, float inCenterY, 
                 float inXRadius, float inYRadius,
                 Color *inColor = NULL, double inAlpha = 1.0 );



#endif

