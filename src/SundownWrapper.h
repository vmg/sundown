//
//  SundownWrapper.h
//  sundown
//
//  Created by Dominik Pich on 19.06.12.
//  Copyright (c) 2012 sundown. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SundownWrapper : NSObject

/* converts the markdown in the file to an html string*/
+ (NSString*)convertMarkdownFileAtPath:(NSString*)path;

/* converts the markdown at the url to an html string*/
+ (NSString*)convertMarkdownFileAtURL:(NSURL*)url;

/* converts the given markdown string to an html string*/
+ (NSString*)convertMarkdownString:(NSString*)markdown;

@end
