#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>



void cleanUpAtExit() {
    printf( "Exiting...\n" );
    
    freeFrameDrawer();    
    }


int main( int inNumArgs, char **inArgs ) {
    
    glutInit( &inNumArgs, inArgs );
    
    int w = 320;
    int h = 480;
    
    initFrameDrawer( w, h );
    
    atexit( cleanUpAtExit );


    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    

    glutInitWindowSize( w, h );
    glutCreateWindow( "game1" );

    glutMainLoop();


    return 0;
    
    }


