#include "HighScoreLoadingPanel.h"

#include "numeral.h"
#include "spriteBank.h"

#include "gameControl.h"


#include "minorGems/util/stringUtils.h"

#include <math.h>
#include <GL/gl.h>


Color scoreLoadingGreen( 0, 1, 0 );

Color scoreLoadingRed( 1, 0, 0 );



HighScoreLoadingPanel::HighScoreLoadingPanel( int inW, int inH )
        : Panel( inW, inH ),
          mMessage( "loading scores" ),
          mStatusLight( inW / 2, inH / 2 ),
          mBlinkTime( 0 ),
          mScoreToPost( NULL ),
          mWebRequest( NULL ),
          mServerFinderURL(
            "http://hcsoftware.sourceforge.net/primrose/serverLocation.txt" ),
          mServerURL( NULL ) {
          
    mStatusLight.setColor( scoreLoadingGreen.copy() );

    mServerURLFetchRequest = new WebRequest( "GET",
                                             mServerFinderURL,
                                             NULL );
    }


    
HighScoreLoadingPanel::~HighScoreLoadingPanel() {
    if( mScoreToPost != NULL ) {
        delete mScoreToPost;
        }
    if( mWebRequest != NULL ) {
        delete mWebRequest;
        }

    if( mServerURL != NULL ) {
        delete [] mServerURL;
        }
    if( mServerURLFetchRequest != NULL ) {
        delete mServerURLFetchRequest;
        }
    
    }



void HighScoreLoadingPanel::setScoreToPost( ScoreBundle *inBundle ) {
    if( mScoreToPost != NULL ) {
        delete mScoreToPost;
        }

    mScoreToPost = inBundle;

    mMessage = "posting score";
    }





void HighScoreLoadingPanel::step() {
    Panel::step();
    
    mStatusLight.step();


    mBlinkTime += 0.2;

    if( mServerURLFetchRequest != NULL ) {
        int stepResult = mServerURLFetchRequest->step();
        
        if( stepResult == 1 ) {
            // done
            
            char *result = mServerURLFetchRequest->getResult();
            
            SimpleVector<char *> *tokens = tokenizeString( result );
            
            delete [] result;

            
            char *returnedURL = *( tokens->getElement( 0 ) );
                
            if( mServerURL != NULL ) {
                delete [] mServerURL;
                }
                    
            mServerURL = stringDuplicate( returnedURL );
            
            printf( "Got server URL: %s\n", mServerURL );
            

            for( int i=0; i<tokens->size(); i++ ) {
                delete [] *( tokens->getElement( i ) );
                }
            delete tokens;


            delete mServerURLFetchRequest;
            mServerURLFetchRequest = NULL;
            }
        else if( stepResult == -1 ) {
            // error
            delete mServerURLFetchRequest;
            mServerURLFetchRequest = NULL;
            
            mMessage = "connect failed";

            mStatusLight.setColor( scoreLoadingRed.copy() );
            }
        }
    else if( mWebRequest != NULL ) {
        //int stepResult = mWebRequest->step();
    
        // FIXME
        }
        
    }



void HighScoreLoadingPanel::setVisible( char inIsVisible ) {
    Panel::setVisible( inIsVisible );
    
    if( inIsVisible ) {
        
        if( mWebRequest != NULL ) {
            delete mWebRequest;
            }
        

        if( mScoreToPost != NULL ) {
            // FIXME
            // compose request
            // char *url =
            }
        }
    
    }



char HighScoreLoadingPanel::pointerUp( int inX, int inY ) {
    
    char consumed = Panel::pointerUp( inX, inY );
    
    if( consumed ) {
        return true;
        }
    

    if( ! isSubPanelVisible() ) {

        } 
    
    return false;
    }



Color loadingMessageColor( 76/255.0, 76/255.0, 255/255.0 );

        
void HighScoreLoadingPanel::drawBase() {
    
    Panel::drawBase();
    
    if( mFadeProgress > 0 ) {
        
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        int textY = 100;
            
        drawStringBig( mMessage, center, 
                       mW / 2,
                       textY,
                       &loadingMessageColor, mFadeProgress );        
        
            
        glDisable( GL_BLEND );

        

        mStatusLight.drawGrid( mFadeProgress );

        float glowVal = sin( mBlinkTime - M_PI / 2 ) * 0.5 + 0.5;

        mStatusLight.drawPieceCenter( mFadeProgress * glowVal );
        mStatusLight.drawPieceHalo( mFadeProgress * glowVal );
        }
    
    }



void HighScoreLoadingPanel::closePressed() {
    }


        
        
