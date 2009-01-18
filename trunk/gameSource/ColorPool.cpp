

#include "ColorPool.h"

#include "spriteBank.h"

#include "numeral.h"


#include <math.h>

#include <GL/gl.h>


#include "minorGems/util/random/CustomRandomSource.h"

extern CustomRandomSource randSource;


#define numColors 7

Color pieceColors[numColors] = { 
    //Color( 255/255.0, 128/255.0, 0/255.0 ),
    // redder orange
    Color( 255/255.0, 112/255.0, 0/255.0 ),
    
    Color( 128/255.0, 255/255.0, 0/255.0 ),
    
    //    Color( 96/255.0,  0/255.0,   128/255.0 ),
    // brighter purple
    //Color( 120/255.0,  0/255.0,   160/255.0 ),
    // bluer purple
    Color( 96/255.0,  0/255.0,   192/255.0 ),

    Color( 192/255.0, 0/255.0,   0/255.0 ),
    
    //Color( 0/255.0,   128/255.0, 96/255.0 ),
    // darker teal
    //Color( 0/255.0,   96/255.0, 72/255.0 ),
    // bluer teal
    //Color( 0/255.0,   96/255.0, 96/255.0 ),
    // dark green instead?
    Color( 0/255.0,   96/255.0, 0/255.0 ),

    //Color( 255/255.0, 96/255.0,  255/255.0 ),
    // ligher pink (to avoid purple)
    Color( 255/255.0, 128/255.0,  255/255.0 ),
    
    //Color( 128/255.0, 96/255.0,  0/255.0 )
    // greener, lighter brown
    //Color( 128/255.0, 128/255.0,  0/255.0 )    
    
    // grey
    Color( 128/255.0, 128/255.0,  128/255.0 )    
    //Color( 255/255.0, 255/255.0, 160/255.0 )//,
    //    Color( 128/255.0, 96/255.0,  0/255.0 )    
    };


#define startingSteps 96
#define minSteps 6

ColorPool::ColorPool( int inX, int inY )
        : mX( inX ), mY( inY ), mNumActiveColors( 3 ), mColorsToSkip( 0 ),
          mStepsBetweenUpdates( startingSteps ),
          mStepsUntilUpdate( startingSteps ),
          mLastStepCount( startingSteps ),
          mStepCountTransitionProgress( 1 ),
          mLevel( 1 ) {

    int i;
    
    int xOffset =  -40 * 3;
    
    for( i=0; i<7; i++ ) {
        
        mSpaces[i] = new GridSpace( inX + xOffset, inY );

        xOffset += 40;
        }
    
    for( i=0; i<6; i++ ) {
        mSpaces[i]->mNeighbors[ GridSpace::right ] = mSpaces[i+1];
        mSpaces[i+1]->mNeighbors[ GridSpace::left ] = mSpaces[i];
        }

    for( i=mColorsToSkip; i<mNumActiveColors; i++ ) {
        mSpaces[i]->setColor( pieceColors[i].copy() );
        }
    
    }



ColorPool::~ColorPool() {
    for( int i=0; i<7; i++ ) {
        delete mSpaces[i];
        }
    }



Color *ColorPool::pickColor() {
    int colorIndex = randSource.getRandomBoundedInt( mColorsToSkip, 
                                                     mNumActiveColors - 1 );
    
    return pieceColors[ colorIndex ].copy();
    }



void ColorPool::registerMove() {
    mStepsUntilUpdate --;
    
    mStepCountTransitionProgress = 0;
    
    if( mStepsUntilUpdate == 0 ) {
        
        mStepsBetweenUpdates /= 2;
        
        if( mStepsBetweenUpdates < minSteps ) {
            mStepsBetweenUpdates = minSteps;
            }
        
        mStepsUntilUpdate = mStepsBetweenUpdates;


        if( mNumActiveColors < numColors ) {
            mNumActiveColors ++;

            int i = mNumActiveColors - 1;
            
            mSpaces[i]->setColor( pieceColors[i].copy() );

            mLevel++;
            }
        else if( mColorsToSkip < numColors - 1 ) {
            mSpaces[mColorsToSkip]->setColor( NULL );            
            mColorsToSkip++;

            mLevel++;
            }        
        }
    }



void ColorPool::deRegisterMove() {
    mStepsUntilUpdate ++;
    mStepCountTransitionProgress = 0;
    }

    

