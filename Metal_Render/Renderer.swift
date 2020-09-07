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
//    var vertices: [Vertex]
//    var indexBuffer: MTLBuffer
//    var indices: [UInt16]
    
    var cameraPosition = float3(0,0,20);
    var time: Float = 0
    
    var testFloats: [Float] =
    // 0, 1, 2
        [-1.0, -1.0, 1.0, 0.0, -1.0, -1.0, 1.0, 0.0, // 0
            -1.0,  1.0, 1.0, 0.0, -1.0,  1.0, 1.0, 0.0, // 1
            -1.0, -1.0,-1.0, 0.0, -1.0, -1.0,-1.0, 0.0, // 2
    //-1.0,  1.0,-1.0, -1.0,  1.0,-1.0, // 3
    //1.0, -1.0, 1.0,  1.0, -1.0, 1.0, // 4
    //1.0,  1.0, 1.0,  1.0,  1.0, 1.0, // 5
    //1.0, -1.0,-1.0,  1.0, -1.0,-1.0, // 6
    //1.0,  1.0,-1.0,  1.0,  1.0,-1.0, // 7
    // 1, 3, 2
            -1.0,  1.0, 1.0, 0.0, -1.0,  1.0, 1.0, 0.0,
            -1.0,  1.0,-1.0, 0.0, -1.0,  1.0,-1.0, 0.0,
            -1.0, -1.0,-1.0, 0.0, -1.0, -1.0,-1.0, 0.0,
    // 3, 6, 2
            -1.0,  1.0,-1.0, 0.0, -1.0,  1.0,-1.0, 0.0,
            1.0, -1.0,-1.0, 0.0,  1.0, -1.0,-1.0, 0.0,
            -1.0, -1.0,-1.0, 0.0, -1.0, -1.0,-1.0, 0.0,
    // 3, 7, 6
            -1.0,  1.0,-1.0, 0.0, -1.0,  1.0,-1.0, 0.0,
            1.0,  1.0,-1.0, 0.0,  1.0,  1.0,-1.0, 0.0,
            1.0, -1.0,-1.0, 0.0,  1.0, -1.0,-1.0, 0.0,
    // 7, 4, 6
            1.0,  1.0,-1.0, 0.0,  1.0,  1.0,-1.0, 0.0,
            1.0, -1.0, 1.0, 0.0,  1.0, -1.0, 1.0, 0.0,
            1.0, -1.0,-1.0, 0.0,  1.0, -1.0,-1.0, 0.0,
    // 7, 5, 4
            1.0,  1.0,-1.0, 0.0,  1.0,  1.0,-1.0, 0.0,
            1.0,  1.0, 1.0, 0.0,  1.0,  1.0, 1.0, 0.0,
            1.0, -1.0, 1.0, 0.0,  1.0, -1.0, 1.0, 0.0,
    // 5, 0, 4
            1.0,  1.0, 1.0, 0.0,  1.0,  1.0, 1.0, 0.0,
            -1.0, -1.0, 1.0, 0.0, -1.0, -1.0, 1.0, 0.0,
            1.0, -1.0, 1.0, 0.0,  1.0, -1.0, 1.0, 0.0,
    // 5, 1, 0
            1.0,  1.0, 1.0, 0.0,  1.0,  1.0, 1.0, 0.0,
            -1.0,  1.0, 1.0, 0.0, -1.0,  1.0, 1.0, 0.0,
            -1.0, -1.0, 1.0, 0.0, -1.0, -1.0, 1.0, 0.0,
    // 1, 5, 7
            -1.0,  1.0, 1.0, 0.0, -1.0,  1.0, 1.0, 0.0,
            1.0,  1.0, 1.0, 0.0,  1.0,  1.0, 1.0, 0.0,
            1.0,  1.0,-1.0, 0.0,  1.0,  1.0,-1.0, 0.0,
    // 1, 7, 3
            -1.0,  1.0, 1.0, 0.0, -1.0,  1.0, 1.0, 0.0,
            1.0,  1.0,-1.0, 0.0,  1.0,  1.0,-1.0, 0.0,
            -1.0,  1.0,-1.0, 0.0, -1.0,  1.0,-1.0, 0.0,
    // 2, 6, 4
            -1.0, -1.0,-1.0, 0.0, -1.0, -1.0,-1.0, 0.0,
            1.0, -1.0,-1.0, 0.0,  1.0, -1.0,-1.0, 0.0,
            1.0, -1.0, 1.0, 0.0,  1.0, -1.0, 1.0, 0.0,
    // 2, 4, 0
            -1.0, -1.0,-1.0, 0.0, -1.0, -1.0,-1.0, 0.0,
            1.0, -1.0, 1.0, 0.0,  1.0, -1.0, 1.0, 0.0,
            -1.0, -1.0, 1.0, 0.0, -1.0, -1.0, 1.0, 0.0
    ]
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
        modelLoader = ModelLoader_Wrapper("./Shared/suzanne_triangulated.obj")
        
        if let vbuff = device.makeBuffer(bytes: modelLoader.vertexData, length: modelLoader.vertexCount * modelLoader.vertexLength * MemoryLayout<Float>.stride, options: .cpuCacheModeWriteCombined) {
            vertexBuffer = vbuff
        } else {
            fatalError("Device unable to make buffer")
        }
        
