//
//  ViewProjection.swift
//  MetalGame
//
//  Created by Alex Nascimento on 23/01/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

import MetalKit

/// This is a static class for building view-projection matrices.
/// It's not recommended to instantiate this.

class ViewProjection {
    
    static func buildMatrix(mtkView: MTKView, cameraTranslation: float3, fov: Float, near: Float, far: Float) -> float4x4 {
        let viewMatrix = float4x4(translationBy: cameraTranslation)
        
        let aspect: Float  = Float(mtkView.drawableSize.width / mtkView.drawableSize.height)
        let projectionMatrix = float4x4(perspectiveProjectionFov: fov, aspectRatio: aspect, nearZ: near, farZ: far)
        
        let viewProjectionMatrix = projectionMatrix * viewMatrix
        return viewProjectionMatrix
    }
}

