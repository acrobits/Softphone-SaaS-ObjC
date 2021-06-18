//
//  SoftphoneVideoPreview.h
//  softphone
//
//  Created by Jiri Kral on 07/23/13.
//  Copyright (c) 2013 Acrobits. All rights reserved.
//

#import <UIKit/UIKit.h>

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
@interface SoftphoneVideoPreview : UIView
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
{
@private
    CGRect mPreviewArea;
    bool   mHavePreviewLayer;
}

@property(nonatomic,assign) CGRect previewArea;

-(void) updatePositionAndOrientation;

@end
