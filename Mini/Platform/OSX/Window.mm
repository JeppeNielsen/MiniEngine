//
//  Window.cpp
//  MiniOSX
//
//  Created by Jeppe Nielsen on 04/01/2019.
//  Copyright © 2019 Jeppe Nielsen. All rights reserved.
//

#include "Window.hpp"
#import <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
#include <iostream>

using namespace Mini;

static Mini::Window* engineWindow;
static int globalWindowWidth;
static int globalWindowHeight;

@interface OSXView : NSOpenGLView <NSWindowDelegate>
{
    NSTimer *renderTimer;
}

- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
-(NSMenuItem*) createMenuItem: (NSMenu*)menu withText:(NSString*)text withObject:(void*)object withShortCut:(NSString*)shortCut;
-(void) removeMenuItem: (NSMenu*)menu withItem:(void*)item;
-(void) menuItemClicked:(id)sender;
-(void)viewDidChangeBackingProperties;
@end


@implementation OSXView

-(void) drawRect: (NSRect) bounds
{
    if (engineWindow->onUpdate()) {
        exit(0);
    }
}

- (void)prepareOpenGL {
    [super prepareOpenGL];
    
    
    NSTimeInterval timeInterval = 0.005;
    
    renderTimer =  [ NSTimer scheduledTimerWithTimeInterval:timeInterval
                                                      target:self
                                                    selector:@selector( updateGLView: )
                                                    userInfo:nil repeats:YES ];// retain ];
    [ [ NSRunLoop currentRunLoop ] addTimer:renderTimer
                                    forMode:NSEventTrackingRunLoopMode ];
    [ [ NSRunLoop currentRunLoop ] addTimer:renderTimer
                                    forMode:NSModalPanelRunLoopMode ];
    GLint defaultFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    
    //Window::Framebuffer = (uint)defaultFBO;
    
    NSApplication* app = [NSApplication sharedApplication];
    NSMenu* menu = [[NSMenu alloc] initWithTitle: @""];

    {
        [menu addItemWithTitle:@"" action:nil keyEquivalent:@""];
    }

    [app setMainMenu:menu];
    
    engineWindow->onInitialize();
}

- (void) updateGLView:(NSTimer *)timer
{
    [self drawRect: [self frame]];
}

- (void) mouseDown:(NSEvent*)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    engineWindow->inputDevice.SetTouch(0, true, Vector2(location.x, location.y));
}

-(void) mouseUp:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    engineWindow->inputDevice.SetTouch(0, false, Vector2(location.x, location.y));
}

-(void) mouseDragged:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    //OSXWindowCreator::Instance()->Move(0, location.x, location.y);
    engineWindow->inputDevice.SetTouchPosition(0, Vector2(location.x, location.y));
}

-(void)mouseMoved:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    
    engineWindow->inputDevice.SetTouch(0, false, Vector2(location.x, location.y));
    engineWindow->inputDevice.SetTouch(1, false, Vector2(location.x, location.y));
    engineWindow->inputDevice.SetTouch(2, false, Vector2(location.x, location.y));
}

- (void)scrollWheel:(NSEvent *)theEvent {
    engineWindow->inputDevice.SetScroll(theEvent.scrollingDeltaY);
}

-(NSPoint) convertLocation: (NSPoint) point {
    return point;
}

-(void)rightMouseDown:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    engineWindow->inputDevice.SetTouch(1, true, Vector2(location.x, location.y));
}

-(void)rightMouseUp:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    engineWindow->inputDevice.SetTouch(1, false, Vector2(location.x, location.y));
}

-(void)rightMouseDragged:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    engineWindow->inputDevice.SetTouchPosition(1, Vector2(location.x, location.y));
}

-(void)otherMouseDown:(NSEvent *)theEvent {
    if (theEvent.type == NSOtherMouseDown) {
        NSPoint location = [self convertLocation:theEvent.locationInWindow];
        engineWindow->inputDevice.SetTouch(2, true, Vector2(location.x, location.y));
    }
}

-(void)otherMouseUp:(NSEvent *)theEvent {
    if (theEvent.type == NSOtherMouseUp) {
        NSPoint location = [self convertLocation:theEvent.locationInWindow];
        engineWindow->inputDevice.SetTouch(2, false, Vector2(location.x, location.y));
    }
}

-(void)otherMouseDragged:(NSEvent *)theEvent {
    if (theEvent.type == NSOtherMouseDragged) {
        NSPoint location = [self convertLocation:theEvent.locationInWindow];
        engineWindow->inputDevice.SetTouchPosition(2, Vector2(location.x, location.y));
    }
}

-(void)keyDown:(NSEvent *)theEvent {
    ModifierKey modifierKey = ModifierKey::None;

    NSUInteger flags = [[NSApp currentEvent] modifierFlags];
    if ((flags & NSShiftKeyMask)) {
        modifierKey = ModifierKey::Shift;
    } else if ((flags & NSCommandKeyMask)) {
        modifierKey = ModifierKey::Command;
    } else if ((flags & NSAlternateKeyMask)) {
        modifierKey = ModifierKey::Alt;
    } else if ((flags & NSControlKeyMask)) {
        modifierKey = ModifierKey::Ctrl;
    }

    unichar cd = [[theEvent characters] characterAtIndex:0];
    NSString* str = [NSString stringWithCharacters:&cd length:1];
    engineWindow->inputDevice.SetButton([str UTF8String], true, modifierKey);
}

