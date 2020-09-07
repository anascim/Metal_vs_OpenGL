//
//  MathUtilities.swift
//  MetalGame
//
//  Created by Alex Nascimento on 16/01/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

// This file was heavily based by this example by Warren More:
// https://github.com/metal-by-example/modern-metal/blob/master/modern-metal/modern-metal/MathUtilities.swift

import simd

let PI = Float.pi

extension float4x4 {
    init(scaleBy s: Float) {
        self.init(float4(s, 0, 0, 0),
                  float4(0, s, 0, 0),
                  float4(0, 0, s, 0),
                  float4(0, 0, 0, 1))
    }
    
    init(scaleBy s: float3) {
        self.init(float4(s.x,    0,    0, 0),
                  float4(   0, s.y,    0, 0),
                  float4(   0,    0, s.z, 0),
                  float4(   0,    0,    0, 1))
    }
    
    init(rotationAbout axis: float3, by radians: Float) {
        
        let c = cos(radians);
        let s = sin(radians);
        
        let nAxis = normalize(axis)
        
        let X = float4(nAxis.x * nAxis.x + (1 - nAxis.x * nAxis.x) * c,
                              nAxis.x * nAxis.y * (1 - c) - nAxis.z * s,
                              nAxis.x * nAxis.z * (1 - c) + nAxis.y * s,
                              0.0)
        let Y = float4(nAxis.x * nAxis.y * (1 - c) + nAxis.z * s,
                              nAxis.y * nAxis.y + (1 - nAxis.y * nAxis.y) * c,
                              nAxis.y * nAxis.z * (1 - c) - nAxis.x * s,
                              0.0)
        let Z = float4(nAxis.x * nAxis.z * (1 - c) - nAxis.y * s,
                              nAxis.y * nAxis.z * (1 - c) + nAxis.x * s,
                              nAxis.z * nAxis.z + (1 - nAxis.z * nAxis.z) * c,
                              0.0)
        let W = float4(0.0, 0.0, 0.0, 1.0)
        
        self.init(X, Y, Z, W)
    }
    
    init(translationBy t: float3) {
        self.init(float4(   1,    0,    0, 0),
                  float4(   0,    1,    0, 0),
                  float4(   0,    0,    1, 0),
                  float4(t.x, t.y, t.z, 1))
    }
    
    init(perspectiveProjectionFov fovRadians: Float, aspectRatio aspect: Float, nearZ: Float, farZ: Float) {
        let yScale = 1 / tan(fovRadians * 0.5)
        let xScale = yScale / aspect
        let zRange = farZ - nearZ
        let zScale = -(farZ + nearZ) / zRange
        let wzScale = -2 * farZ * nearZ / zRange
        
        self.init(float4(xScale,      0,      0,  0),
                  float4(     0, yScale,      0,  0),
                  float4(     0,      0, zScale, -1),
                  float4(     0,      0, wzScale, 0))
    }
}
