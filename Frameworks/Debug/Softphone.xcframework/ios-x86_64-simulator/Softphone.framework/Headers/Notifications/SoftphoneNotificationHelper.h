//
//  SoftphoneLocationNotification.h
//  softphone
//
//  Created by Ghazanfar Ali on 21/09/2016.
//
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, NotificationActivationMode)
{
    NotificationActivationModeForeground, // activates the application in the foreground
    NotificationActivationModeBackground  // activates the application in the background, unless it's already in the foreground
};

typedef NS_ENUM(NSUInteger, NotificationActionBehavior)
{
    NotificationActionBehaviorDefault,        // the default action behavior
    NotificationActionBehaviorTextInput       // system provided action behavior, allows text input from the user
};

typedef NS_ENUM(NSUInteger, NotificationActionContext)
{
    NotificationActionContextDefault,  // the default context of a notification action
    NotificationActionContextMinimal   // the context of a notification action when space is limited
};

typedef NS_ENUM(NSUInteger, NotificationSoundType)
{
    NotificationSoundNameNone,      // no sound
    NotificationSoundNameDefault,  // the default sound to be used
    NotificationSoundNameCustom   // the user provided sound to be used
};

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@interface UserNotificationAction : NSObject
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

@property (nullable, nonatomic, strong) NSString *identifier;
@property (nonatomic, strong) NSString *title;
@property (nonatomic, strong) NSString *actionButtonTitle;
@property (nonatomic, strong) NSString *placeHolderText;
@property (nonatomic, readwrite) NotificationActivationMode activationMode;
@property (nonatomic, readwrite) BOOL authenticationRequired;
@property (nonatomic, readwrite) BOOL destructive;
@property (nonatomic, readwrite) NotificationActionBehavior behavior;

@end

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@interface UserNotificationCategory : NSObject
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

@property (nonatomic, strong) NSString *identifier;
@property (nonatomic, strong) NSArray *actions;
@property (nonatomic, readwrite) NotificationActionContext context;
@property (nonatomic, strong) NSString *categorySummaryFormat;
@property (nonatomic, strong) NSString *hiddenPreviewsBodyPlaceholder;

@end

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@interface SoftphoneLocalNotification : NSObject
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

@property (nonatomic, strong) NSString *identifier;
@property (nonatomic, strong) NSString *title;
@property (nonatomic, strong) NSString *alertBody;
@property (nonatomic, strong) NSString *alertAction;
@property (nonatomic, strong) NSString *category;
@property (nonatomic, strong) NSString *threadIdentifier;
@property (nonatomic, strong) NSString *summaryArgument;
@property (nonatomic, assign) NSUInteger summaryArgumentCount;
@property (nullable, nonatomic, copy) NSDictionary *userInfo;

@property (nonatomic, readwrite) NotificationSoundType soundType;
@property (nullable, nonatomic, strong) NSString *soundName;

@property (nullable, nonatomic, readwrite) NSNumber *applicationIconBadgeNumber;

@end

//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@interface SoftphoneNotificationHelper : NSObject
//*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

+(nonnull id)sharedHelper;

-(void)registerNotificationCategories:(nonnull NSArray *)categories;

-(void)cancelAllNotifications;
-(void)cancelNotificationWithIdentifier:(nullable NSString *)notificationId;
-(void)cancelNotificationsWithAttribute: (NSString *) attribute value:(nullable NSString*) value completion: (nullable void (^)(BOOL)) completion;
-(void)findNotificationsWithAttribute: (NSString *) attribute value:(nullable NSString*) value completion: (nullable void (^)(NSArray*)) completion;


-(void)presentLocalNotification:(nonnull SoftphoneLocalNotification *)notification;

-(void)notificationStatusWithCompletionHandler:(void(^)(bool))handler;

@end

NS_ASSUME_NONNULL_END
