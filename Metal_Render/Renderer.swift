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
    var vertices: [Vertex]
    var indexBuffer: MTLBuffer
    var indices: [UInt16]
    
    var meshes: [MTKMesh]
    
    var cameraPosition = float3(0,0,20);
    var time: Float = 0
    
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
        
        meshes = Renderer.loadMeshes(device: device, vertexDescriptor: vertexDescriptor)
        
        // Buffers
        vertices = Cube.buildVertices(color: [0.8, 0.3, 0.0])
        indices = Cube.buildIndices()
        if let vbuff = device.makeBuffer(bytes: vertices, length: MemoryLayout<Vertex>.stride * vertices.count, options: .cpuCacheModeWriteCombined),
            let ibuff = device.makeBuffer(bytes: indices, length: MemoryLayout<UInt16>.stride * indices.count, options: .cpuCacheModeWriteCombined) {
            vertexBuffer = vbuff
            indexBuffer = ibuff
        } else {
            fatalError("Device unable to make buffer")
        }
        
        super.init()
    }
    
    static func buildVertexDescriptor() -> MTLVertexDescriptor {
        let vertexDescriptor = MTLVertexDescriptor()
        vertexDescriptor.attributes[0].format = .float3
        vertexDescriptor.attributes[0].offset = 0
        vertexDescriptor.attributes[0].bufferIndex = 0
        vertexDescriptor.attributes[1].format = .float3
        vertexDescriptor.attributes[1].offset = MemoryLayout<SIMD3<Float>>.stride
        vertexDescriptor.attributes[1].bufferIndex = 0
        vertexDescriptor.attributes[2].format = .float3
        vertexDescriptor.attributes[2].offset = MemoryLayout<SIMD3<Float>>.stride * 2
        vertexDescriptor.attributes[2].bufferIndex = 0
        vertexDescriptor.layouts[0].stride = MemoryLayout<Vertex>.stride
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
    
    static func loadMeshes(device: MTLDevice, vertexDescriptor: MTLVertexDescriptor) -> [MTKMesh] {
        
        let mdlDescriptor = MTKModelIOVertexDescriptorFromMetal(vertexDescriptor)
        var attribute = mdlDescriptor.attributes[0] as! MDLVertexAttribute
        attribute.name = MDLVertexAttributePosition
        attribute = mdlDescriptor.attributes[1] as! MDLVertexAttribute
        attribute.name = MDLVertexAttributeNormal
        attribute = mdlDescriptor.attributes[2] as! MDLVertexAttribute
        attribute.name = MDLVertexAttributeColor
        let mtkBufferAllocator = MTKMeshBufferAllocator(device: device)
        guard let url = Bundle.main.url(forResource: "blue_suzanne", withExtension: "obj") else {
            fatalError("Unable to get URL from bundle")
        }
        let asset = MDLAsset(url: url, vertexDescriptor: mdlDescriptor, bufferAllocator: mtkBufferAllocator)
        
//        guard let mesh = asset.object(at: 0) as? MDLMesh else {
//            fatalError("Mesh not found")
//        }
        do {
            let (_, meshes) = try MTKMesh.newMeshes(asset: asset, device: device)
            return meshes
        } catch {
            fatalError("Couldn't create meshes\(error)")
        }
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
            
            
            
            guard let mesh = meshes.first else {
                fatalError("Mesh not found.")
            }
            let vertexBuffer = mesh.vertexBuffers[0]
            commandEncoder.setVertexBuffer(vertexBuffer.buffer, offset: vertexBuffer.offset, index: 0)
            guard let submesh = mesh.submeshes.first else {
                fatalError("Submesh not found.")
            }
            commandEncoder.drawIndexedPrimitives(type: submesh.primitiveType, indexCount: submesh.indexCount, indexType: submesh.indexType, indexBuffer: submesh.indexBuffer.buffer, indexBufferOffset: submesh.indexBuffer.offset)
            
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



