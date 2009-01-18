/*
 *     Generated by class-dump 3.1.1.
 *
 *     class-dump is Copyright (C) 1997-1998, 2000-2001, 2004-2006 by Steve Nygard.
 */

//#import "NSObject.h"

@class NSMutableDictionary, NSMutableSet, NSString, NSTimer;

@interface NetworkController : NSObject
{
	struct __SCDynamicStore *_store;	// 4 = 0x4
	NSString *_domainName;	// 8 = 0x8
	NSMutableDictionary *_reachabilityObjects;	// 12 = 0xc
	NSMutableSet *_reachableHosts;	// 16 = 0x10
	unsigned int _waitingForDialToFinish:1;	// 20 = 0x14
	unsigned int _checkedNetwork:1;	// 20 = 0x14
	unsigned int _isNetworkUp:1;	// 20 = 0x14
	unsigned int _isFatPipe:1;	// 20 = 0x14
	unsigned int _edgeRequested:1;	// 20 = 0x14
	NSTimer *_notificationTimer;	// 24 = 0x18
}

+ (id)sharedInstance;	// IMP=0x30b96e29
- (void)dealloc;	// IMP=0x30b96e81
- (id)init;	// IMP=0x30b97cb5
- (BOOL)isNetworkUp;	// IMP=0x30b96fad
- (BOOL)isFatPipe;	// IMP=0x30b97215
- (id)domainName;	// IMP=0x30b97fb1
- (void)createReacher:(id)fp8;	// IMP=0x30b9739d
- (BOOL)isHostReachable:(id)fp8;	// IMP=0x30b974d1
- (id)primaryEthernetAddressAsString;	// IMP=0x30b975d5
- (void)registerCTServerRunLoopSource;	// IMP=0x30b976d5
- (id)IMEI;	// IMP=0x30b97a55
- (id)edgeInterfaceName;	// IMP=0x30b97a5f
- (BOOL)isEdgeUp;	// IMP=0x30b97ab9
- (void)bringUpEdge;	// IMP=0x30b97b09
- (void)keepEdgeUp;	// IMP=0x30b97b55

@end

