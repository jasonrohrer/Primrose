


class GridSpace {
        
    public:
        
        // constructs w/ center location on screen
        GridSpace( int inX, int inY );
        

        // draws onto screen and steps animations
        void draw();
        

        int mX, mY;


        enum NeighborDirections { top=0, right, bottom, left 
            };
        
        
        GridSpace *mNeighbors[4];
        
    };

        
        
