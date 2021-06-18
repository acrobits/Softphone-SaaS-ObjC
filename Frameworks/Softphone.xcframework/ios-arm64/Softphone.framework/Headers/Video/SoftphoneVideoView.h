//
//  SoftphoneVideoView.h
//  softphone
//
//  Created by Jiri Kral on 11/17/11.
//  Copyright (c) 2011 Acrobits. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ali/ali_string.h"
#import "Softphone/EventHistory/CallEvent.h"

@class SoftphoneVideoView;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@protocol VideoViewDelegate<NSObject>
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@optional
-(void) onNewFrame: (SoftphoneVideoView *) view;
-(void) onFrameSizeChanged: (SoftphoneVideoView *) view;

@end

struct Impl;
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@interface SoftphoneVideoView : UIView
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
{
    id<VideoViewDelegate>   __weak _delegate;

@private
    Impl * _impl;
    Softphone::EventHistory::CallEvent::Pointer _call;
}

@property (nonatomic,weak) IBOutlet id<VideoViewDelegate> delegate;
@property (nonatomic,assign) unsigned int sessionId;
@property (nonatomic,readonly) CGSize videoFrameSize;
@property (nonatomic,readonly) CGFloat videoRatio;
@property (nonatomic,readonly) Softphone::EventHistory::CallEvent::Pointer call;

-(id) initWithCall:(Softphone::EventHistory::CallEvent::Pointer)call;
-(void) reset;
-(void) destroy;

@end