-(void)keyUp:(NSEvent *)theEvent {
    unichar cd = [[theEvent characters] characterAtIndex:0];
    NSString* str = [NSString stringWithCharacters:&cd length:1];
    engineWindow->inputDevice.SetButton([str UTF8String], false, ModifierKey::None);
}

//std::map<NSMenuItem*, AppMenu*> menuItemToAppMenu;

-(NSMenuItem*)createMenuItem:(NSMenu *)menu withText:(NSString*)text withObject:(void*)object withShortCut:(NSString*)shortCut
{
    //NSMenuItem* menuItem = [[menu addItemWithTitle:text action:@selector(menuItemClicked:) keyEquivalent:shortCut] retain];
    //menuItemToAppMenu[menuItem] = (AppMenu*)object;
    //return menuItem;
    return NULL;
}

-(void)removeMenuItem:(NSMenu *)menu withItem:(void*)item {
    /*for(auto it : menuItemToAppMenu) {
        if (it.second == (AppMenu*)item){
            [menu removeItem:it.first];
            menuItemToAppMenu.erase(menuItemToAppMenu.find(it.first));
            break;
        }
    }
    */
}

-(void)menuItemClicked:(id)sender
{
    NSMenuItem* menuItem = (NSMenuItem*)sender;
    //menuItemToAppMenu[menuItem]->Clicked();
}

- (void)viewDidChangeBackingProperties {
    NSScreen* scr = [[self window] screen];
    float scalingFactor = [scr backingScaleFactor];
    //Engine::Context().ScreenScalingFactor = scalingFactor;
}

@end

@interface AppDelegate : NSResponder <NSApplicationDelegate, NSWindowDelegate>
{
    NSTimer* renderTimer;
    NSWindow* window;
    OSXView* view;
}

@end


@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    int windowWidth = 1280;//OSXWindowCreator::Instance()->Width;
    int windowHeight = 920;// OSXWindowCreator::Instance()->Height;
    bool isFullScreen = false;//OSXWindowCreator::Instance()->FullScreen;
    
    NSRect mainScreenSize = [[NSScreen mainScreen] frame];
    
    CGFloat xPos = mainScreenSize.size.width/2 - windowWidth/2;
    CGFloat yPos = mainScreenSize.size.height/2 - windowHeight/2;
    
    if (isFullScreen) {
        //windowWidth = mainScreenSize.size.width + [[[NSScreen screens] objectAtIndex:1] frame].size.width;
        //windowHeight = mainScreenSize.size.height;
        xPos = 0;
        yPos = 0;
        //OSXWindowCreator::Instance()->Width = windowWidth;
        //OSXWindowCreator::Instance()->Height = windowHeight;
    }
    
    NSRect frame = NSMakeRect(xPos, yPos, windowWidth, windowHeight);
    NSUInteger styleMask = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask;
    if (isFullScreen) {
        styleMask |=NSFullScreenWindowMask;
    }
    NSRect rect = [NSWindow contentRectForFrameRect:frame styleMask:styleMask];
    window =  [[NSWindow alloc] initWithContentRect:rect styleMask:styleMask backing: NSBackingStoreBuffered    defer:false];
    [window setDelegate:self];
    [window setAcceptsMouseMovedEvents:YES];
    
    [ NSApp setDelegate:self ];
    renderTimer = nil;
    
    GLuint attributes[] = {
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAWindow,
       // NSOpenGLPFAAccelerated,
        //NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 16,
        NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAAccumSize, 0,
        0
    };
    
    NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:(NSOpenGLPixelFormatAttribute*) attributes];
    if (!fmt)
        NSLog(@"No OpenGL pixel format");
    

    view = [[OSXView alloc]init];
    [view setWantsBestResolutionOpenGLSurface:YES];
    
    float scalingFactor = [[NSScreen mainScreen] backingScaleFactor];
    //Engine::Context().ScreenScalingFactor = scalingFactor;
    
    [window setContentView:view ];
    
    [window makeFirstResponder:view];
    
    [view setPixelFormat:fmt];

    NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |
                         NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSOtherMouseDownMask | NSOtherMouseUpMask);

    NSTrackingArea *area = [[NSTrackingArea alloc] initWithRect:[view bounds]
                                                    options:options
                                                      owner:window
                                                   userInfo:nil];
    [view addTrackingArea:area];
    
    [window makeKeyAndOrderFront: window];
    
    [window orderFront:window];
}

- (void)windowWillClose:(NSNotification *)notification
{
    exit(0);
}

- (void)windowDidResize:(NSNotification *)notification {
    globalWindowWidth = view.bounds.size.width;
    globalWindowHeight = view.bounds.size.height;
    engineWindow->onScreenSizeChanged(globalWindowWidth, globalWindowHeight);
}

@end

void Window::StartLoop(OnInitialize onInitialize, OnUpdate onUpdate, OnScreenSizeChanged onScreenSizeChanged) {

    inputDevice.Initialize(3);
    engineWindow = this;

    this->onInitialize = onInitialize;
    this->onUpdate = onUpdate;
    this->onScreenSizeChanged = onScreenSizeChanged;
    
    [NSApplication sharedApplication];
    AppDelegate *appDelegate = [[AppDelegate alloc] init];
    [NSApp setDelegate:appDelegate];
    [NSApp run];
}

void Window::PreRender() {
    glViewport(0, 0, globalWindowWidth, globalWindowHeight);
    glClearColor(1, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
}

void Window::PostRender() {
    glFlush();
}
