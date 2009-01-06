
// interface for game engine

// called by GLUT or iPhone app wrapper


void initFrameDrawer( int inWidth, int inHeight );
void freeFrameDrawer();


// draw scene into frame using GL function calls
void drawFrame();


// start of pointer press
void pointerDownPoint( float inX, float inY );

// movement with pointer pressed
void pointerMovePoint( float inX, float inY );

// end of pointer press
void pointerUpPoint( float inX, float inY );






