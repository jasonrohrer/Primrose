#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>



// callbacks for GLUT
void callbackMotion( int inX, int inY );
// ignore passive motion
// void callbackPassiveMotion( int inX, int inY );
void callbackMouse( int inButton, int inState, int inX, int inY );
void callbackDisplay();
void callbackTimer( int inValue );

// keyboard to catch quit keys (don't pass to game)
void callbackKeyboard( unsigned char inKey, int inX, int inY );


void callbackReshape( int inW, int inH );



void cleanUpAtExit() {
    printf( "Exiting...\n" );
    
    freeFrameDrawer();    
    }


int w = 320;
int h = 480;


// 25 fps
int frameMS = 40;


int main( int inNumArgs, char **inArgs ) {
    
    glutInit( &inNumArgs, inArgs );
    
    
    
    atexit( cleanUpAtExit );


    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );

    glutInitWindowSize( w, h );
    glutCreateWindow( "game1" );


    glutMotionFunc( callbackMotion );
	glutMouseFunc( callbackMouse );
	//glutPassiveMotionFunc( callbackPassiveMotion );
	glutDisplayFunc( callbackDisplay );
	//glutIdleFunc( callbackIdle );

    glutReshapeFunc( callbackReshape );
    

    // 25 fps
    glutTimerFunc( frameMS, callbackTimer, 0 );
    


    glutKeyboardFunc( callbackKeyboard );



    initFrameDrawer( w, h );


    glutMainLoop();


    return 0;    
    }




void callbackDisplay() {
		
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0, 320, 480, 0, -1.0f, 1.0f);
    
    
    glMatrixMode(GL_MODELVIEW);

    glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
    
    drawFrame();
    
	
	glutSwapBuffers();
	}


/*
void callbackIdle() {
	glutPostRedisplay();
	}
*/

void callbackTimer( int inValue ) {
    glutTimerFunc( frameMS, callbackTimer, 0 );
    
    
    if( glutGet( GLUT_WINDOW_WIDTH ) != w ||
        glutGet( GLUT_WINDOW_WIDTH ) != h ) {
        
        glutReshapeWindow( w, h );
        }
    
    glutPostRedisplay();
	}



void callbackMotion( int inX, int inY ) {
    pointerMove( inX, inY );
	}
		
	
	
void callbackMouse( int inButton, int inState, int inX, int inY ) {
	if( inState == GLUT_DOWN ) {
        pointerDown( inX, inY );
        }
    else if( inState == GLUT_UP ) {
        pointerUp( inX, inY );
        }
    else {
        printf( "Error:  Unknown mouse state received from OpenGL\n" );
        }	
    }


void callbackKeyboard( unsigned char inKey, int inX, int inY ) {
    
    // q or escape
    if( inKey == 'q' || inKey == 'Q' || inKey == 27 ) {
        exit( 0 );
        }
	}


void callbackReshape( int inW, int inH ) {
    // force original size
    glutReshapeWindow( w, h );

    // note that we also need to check this in the main loop above
    // because sometimes callbackReshape isn't getting called (maybe
    // after releasing the mouse near the end of a resize)
    }

    


