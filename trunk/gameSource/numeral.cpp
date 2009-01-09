
#include "numeral.h"
#include "spriteBank.h"

#include <GL/gl.h>

#include <math.h>



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



void drawScorePip( int inScore,
                   float inCenterX, float inCenterY,
                   Color *inColor, float inAlpha ) {
    
    int numDigits = (int)( floor( log10( inScore ) ) ) + 1;
    

    // draw up to 9 digits

    if( numDigits > 9 ) {
        printf( "Error:  more than 9 digits passed to drawScorePip: %d\n",
                inScore );
        
        numDigits = 9;
        }
    
    int rowSep = 2;
    int colSep = 3;
    
    int centerOffsetsX[9] = { 
        6 + colSep,
        0,
        -( 6 + colSep ),
        6 + colSep,
        0,
        -( 6 + colSep ),
        6 + colSep,
        0,
        -( 6 + colSep ) 
        };
    
    int centerOffsetsY[9] = {
        10 + rowSep,
        10 + rowSep,
        10 + rowSep,
        0,
        0,
        0,
        -( 10 + rowSep ),
        -( 10 + rowSep ),
        -( 10 + rowSep ) 
        };
    
    // find center of mass for all digets together

    int xSum = 0;
    int ySum = 0;
    
    int i;
    
    for( i=0; i<numDigits; i++ ) {
        xSum += centerOffsetsX[ i ];
        ySum += centerOffsetsY[ i ];
        }
    
    int cX = xSum / numDigits;
    int cY = ySum / numDigits;


    if( numDigits > 3 ) {
        // center of mass doesn't look good for more than one row

        cX = 0;
        
        if( numDigits < 7 ) {
            cY = ( 10 + rowSep ) / 2;
            }
        else {
            cY = 0;
            }
        }
    
    
    
    for( i=0; i<numDigits; i++ ) {
        
        int number = ( inScore / ( (int)( pow( 10, i ) ) ) ) % 10;
        

        drawNumeral( number,
                     inCenterX - cX + centerOffsetsX[i], 
                     inCenterY - cY + centerOffsetsY[i],
                     inColor, inAlpha );
        }

    // commas
    if( numDigits > 3 ) {
        drawNumeral( 10,
                     inCenterX - cX + centerOffsetsX[3] + 3 + colSep,
                     inCenterY - cY + centerOffsetsY[3] + 1,
                     inColor, inAlpha );
        }
    if( numDigits > 6 ) {
        drawNumeral( 10,
                     inCenterX - cX + centerOffsetsX[6] + 3 + colSep,
                     inCenterY - cY + centerOffsetsY[6] + 1,
                     inColor, inAlpha );
        }
    
    }

