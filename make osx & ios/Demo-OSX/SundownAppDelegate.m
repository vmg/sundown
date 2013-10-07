//
//  SundownAppDelegate.m
//  Demo-OSX
//
//  Created by Dominik Pich on 19.06.12.
//  Copyright (c) 2012 sundown. All rights reserved.
//

#import "SundownAppDelegate.h"
#import "SundownWrapper.h"

@implementation SundownAppDelegate

@synthesize window;
@synthesize webview;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
}

- (IBAction)openAndConvert:(id)sender {
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    openPanel.canChooseFiles = YES;
    openPanel.canChooseDirectories = NO;
    if([openPanel runModal] == NSAlertDefaultReturn) {
        id file = [openPanel.URLs objectAtIndex:0];
        id html = [SundownWrapper convertMarkdownFileAtURL:file];

        if(!html) {
            html = [@"<b>failed to convert " stringByAppendingString:[file description]];
        }
        
        [[self.webview mainFrame] loadHTMLString:html baseURL:nil];

    }
}
@end
