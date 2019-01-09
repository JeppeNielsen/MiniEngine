//
//  OSXView.h
//  MiniOSX
//
//  Created by Jeppe Nielsen on 09/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <Cocoa/Cocoa.h>


@interface OSXView : NSOpenGLView <NSWindowDelegate>
{
    NSTimer *renderTimer;
    NSWindow* window;
}
+ (OSXView*) getInstance;
- (void) setWindow: (NSWindow*) window;
- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
-(NSMenuItem*) createMenuItem: (NSMenu*)menu withText:(NSString*)text withObject:(void*)object withShortCut:(NSString*)shortCut;
-(void) removeMenuItem: (NSMenu*)menu withItem:(void*)item;
-(void) menuItemClicked:(id)sender;
-(void)viewDidChangeBackingProperties;
-(NSPoint) convertViewLocationToWorldPoint: (NSPoint) point;
@end


