//
//  Softphone_iOS.h
//  libsoftphone
//
//  Created by Stanislav Kutil on 06/09/16.
//
//

#import <Foundation/Foundation.h>
#import <CallKit/CallKit.h>
#import <Intents/Intents.h>
#import "Softphone/EventHistory/CallEvent.h"

@protocol Softphone_iOS_Delegate <NSObject>

@optional

/** @brief callback allowing you to modify CXCallUpdate before it's reported to the system
  * @param callUpdate The original CXCallUpdate as set by the SDK
  * @param call The call event to which the CXCallUpdate relates
  */
-(void) adjustCxCallUpdate: (CXCallUpdate*) callUpdate forCall: (Softphone::EventHistory::CallEvent::Pointer) call;

/** @brief callback allowing you to modify SDK created or updated CXProviderConfiguration or create a new one
 * @param configuration The original CXProviderConfiguration as created/modified by the SDK
 * @return the new or updated CXProviderConfiguration. Null can be returned if you haven't created a new object and just modified the original one
 */
-(CXProviderConfiguration*) adjustCxProviderConfiguration: (CXProviderConfiguration*) configuration;

@end

@interface Softphone_iOS <CXCallHandlerDelegate> : NSObject

+(instancetype) sharedInstance;

/** @brief helper function to handle start call intents
 * @param intent The intent you receive in
 *             - (BOOL)application:(UIApplication *)application continueUserActivity:(nonnull NSUserActivity *)userActivity restorationHandler:(nonnull void (^)(NSArray * _Nullable))restorationHandler
 * @return YES we handled the intent
 * @return NO we did not handle the intent
 */
-(BOOL) processIntent: (INIntent*) intent;

/** @brief accessor for callkit configuration
 * @return CXProviderConfiguration you can update and pass back to updateCallKitConfiguration
 */
-(CXProviderConfiguration*) callKitConfiguration;

/** @brief allows to change the callkit configuration
 * @param configuration a new or updated CXProviderConfiguration
 */
-(void) updateCallKitConfiguration: (CXProviderConfiguration*) configuration;

/** @brief helper to set the icon for the callkit - it should be an image file in your app bundle (40ptx40pt)
 * @return YES if the icon was set
 * @return NO if the file cannot be found or the callkit is not enabled
 */
-(BOOL) setCallKitIcon: (NSString*) imageName;

/** @brief helper function to find out if the callkit is available
 * @return YES the callkit is available and enabled
 * @return NO the callkit is not available (pre iOS 10) or is disabled by the configuration (<useCallKit>0</useCallKit>)
 */
-(BOOL) isCallKitAvailable;


/** @brief allows you to asynchronously update the callkit calls
 * @param call The call event to which the CXCallUpdate relates
 * @param update the new set of properties
 */

-(void) reportCall: (Softphone::EventHistory::CallEvent::Pointer) call updated: (CXCallUpdate*) update;

/** @brief the delagate allows receiving iOS specific callbacks. E.g. adjusting the CxCallUpdate object before it's passed to the callkit */

@property(nonatomic, weak) id<Softphone_iOS_Delegate> delegate;

@end
