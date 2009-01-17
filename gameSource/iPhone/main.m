//
//  main.m
//  testWindowApp
//
//  Created by Jason Rohrer on 12/14/08.
//  Copyright __MyCompanyName__ 2008. All rights reserved.
//

#import <UIKit/UIKit.h>




// adapted from iNewsgroup GPL'd code
// force a DNS request just to get an address in the cache
// (otherwise, DNS through gethostbyname is spotty!!)

bool resolveHostname( char *inHostname ) {
	NSString * name = [NSString stringWithFormat: @"%s", inHostname ];

	NSLog( @"Resolving for %@", name );
	DNSServiceErrorType error;
	DNSServiceRef service;

	error = DNSServiceQueryRecord( 
        &service, 0 /*no flags*/,
        0 /*all network interfaces */,
        inHostname,
        kDNSServiceType_A, /* we want the ipv4 addy */ 
        kDNSServiceClass_IN, /* internet */
        0, /* no callback */ 
        NULL /* no context */ );

	if( error == kDNSServiceErr_NoError ) {
        //good so far...

		int dns_sd_fd = DNSServiceRefSockFD(service);
        int nfds = dns_sd_fd + 1;
		fd_set readfds;
		struct timeval tv;
        
		FD_ZERO( &readfds );
		FD_SET( dns_sd_fd, &readfds );
		tv.tv_sec = DNS_RESOLV_TIMEOUT;
		tv.tv_usec = 0;
		bool ret = false;
		int result = 
            select( nfds, &readfds, (fd_set*)NULL, (fd_set*)NULL, &tv );

		if( result > 0 ) {
			if( FD_ISSET( dns_sd_fd, &readfds ) ) {
				//remove this if you want to compile in c, not obj-c
				NSLog( @"resolved %s to %@", 
                       inHostname, [ [ NSHost hostWithName: name ] address ] );
				ret = true;
                }
            }
		//clean up and return accordingly
		DNSServiceRefDeallocate( service );
		return ret;

        }
	//clean up....
	DNSServiceRefDeallocate( service );
    
	NSLog( @"dns error: %d", error );
    
	return false;
    }







int main(int argc, char *argv[]) {
    
    // force resolve here so that looking it up later works
    resolveHostname( "hcsoftware.sourceforge.net" );
    

    //printf( "Arg 0 = %s\n", argv[0] );
    
    // arg 0 is the path to the app executable
    char *appDirectoryPath = strdup( argv[ 0 ] );
    
    //printf( "Mac:  app path %s\n", appDirectoryPath );
    
    char *bundleName = "tileGame1.app";
    
    char *appNamePointer = strstr( appDirectoryPath, bundleName );
    
    if( appNamePointer != NULL ) {
        // terminate full app path to get bundle directory
        appNamePointer[ strlen( bundleName ) ] = '\0';
        
        printf( "Mac: changing working dir to %s\n", appDirectoryPath );
        chdir( appDirectoryPath );
    }
    
    free( appDirectoryPath );
    
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