Color colorAddCountColor( 76/255.0, 76/255.0, 255/255.0 );
Color colorRemoveCountColor( 0/255.0, 0/255.0, 0/255.0 );


        

void ColorPool::draw( float inAlpha ) {    

    // now draw spaces
    int i;
    for( i=0; i<7; i++ ) {
        mSpaces[i]->drawGrid( inAlpha );
        }
    for( i=0; i<7; i++ ) {
        mSpaces[i]->drawPieceCenter( inAlpha );
        }
    for( i=0; i<7; i++ ) {
        mSpaces[i]->drawPieceHalo( inAlpha );
        }

    if( mNumActiveColors < numColors || mColorsToSkip < (numColors-1) ) {
        
        Color *thisNumberColor;
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        
        
        if( mNumActiveColors < numColors ) {
            i = mNumActiveColors;
            glBlendFunc( GL_SRC_ALPHA, GL_ONE );
            thisNumberColor = &colorAddCountColor;
            }
        else {
            i = mColorsToSkip;
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            thisNumberColor = &colorRemoveCountColor;
            }
        
        // draw counter over next active space

        
        // blend between last count
        
        drawCounter( mStepsUntilUpdate,
                     mSpaces[i]->mX,  mSpaces[i]->mY,
                     thisNumberColor,
                     mStepCountTransitionProgress * inAlpha );
        
        
        if( mStepCountTransitionProgress < 1 && mLastStepCount > 1 ) {
            
            // skip drawing if counter moving to next space (when last count 0)
            // thus, counter fades in from blank in next space
            
            drawCounter( mLastStepCount,
                         mSpaces[i]->mX,  mSpaces[i]->mY,
                         thisNumberColor,
                         ( 1 - mStepCountTransitionProgress ) * inAlpha  );
            }
        

        int lastSpaceIndex = -1;
        

        if( i > 0 && mNumActiveColors < numColors ) {
            lastSpaceIndex = i-1;
            }
        else if( mNumActiveColors == numColors && mColorsToSkip == 0 ) {
            lastSpaceIndex = numColors - 1;
            }
        else if( mNumActiveColors == numColors && mColorsToSkip > 0 ) {
            lastSpaceIndex = i-1;
            }
                 

        
        // skip this at beginning, when there are first 3 colors
        // only (otherwise, a 1 fades out on 3rd color)
        if( mNumActiveColors > 3
            &&
            lastSpaceIndex >= 0 
            && 
            ( mSpaces[lastSpaceIndex]->mDrawColor == NULL 
              || 
              ( mNumActiveColors < numColors 
                &&
                mSpaces[lastSpaceIndex]->mDrawColor->a < 1 )
              ||
              ( mNumActiveColors >= numColors 
                &&
                mSpaces[lastSpaceIndex]->mDrawColor->a > 0 ) ) ) {

            
            float alpha;
            if( mColorsToSkip == 0 ) {
                // last space fading in
                alpha = 1;
                
                if( mSpaces[lastSpaceIndex]->mDrawColor != NULL ) {
                    alpha = 1 - mSpaces[lastSpaceIndex]->mDrawColor->a;
                    }

                glBlendFunc( GL_SRC_ALPHA, GL_ONE );
                thisNumberColor = &colorAddCountColor;
                }
            else {
                // last space fading out
                alpha = 0;
                
                if( mSpaces[lastSpaceIndex]->mDrawColor != NULL ) {
                    alpha = mSpaces[lastSpaceIndex]->mDrawColor->a;
                    }
                glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
                thisNumberColor = &colorRemoveCountColor;
                }
            


            // fade out 1 on last space as color fades in
            
            drawCounter( 1,
                         mSpaces[lastSpaceIndex]->mX,  
                         mSpaces[lastSpaceIndex]->mY,
                         thisNumberColor,
                         alpha * inAlpha );
            }
        

        glDisable( GL_BLEND );
        }
    
    
    }



void ColorPool::step() {
    
    for( int i=0; i<7; i++ ) {
        mSpaces[i]->step();
        }

    if( mStepCountTransitionProgress < 1 ) {
        
        mStepCountTransitionProgress += 0.2;
        
        if( mStepCountTransitionProgress >= 1 ) {
            mStepCountTransitionProgress = 1;
            
            // done with transition
            mLastStepCount = mStepsUntilUpdate;
            }
        }
    
    }


