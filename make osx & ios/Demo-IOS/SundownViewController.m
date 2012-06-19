//
//  SundownViewController.m
//  Demo-IOS
//
//  Created by Dominik Pich on 19.06.12.
//  Copyright (c) 2012 sundown. All rights reserved.
//

#import "SundownViewController.h"
#import "SundownWrapper.h"

@interface SundownViewController ()

@end

@implementation SundownViewController

@synthesize webView;

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
        return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
    } else {
        return YES;
    }
}

- (IBAction)openAndConvert {
    id file = [[NSBundle mainBundle] URLForResource:@"README" withExtension:@"markdown"];
    id html = [SundownWrapper convertMarkdownFileAtURL:file];
    
    if(!html) {
        html = [@"<b>failed to convert " stringByAppendingString:[file description]];
    }
    
   [self.webView loadHTMLString:html baseURL:nil];
}
@end
