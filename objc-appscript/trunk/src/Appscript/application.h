//
//  application.h
//  aem
//
//  Copyright (C) 2007 HAS
//

#import <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import <Carbon/Carbon.h>
#import "codecs.h"
#import "SendThreadSafe.h"
#import "event.h"
#import "utils.h"


/**********************************************************************/
// Application class

@interface AEMApplication : NSObject {
	AEMTargetType targetType;
	id targetData;
	NSAppleEventDescriptor *addressDesc;
	id defaultCodecs;
	AETransactionID transactionID;
	
	AEMCreateProcPtr createProc;
	AEMSendProcPtr sendProc;
	Class eventClass;
}

// Utility class methods

// Find application by creator code, bundle ID and/or file name
// Convenience wrapper for LSFindApplicationForInfo()

+ (NSURL *)findApplicationForCreator:(OSType)creator		// use kLSUnknownCreator if none
							bundleID:(NSString *)bundleID	// use nil if none
								name:(NSString *)name		// use nil if none
							   error:(NSError **)error;

// Find application by file name. Full path is also acceptable; .app suffix is optional.

+ (NSURL *)findApplicationForName:(NSString *)name error:(NSError **)error;

// Get Unix process ID of first process launched from specified application

+ (pid_t)findProcessIDForApplication:(NSURL *)fileURL error:(NSError **)error;


// Check if specified application is running

// Does a local process launched from the specified application file exist?
// e.g. [NSURL fileURLWithPath: @"/Applications/iCal.app"]
// Returns false if process doesn't exist or file isn't found.

+(BOOL)processExistsForFileURL:(NSURL *)fileURL;


// Is there a local application process with the given Unix process id?

+(BOOL)processExistsForPID:(pid_t)pid;


// Does an application process specified by the given eppc:// URL exist?
// e.g. [NSURL URLWithString: @"eppc://user:pass@0.0.0.1/TextEdit"]
// Returns false if process doesn't exist, or if access isn't allowed.

+(BOOL)processExistsForEppcURL:(NSURL *)eppcURL;


// Does an application process specified by the given AEAddressDesc exist?
// Returns false if process doesn't exist, or if access isn't allowed.

+(BOOL)processExistsForDescriptor:(NSAppleEventDescriptor *)desc;




// Launch an application

+ (pid_t)launchApplication:(NSURL *)fileURL
					 event:(NSAppleEventDescriptor *)firstEvent
					 flags:(LaunchFlags)launchFlags
					 error:(NSError **)error;

// convenience shortcuts for the above

+ (pid_t)launchApplication:(NSURL *)appFileURL error:(NSError **)error;

+ (pid_t)runApplication:(NSURL *)appFileURL error:(NSError **)error;

+ (pid_t)openDocuments:(id)files inApplication:(NSURL *)appFileURL error:(NSError **)error;

/*
 * make AEAddressDescs
 *
 * Note: addressDescForLocalApplication:error: will start application if not already running
 */

+ (NSAppleEventDescriptor *)addressDescForLocalApplication:(NSURL *)fileURL error:(NSError **)error;

+ (NSAppleEventDescriptor *)addressDescForLocalProcess:(pid_t)pid;

+ (NSAppleEventDescriptor *)addressDescForRemoteProcess:(NSURL *)eppcURL;

+ (NSAppleEventDescriptor *)addressDescForCurrentProcess;


/*******/

// designated initialiser; clients shouldn't call this directly but use one of the following methods

- (id)initWithTargetType:(AEMTargetType)targetType_ data:(id)targetData_ error:(NSError **)error;


/*
 * clients should call one of the following methods to initialize AEMApplication object
 *
 * Note: if an error occurs when finding/launching an application by name/bundle ID/file URL, additional
 * error information may be returned via the error argument.
 */

- (id)initWithName:(NSString *)name error:(NSError **)error;

- (id)initWithBundleID:(NSString *)bundleID error:(NSError **)error;

- (id)initWithURL:(NSURL *)url error:(NSError **)error;

- (id)initWithPID:(pid_t)pid;

- (id)initWithDescriptor:(NSAppleEventDescriptor *)desc;

// shortcuts for above

- (id)initWithName:(NSString *)name;

- (id)initWithBundleID:(NSString *)bundleID;

- (id)initWithURL:(NSURL *)url;

// comparison, hash support

- (BOOL)isEqual:(id)object;

- (AEMTargetType)targetType; // used by -isEqual:

- (id)targetData; // used by -isEqual:


// clients can call following methods to modify standard create/send behaviours

- (void)setCreateProc:(AEMCreateProcPtr)createProc_;

- (void)setSendProc:(AEMSendProcPtr)sendProc_;

- (void)setEventClass:(Class)eventClass_;


// create new AEMEvent object

- (id)eventWithEventClass:(AEEventClass)classCode
				  eventID:(AEEventID)code
				 returnID:(AEReturnID)returnID
				   codecs:(id)codecs;

- (id)eventWithEventClass:(AEEventClass)classCode
				  eventID:(AEEventID)code
				 returnID:(AEReturnID)returnID;

- (id)eventWithEventClass:(AEEventClass)classCode
				  eventID:(AEEventID)code
				   codecs:(id)codecs;

- (id)eventWithEventClass:(AEEventClass)classCode
				  eventID:(AEEventID)code;


// reconnect to a local application originally specified by name, bundle ID or file URL

- (BOOL)reconnect;

- (BOOL)reconnectWithError:(NSError **)error;


// transaction support

- (BOOL)beginTransactionWithError:(NSError **)error;

- (BOOL)beginTransactionWithSession:(id)session error:(NSError **)error;

- (BOOL)endTransactionWithError:(NSError **)error;

- (BOOL)abortTransactionWithError:(NSError **)error;



@end

