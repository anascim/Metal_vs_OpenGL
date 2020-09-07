//
//  Types.swift
//  MetalGame
//
//  Created by Alex Nascimento on 19/01/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

import simd

typealias float4 = SIMD4<Float>
typealias float3 = SIMD3<Float>

public struct Uniforms {
    var model: float4x4
    var view: float4x4
    var projection: float4x4
}

struct Lighting {
    var direction: SIMD3<Float>
    var color: SIMD3<Float>
    var camPos: SIMD3<Float>
}

struct Material {
    var ambient: SIMD3<Float>
    var diffuse: SIMD3<Float>
    var specular: SIMD3<Float>
    var shininnes: Float
}
