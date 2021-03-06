//
//  Renderer.swift
//  Metal
//
//  Created by Alex Nascimento on 13/08/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

import MetalKit
import ModelIO

class Renderer: NSObject {
   
    let device: MTLDevice
    let view: MTKView
    let commandQueue: MTLCommandQueue
    var vertexDescriptor: MTLVertexDescriptor
    var pipelineState: MTLRenderPipelineState
    var depthStencilState: MTLDepthStencilState
    
    var vertexBuffer: MTLBuffer
    var modelLoader: ModelLoader_Wrapper
    
    var cameraPosition = float3(0,0,20);
    var time: Double = 0
    
    let viewMtx: float4x4
    let aspect: Float
    let projMtx: float4x4
    var lighting: Lighting
    
    var scene: [float3]
    
    //logging variables
    var renderTime: Double = 0.0
    var frameCount: Int = 0
    
    init?(mtkView: MTKView){
        // View and Device
        self.view = mtkView
        if let mtkDevice = mtkView.device {
            self.device = mtkDevice
        } else {
            print("MTKView doesn't have a device")
            return nil
        }
        
        // Command Queue
        if let newCommandQueue = device.makeCommandQueue() {
            commandQueue = newCommandQueue
        } else { fatalError("Couldn't make command queue") }
        
        // Vertex Descriptor
        vertexDescriptor = Renderer.buildVertexDescriptor()
        
        // RenderPipeline and DepthStencil
        do {
            self.pipelineState = try Renderer.buildPipeline(device: device, view: view, vertexDescriptor: vertexDescriptor)
        } catch {
            fatalError("StandardPipeline failed to initialize. Error: \(error)")
        }
        self.depthStencilState = Renderer.buildDepthState(device: device)
        
        // Model Buffers
        modelLoader = ModelLoader_Wrapper("./Shared/teapot.obj")
        
        if let vbuff = device.makeBuffer(bytes: modelLoader.vertexData, length: modelLoader.vertexCount * modelLoader.vertexLength * MemoryLayout<Float>.stride, options: .cpuCacheModeWriteCombined) {
            vertexBuffer = vbuff
        } else {
            fatalError("Device unable to make buffer")
        }
        
        // Permanent Setup
        viewMtx = float4x4(translationBy: -cameraPosition)
        aspect = Float(view.drawableSize.width / view.drawableSize.height)
        projMtx = float4x4(perspectiveProjectionFov: (Float.pi)/4, aspectRatio: aspect, nearZ: 1, farZ: 4500)
        lighting = Lighting(ambient: [0.2,0.2,0.2], diffuse: [1,1,1], specular: [1,1,1], direction: [-1,-1,-1], camPos: cameraPosition)
        
        // Chose the scene here (scene[1...5])
        scene = scene4
        super.init()
    }
    
    static func buildVertexDescriptor() -> MTLVertexDescriptor {
        let vertexDescriptor = MTLVertexDescriptor()
        vertexDescriptor.attributes[0].format = .float3
        vertexDescriptor.attributes[0].offset = 0
        vertexDescriptor.attributes[0].bufferIndex = 0
        vertexDescriptor.attributes[1].format = .float3
        vertexDescriptor.attributes[1].offset = MemoryLayout<float3>.stride
        vertexDescriptor.attributes[1].bufferIndex = 0
        vertexDescriptor.layouts[0].stride = MemoryLayout<float3>.stride * 2
        return vertexDescriptor
    }
    
    static func buildPipeline(device: MTLDevice, view: MTKView, vertexDescriptor: MTLVertexDescriptor) throws -> MTLRenderPipelineState {
        guard let library = device.makeDefaultLibrary() else {
            fatalError("Couldn't find default library!")
        }
        let vertexFunction = library.makeFunction(name: "vertex_function")
        let fragmentFunction = library.makeFunction(name: "fragment_function")
        
        let renderPipelineDescriptor = MTLRenderPipelineDescriptor()
        renderPipelineDescriptor.vertexFunction = vertexFunction
        renderPipelineDescriptor.fragmentFunction = fragmentFunction
        
        renderPipelineDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat
        renderPipelineDescriptor.depthAttachmentPixelFormat = .depth32Float
        
        renderPipelineDescriptor.vertexDescriptor = vertexDescriptor
        
        do {
            return try device.makeRenderPipelineState(descriptor: renderPipelineDescriptor)
        } catch {
            fatalError("StandardPipeline:buildPipeline() Couldn't make MTLRenderPipelineState: \(error)")
        }
    }
    
    static func buildDepthState(device: MTLDevice) -> MTLDepthStencilState {
        let depthStencilDescriptor = MTLDepthStencilDescriptor()
        depthStencilDescriptor.isDepthWriteEnabled = true
        depthStencilDescriptor.depthCompareFunction = .less
        
        if let state = device.makeDepthStencilState(descriptor: depthStencilDescriptor) {
            return state
        } else { fatalError("StandardPipeline:buildDepthState() device returned nil DepthStencilState!") }
    }
}

extension Renderer: MTKViewDelegate {
    
    func draw(in view: MTKView) {
        
        guard let commandBuffer = commandQueue.makeCommandBuffer(),
        let renderPassDescriptor = view.currentRenderPassDescriptor else {
            print("Unable to get resources for rendering.")
            return
        }
    
        // Clear Background
        renderPassDescriptor.colorAttachments[0].clearColor = .init(red: 0.1, green: 0.2, blue: 0.4, alpha: 1.0)
        commandBuffer.addCompletedHandler { (cmdbuff) in
            let duration = cmdbuff.gpuEndTime - cmdbuff.gpuStartTime
            if duration == 0 { return }
            self.frameCount += 1
            self.renderTime += duration
            
            // Log
            if self.frameCount == 600 {
                let meanRenderTime = self.renderTime/Double(self.frameCount)
                print("Render Time médio: \(meanRenderTime)")
            }
        }
        if let commandEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) {
            
            // Scene setup (render loop)
            commandEncoder.setFrontFacing(.counterClockwise)
            commandEncoder.setCullMode(.back)
            commandEncoder.setDepthStencilState(depthStencilState)
            commandEncoder.setRenderPipelineState(pipelineState)
            
            commandEncoder.setFragmentBytes(&lighting, length: MemoryLayout<Lighting>.stride, index: 1)
            commandEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
            
            for i in 0..<scene.count {
                var imat = i%materials.count
                var modelMtx = float4x4(translationBy: scene[i])
                var uniforms = Uniforms(model: modelMtx, view: viewMtx, projection: projMtx)
                var material = Material(ambient: materials[imat].ambient, diffuse: materials[imat].diffuse, specular: materials[imat].specular, shininess: materials[imat].shininess)
                commandEncoder.setVertexBytes(&uniforms, length: MemoryLayout<Uniforms>.stride, index: 1)
                commandEncoder.setFragmentBytes(&material, length: MemoryLayout<Material>.stride, index: 2)
                commandEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: modelLoader.vertexCount)
            }
            
            commandEncoder.endEncoding()

            // drawable should be used and freed as soon as possible for better performance
            if let drawable = view.currentDrawable {
                commandBuffer.present(drawable)
            }
        }
        
        commandBuffer.commit()
    }
    
    func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
        
    }
}



