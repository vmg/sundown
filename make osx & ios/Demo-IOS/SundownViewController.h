//
//  SundownViewController.h
//  Demo-IOS
//
//  Created by Dominik Pich on 19.06.12.
//  Copyright (c) 2012 sundown. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SundownViewController : UIViewController

@property(weak) IBOutlet UIWebView *webView;
- (IBAction)openAndConvert;
@end
