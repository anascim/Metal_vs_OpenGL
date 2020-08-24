//
//  Shaders.metal
//  MetalGame
//
//  Created by Alex Nascimento on 16/01/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#include <metal_stdlib>
using namespace metal;

struct VertexIn
{
    float4 position [[ attribute(0) ]];
    float3 normal [[ attribute(1) ]];
    float3 color [[ attribute(2) ]];
};

struct VertexOut
{
    float4 position [[ position ]];
    float3 normal;
    float3 color;
    float4 worldPos;
};

struct Uniforms
{
    float4x4 model;
    float4x4 view;
    float4x4 projection;
};

struct Lighting
{
    float3 position;
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float3 camPos;
};

vertex VertexOut vertex_function(const VertexIn vertexIn [[ stage_in ]], // vertexIn takes the buffer index 0
                              constant Uniforms &uniforms [[ buffer(1) ]])
{
    VertexOut out;
    out.position = uniforms.projection * uniforms.view * uniforms.model * vertexIn.position;
    // warning: normal calculation doesn't conform to no uniform scaling
    out.normal = normalize(float3(uniforms.model * float4(vertexIn.normal,0)));
    out.color = vertexIn.color;
    out.worldPos = uniforms.model * vertexIn.position;
    return out;
}

fragment float4 fragment_function(const VertexOut vertexIn [[ stage_in ]],
                                  constant Lighting &lighting [[ buffer(0) ]])
{
    // iluminação Phong
    float3 ambient = lighting.ambient;
    
    float3 lightDir = normalize(float3(vertexIn.worldPos) - lighting.position);
    float3 diffuse = max(dot(vertexIn.normal, -lightDir),0.0);
    //diffuse = step(0.1, diffuse) * lighting.diffuse;
    
    float3 r = reflect(lightDir, vertexIn.normal);
    float3 camDir = normalize(float3(vertexIn.worldPos) - lighting.camPos);
    float specular = pow(max(dot(r, -camDir),0.0), 4);
    //specular = step(0.3, specular);
    
    float3 finalColor = (ambient + diffuse + specular) * float3(1.0, 0.2, 0.4);
    return float4(finalColor, 1.0);
}
