

#include "minorGems/graphics/Color.h"


// 10 for comma
void drawNumeral( int inNumber,
                  float inCenterX, float inCenterY,
                  Color *inColor = NULL, float inAlpha = 1.0 );

void drawNumeralBig( int inNumber,
                     float inCenterX, float inCenterY,
                     Color *inColor = NULL, float inAlpha = 1.0 );



void drawScorePip( int inScore,
                   float inCenterX, float inCenterY,
                   Color *inColor = NULL, float inAlpha = 1.0 );


void drawScore( int inScore,
                float inRightX, float inBottomY,
                Color *inColor = NULL, float inAlpha = 1.0 );
