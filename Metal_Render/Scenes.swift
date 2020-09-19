//
//  Scenes.swift
//  Metal_Render
//
//  Created by Alex Nascimento on 18/09/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

var scene1: [float3] = [
    [0,-1,0]
]

var scene2: [float3] = [
    [0,-1,0],
    [8,0,-7],
    [-3,1,10],
    [2,-6,1],
    [-10,-7,-20]
]

var scene3: [float3] = [
    [0,-1,0],     // gold
    [8,0,-7],     // bronze
    [-3,1,10],    // cyan
    [2,-6,1],     // red
    [-10,-7,-20], // green
    [8,-9,-4],   // gold
    [5,4,-3],     // bronze
    [17,-8,-24],     // cyan
    [4,3,-43],     // red
    [5,-5,-14],   // green
    [18,9,-19],     // gold
    [-3,5,-27],  // bronze
    [14,6,-33],     // cyan
    [-9,-3,-6],     // red
    [12,11,-13],  // green
    [-9,-11,-9],   // gold
    [-7,-6,1],    // bronze
    [-9,-3,-30],  // cyan
    [13,3,-9],     // red
    [0,8,-17]     // green
]

func genCylinder(radius: Float, angleDelta: Float, depthDelta: Float, posCount: Int) -> [float3] {
    var result = [float3]()
    for i in 0..<posCount {
        let fi = Float(i)
        let theta = fi * angleDelta
        let x = sin(theta) * radius
        let y = cos(theta) * radius
        let z = fi * -depthDelta
        result.append([x,y,z])
    }
    return result
}
// (x,y,z) -> (sin(theta i)R, cos(theta i)R, i d)

var scene4 = genCylinder(radius: 16, angleDelta: 1, depthDelta: 0.4, posCount: 1000)
var scene5 = genCylinder(radius: 16, angleDelta: 1, depthDelta: 0.4, posCount: 10000)
