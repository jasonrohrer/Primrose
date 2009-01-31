//
//  main.m
//  testWindowApp
//
//  Created by Jason Rohrer on 12/14/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

//#import <NetworkController.h>



int main(int argc, char *argv[]) {
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    /*
        // try to force the network connection to come up
        char *testURL = "http://www.google.com";
    
    NSString *urlString = [NSString stringWithCString:testURL 
                                           encoding:NSASCIIStringEncoding];
    
        NSURL *OTURL = [NSURL URLWithString:urlString];
        NSError *error;
        
        NSString *source = [ NSString stringWithContentsOfURL:OTURL
                             encoding:NSASCIIStringEncoding error:&error];
        
    const char *sourceString = [ source cStringUsingEncoding:NSASCIIStringEncoding ];
        printf( "%s,\n", sourceString );
    */
/*
    // force network to start
    if( !( [[NetworkController sharedInstance]isNetworkUp] ) ) {
        printf( "Network not up...\n" );
		if( ! [[NetworkController sharedInstance]isEdgeUp] ) {
            printf( "Edge not up... bringing it up\n" );
		    
			[[NetworkController sharedInstance]keepEdgeUp];
			[[NetworkController sharedInstance]bringUpEdge];
            }
        else {
            printf( "Edge already up\n" );
        }
    }
    else {
        printf( "Network already up\n" );
    }
*/
    


    //printf( "Arg 0 = %s\n", argv[0] );
    
    // arg 0 is the path to the app executable
    char *appDirectoryPath = strdup( argv[ 0 ] );
    
    //printf( "Mac:  app path %s\n", appDirectoryPath );
    
    char *bundleName = "Primrose.app";
    
    char *appNamePointer = strstr( appDirectoryPath, bundleName );
    
    if( appNamePointer != NULL ) {
        // terminate full app path to get bundle directory
        appNamePointer[ strlen( bundleName ) ] = '\0';
        
        printf( "Mac: changing working dir to %s\n", appDirectoryPath );
        chdir( appDirectoryPath );
    }
    
    free( appDirectoryPath );
    
    
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
