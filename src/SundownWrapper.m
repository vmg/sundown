//
//  SundownWrapper.m
//  sundown
//
//  Created by Dominik Pich on 19.06.12.
//  Copyright (c) 2012 sundown. All rights reserved.
//

#import "SundownWrapper.h"

#include "markdown.h"
#include "html.h"
#include "buffer.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

@implementation SundownWrapper

+ (NSString*)convertMarkdownFileAtPath:(NSString*)path {
    return [self convertMarkdownString:[NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:nil]];
}

+ (NSString*)convertMarkdownFileAtURL:(NSURL*)url {
    return [self convertMarkdownString:[NSString stringWithContentsOfURL:url encoding:NSUTF8StringEncoding error:nil]];
}

+ (NSString*)convertMarkdownString:(NSString *)markdownString {
    if(!markdownString.length) {
        fprintf(stderr,"Empty string passed into conversion method.");
        return nil;
    }
    
    NSData *data = [markdownString dataUsingEncoding:NSUTF8StringEncoding];
    
	struct buf *ob;
    
	struct sd_callbacks callbacks;
	struct html_renderopt options;
	struct sd_markdown *markdown;
       
	/* performing markdown parsing */
	ob = bufnew(data.length);
    
	sdhtml_renderer(&callbacks, &options, 0);
	markdown = sd_markdown_new(0, 16, &callbacks, &options);
	sd_markdown_render(ob, data.bytes, data.length, markdown);
	sd_markdown_free(markdown);

    NSString *string = nil;
    if (!ob->size) {
        fprintf(stderr,"Conversion of input string resulted in no html");
	}
    else {
        /* writing the result to string*/
        string = [[NSString alloc] initWithBytes:ob->data length:ob->size encoding:NSUTF8StringEncoding];
    }
    
	/* cleanup */
	bufrelease(ob);
    
	return string;
}

@end