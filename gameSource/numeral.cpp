
#include "numeral.h"
#include "spriteBank.h"

#include <GL/gl.h>


void drawNumeral( int inNumber,
                  float inCenterX, float inCenterY,
                  Color *inColor, float inAlpha ) {


    glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    drawSprite( numerals,
                inCenterX, inCenterY,
                8, 10,
                inColor, inAlpha,
                inNumber * 20 / 256.0f,
                20 / 256.0f );

    glDisable( GL_BLEND );
    }

