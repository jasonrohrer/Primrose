

#include "ColorPool.h"

#include "spriteBank.h"

#include "numeral.h"


#include <math.h>

#include <GL/gl.h>


#include "minorGems/util/random/StdRandomSource.h"

extern StdRandomSource randSource;


#define numColors 7

Color pieceColors[numColors] = { 
    Color( 255/255.0, 128/255.0, 0/255.0 ),
    Color( 128/255.0, 255/255.0, 0/255.0 ),
    Color( 96/255.0,  0/255.0,   128/255.0 ),
    Color( 192/255.0, 0/255.0,   0/255.0 ),
    Color( 0/255.0,   128/255.0, 96/255.0 ),
    Color( 255/255.0, 96/255.0,  255/255.0 ),
    Color( 255/255.0, 255/255.0, 160/255.0 )//,
    //    Color( 128/255.0, 96/255.0,  0/255.0 )    
    };


#define startingSteps 12


ColorPool::ColorPool( int inX, int inY )
        : mX( inX ), mY( inY ), mNumActiveColors( 3 ), mColorsToSkip( 0 ),
          mStepsBetweenUpdates( startingSteps ),
          mStepsUntilUpdate( startingSteps ),
          mLastStepCount( startingSteps ),
          mStepCountTransitionProgress( 1 ) {

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
        
        if( mStepsBetweenUpdates < 6 ) {
            mStepsBetweenUpdates = 6;
            }
        
        mStepsUntilUpdate = mStepsBetweenUpdates;


        if( mNumActiveColors < numColors ) {
            mNumActiveColors ++;

            int i = mNumActiveColors - 1;
            
            mSpaces[i]->setColor( pieceColors[i].copy() );
            }
        else if( mColorsToSkip < numColors - 1 ) {
            mSpaces[mColorsToSkip]->setColor( NULL );            
            mColorsToSkip++;
            }        
        }
    }

    

Color colorAddCountColor( 76/255.0, 76/255.0, 255/255.0 );


        

void ColorPool::draw() {    

    // now draw spaces
    int i;
    for( i=0; i<7; i++ ) {
        mSpaces[i]->drawGrid();
        }
    for( i=0; i<7; i++ ) {
        mSpaces[i]->drawPieceCenter();
        }
    for( i=0; i<7; i++ ) {
        mSpaces[i]->drawPieceHalo();
        }

    if( mNumActiveColors < numColors ) {
        i = mNumActiveColors;
        
        // draw counter over next active space

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        
        // blend between last count
        
        drawCounter( mStepsUntilUpdate,
                     mSpaces[i]->mX,  mSpaces[i]->mY,
                     &( colorAddCountColor ),
                     mStepCountTransitionProgress );
        
        
        if( mStepCountTransitionProgress < 1 && mLastStepCount > 1 ) {
            
            // skip drawing if counter moving to next space (when last count 0)
            // thus, counter fades in from blank in next space
            
            drawCounter( mLastStepCount,
                         mSpaces[i]->mX,  mSpaces[i]->mY,
                         &( colorAddCountColor ),
                         1 - mStepCountTransitionProgress );
            }
        

        if( i > 0 && 
            ( mSpaces[i-1]->mDrawColor == NULL 
              || 
              mSpaces[i-1]->mDrawColor->a < 1 ) ) {
            
            float alpha = 1;
            
            if( mSpaces[i-1]->mDrawColor != NULL ) {
                alpha = 1 - mSpaces[i-1]->mDrawColor->a;
                }
            

            // fade out 1 on last space as color fades in
            
            drawCounter( 1,
                         mSpaces[i-1]->mX,  mSpaces[i-1]->mY,
                         &( colorAddCountColor ),
                         alpha );
            }
        

        glDisable( GL_BLEND );
        }
    
    
    }



void ColorPool::step() {
    
    for( int i=0; i<7; i++ ) {
        mSpaces[i]->step();
        }

    if( mStepCountTransitionProgress < 1 ) {
        
        mStepCountTransitionProgress += 0.075;
        
        if( mStepCountTransitionProgress >= 1 ) {
            mStepCountTransitionProgress = 1;
            
            // done with transition
            mLastStepCount = mStepsUntilUpdate;
            }
        }
    
    }


