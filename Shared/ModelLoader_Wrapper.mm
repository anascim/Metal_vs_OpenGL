//
//  ModelLoader_Wrapper.m
//  Metal_Render
//
//  Created by Alex Nascimento on 04/09/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#import "ModelLoader_Wrapper.h"
#import "ModelLoader.hpp"

@implementation ModelLoader_Wrapper
{
    ModelLoader* modelLoader;
}

- (NSInteger)dataLength
{
   return modelLoader->getDataLength();
}

- (NSInteger) vertexLength
{
    return modelLoader->getVertexLength();
}

- (NSInteger) vertexCount
{
    return modelLoader->getDataLength()/modelLoader->getVertexLength();
}

- (nonnull float*)vertexData
{
    return &modelLoader->getVertexData()[0];
}

- (instancetype)init:(NSString*)modelName
{
    self = [super init];
    const char* cstring = [modelName cStringUsingEncoding:[NSString defaultCStringEncoding]];
    modelLoader = new ModelLoader(cstring);
    return self;
}

@end
