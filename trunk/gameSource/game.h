
// interface for game engine

// called by GLUT or iPhone app wrapper


void initFrameDrawer( int inWidth, int inHeight );


// called at application termination
// good time to save state for next launch
void freeFrameDrawer();


// draw scene into frame using GL function calls
void drawFrame();


// start of pointer press
void pointerDown( float inX, float inY );

// movement with pointer pressed
void pointerMove( float inX, float inY );

// end of pointer press
void pointerUp( float inX, float inY );






