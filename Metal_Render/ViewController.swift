//
//  ViewController.swift
//  Graphics_APIs
//
//  Created by Alex Nascimento on 13/08/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

import Cocoa
import MetalKit

class ViewController: NSViewController {
    
    var renderer: Renderer!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        guard let mtkView = self.view as? MTKView else {
            fatalError("View is not a MTKView")
        }
        
        guard let defaultDevice = MTLCreateSystemDefaultDevice() else {
            fatalError("Metal is not supported on this device")
        }
        mtkView.device = defaultDevice
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.depthStencilPixelFormat = .depth32Float
        mtkView.clearColor = MTLClearColorMake(0.4, 0.1, 0.1, 1.0)
        renderer = Renderer(mtkView: mtkView)
        mtkView.delegate = renderer
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }


}

