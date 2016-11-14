//
//  GameViewController.m
//  yuusha-ios
//
//  Created by jukka on 16/7/30.
//  Copyright © 2016年 jukka. All rights reserved.
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>

#include "seal.h"


@interface GameViewController () {

}
@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;

@end

@implementation GameViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
        return;
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self setupGL];
}

- (void)viewDidLayoutSubviews
{
    GLKView* view = (GLKView*)self.view;
    [view bindDrawable];

    CGSize viewSize = [view bounds].size;
//    CGRect screenRect = [[UIScreen mainScreen] bounds];

    CGFloat scaleFactor = [view contentScaleFactor];
    struct glview* glview = seal_init_graphics();

//    glview_set_screen_size(glview, screenRect.size.width * scaleFactor,
//                                   screenRect.size.height * scaleFactor);

    glview_set_view_size(glview, viewSize.width * scaleFactor,
                                 viewSize.height * scaleFactor);
    glview_set_fb_size(glview, (int)view.drawableWidth, (int)view.drawableHeight);

    seal_start_game();
}

- (void)dealloc
{    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        [self tearDownGL];
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];
    seal_load_game_config();
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:self.context];
    
    seal_destroy();
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    // update
    seal_update();
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    seal_draw();
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    // Enumerate through all the touch objects.
    NSUInteger touchCount = 0;

    for (UITouch *touch in touches) {
        // Send to the dispatch method, which will make sure the appropriate subview is acted upon.
        CGPoint p = [touch locationInView:self.view];
        touchCount++;
        struct touch_event e = {
            TOUCH_BEGIN,
            p.x,
            [self.view bounds].size.height - p.y,
            false
        };
        seal_touch_event(&e);
    }
    LOGP("touchCount = %d", touchCount);
}

-(void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    NSUInteger touchCount = 0;
    for (UITouch *touch in touches) {
        CGPoint p = [touch locationInView:self.view];
        touchCount++;
        struct touch_event e = {
            TOUCH_MOVE,
            p.x,
            [self.view bounds].size.height - p.y,
            false
        };
        seal_touch_event(&e);
    }
}


-(void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches) {
        CGPoint p = [touch locationInView:self.view];
        struct touch_event e = {
            TOUCH_END,
            p.x,
            [self.view bounds].size.height - p.y,
            false
        };
        seal_touch_event(&e);
    }
}

-(void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    for (UITouch *touch in touches) {
        CGPoint p = [touch locationInView:self.view];
        struct touch_event e = {
            TOUCH_CANCEL,
            p.x,
            [self.view bounds].size.height - p.y,
            false
        };
        seal_touch_event(&e);
    }
}

@end
