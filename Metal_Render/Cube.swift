//
//  Cube.swift
//  MetalGame
//
//  Created by Alex Nascimento on 23/01/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

/// Contains static methods that build vertices and indices that describe a cube.
/// Should not be instantiated. Use the static methods instead.

struct Cube {
    
    static func buildVertices(color: SIMD3<Float>) -> [Vertex] {
        return [
            Vertex(position: [-1.0, -1.0, 1.0], normal: [-1.0, -1.0, 1.0]),
            Vertex(position: [-1.0,  1.0, 1.0], normal: [-1.0,  1.0, 1.0]),
            Vertex(position: [-1.0, -1.0,-1.0], normal: [-1.0, -1.0,-1.0]),
            Vertex(position: [-1.0,  1.0,-1.0], normal: [-1.0,  1.0,-1.0]),
            Vertex(position: [ 1.0, -1.0, 1.0], normal: [ 1.0, -1.0, 1.0]),
            Vertex(position: [ 1.0,  1.0, 1.0], normal: [ 1.0,  1.0, 1.0]),
            Vertex(position: [ 1.0, -1.0,-1.0], normal: [ 1.0, -1.0,-1.0]),
            Vertex(position: [ 1.0,  1.0,-1.0], normal: [ 1.0,  1.0,-1.0])
        ]
    }
    /*
    static func buildVertices(frontColor: float4, backColor: float4) -> [Vertex] {
        return [
            Vertex(position: [-1.0, -1.0, 1.0], color: frontColor),
            Vertex(position: [-1.0,  1.0, 1.0], color: frontColor),
            Vertex(position: [-1.0, -1.0,-1.0], color: backColor),
            Vertex(position: [-1.0,  1.0,-1.0], color: backColor),
            Vertex(position: [ 1.0, -1.0, 1.0], color: frontColor),
            Vertex(position: [ 1.0,  1.0, 1.0], color: frontColor),
            Vertex(position: [ 1.0, -1.0,-1.0], color: backColor),
            Vertex(position: [ 1.0,  1.0,-1.0], color: backColor)
        ]
    }
    
    static func buildVertices(leftColor: float4, rightColor: float4) -> [Vertex] {
        return [
            Vertex(position: [-1.0, -1.0, 1.0], color: leftColor),
            Vertex(position: [-1.0,  1.0, 1.0], color: leftColor),
            Vertex(position: [-1.0, -1.0,-1.0], color: leftColor),
            Vertex(position: [-1.0,  1.0,-1.0], color: leftColor),
            Vertex(position: [ 1.0, -1.0, 1.0], color: rightColor),
            Vertex(position: [ 1.0,  1.0, 1.0], color: rightColor),
            Vertex(position: [ 1.0, -1.0,-1.0], color: rightColor),
            Vertex(position: [ 1.0,  1.0,-1.0], color: rightColor)
        ]
    }
    
    static func buildVertices(topColor: float4, bottomColor: float4) -> [Vertex] {
        return [
            Vertex(position: [-1.0, -1.0, 1.0], color: bottomColor),
            Vertex(position: [-1.0,  1.0, 1.0], color: topColor),
            Vertex(position: [-1.0, -1.0,-1.0], color: bottomColor),
            Vertex(position: [-1.0,  1.0,-1.0], color: topColor),
            Vertex(position: [ 1.0, -1.0, 1.0], color: bottomColor),
            Vertex(position: [ 1.0,  1.0, 1.0], color: topColor),
            Vertex(position: [ 1.0, -1.0,-1.0], color: bottomColor),
            Vertex(position: [ 1.0,  1.0,-1.0], color: topColor)
        ]
    }
    
    
    static func buildVertices() -> [Vertex] {
        return [
            Vertex(position: [-1.0, -1.0, 1.0], color: [0.0, 0.0, 1.0, 1.0]), // 0: blue
            Vertex(position: [-1.0,  1.0, 1.0], color: [0.0, 1.0, 1.0, 1.0]), // 1: cian
            Vertex(position: [-1.0, -1.0,-1.0], color: [1.0, 0.0, 1.0, 1.0]), // 2: pink
            Vertex(position: [-1.0,  1.0,-1.0], color: [1.0, 1.0, 1.0, 1.0]), // 3: white
            Vertex(position: [ 1.0, -1.0, 1.0], color: [0.0, 0.0, 0.0, 1.0]), // 4: black
            Vertex(position: [ 1.0,  1.0, 1.0], color: [0.0, 1.0, 0.0, 1.0]), // 5: green
            Vertex(position: [ 1.0, -1.0,-1.0], color: [1.0, 0.0, 0.0, 1.0]), // 6: red
            Vertex(position: [ 1.0,  1.0,-1.0], color: [1.0, 1.0, 0.0, 1.0])  // 7: yellow
        ]
    }
 */
    
    static func buildIndices() -> [UInt16] {
        return [
              0, 1, 2, 1, 3, 2,
              3, 6, 2, 3, 7, 6,
              7, 4, 6, 7, 5, 4,
              5, 0, 4, 5, 1, 0,
              1, 5, 7, 1, 7, 3,
              2, 6, 4, 2, 4, 0
          ]
    }
}