//        print("vertex count: ", modelLoader.vertexCount)
//        print("vertex length: ", modelLoader.vertexLength)
        
        let res = vertexBuffer.contents().bindMemory(to: Float.self, capacity: testFloats.count)
        var data = [Float](repeating:0, count: testFloats.count)
        for i in 0..<testFloats.count { data[i] = res[i] }
        
        for (i, v) in data.enumerated() {
            if i % 6 == 0 { print("][")}
            print(v)
        }
        
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
        time += 1.0/Float(view.preferredFramesPerSecond)
        
        guard let commandBuffer = commandQueue.makeCommandBuffer(),
        let renderPassDescriptor = view.currentRenderPassDescriptor else {
            print("Unable to get resources for rendering. Skipping frame at \(time)s")
            return
        }
    
        renderPassDescriptor.colorAttachments[0].clearColor = .init(red: 0.1, green: 0.3, blue: 0.2, alpha: 1.0)
        
        if let commandEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor) {
            // Render scene
            commandEncoder.setFrontFacing(.counterClockwise)
            commandEncoder.setCullMode(.back)
            commandEncoder.setDepthStencilState(depthStencilState)
            commandEncoder.setRenderPipelineState(pipelineState)
            
            //commandEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
            
            var modelMtx = float4x4(translationBy: float3(sin(time) * 3, cos(time) * 3, 0))
            modelMtx *= float4x4(rotationAbout: [1, 1, 0], by: time)
            let viewMtx = float4x4(translationBy: -cameraPosition)
            let aspect: Float  = Float(view.drawableSize.width / view.drawableSize.height)
            let projMtx = float4x4(perspectiveProjectionFov: (Float.pi)/6, aspectRatio: aspect, nearZ: 0.1, farZ: 100)
            var uniforms = Uniforms(model: modelMtx, view: viewMtx, projection: projMtx)
            commandEncoder.setVertexBytes(&uniforms, length: MemoryLayout<Uniforms>.stride, index: 1)
            var lighting = Lighting(position: [0, 20, 0], ambient: [0.2, 0, 0], diffuse: [0.5, 0.5, 0.5], specular: [0.9, 0, 0.4], camPos: [0,0,20])
            commandEncoder.setFragmentBytes(&lighting, length: MemoryLayout<Lighting>.stride, index: 0)
            //commandEncoder.drawIndexedPrimitives(type: .triangle, indexCount: indices.count, indexType: .uint16, indexBuffer: indexBuffer, indexBufferOffset: 0)
            
            commandEncoder.setVertexBuffer(vertexBuffer, offset: 0, index: 0)
            commandEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: modelLoader.vertexCount)
            
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



