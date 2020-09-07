//
//  ModelLoader_Wrapper.h
//  Graphics_APIs
//
//  Created by Alex Nascimento on 04/09/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ModelLoader_Wrapper : NSObject

@property (nonatomic, readonly, nonnull) float *vertexData;

@property (nonatomic, readonly) NSInteger dataLength;

@property (nonatomic, readonly) NSInteger vertexLength;

@property (nonatomic, readonly) NSInteger vertexCount;

- (instancetype _Nonnull )init:(NSString*)modelName;

@end
