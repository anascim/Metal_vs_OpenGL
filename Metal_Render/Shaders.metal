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
    float3 position [[ attribute(0) ]];
    float3 normal [[ attribute(1) ]];
};

struct VertexOut
{
    float4 position [[ position ]];
    float3 normal;
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
    float3 direction; // directional light
    float3 color;
    float3 camPos;
};

struct Material
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
    float shininnes;
};

vertex VertexOut vertex_function(const VertexIn vertexIn [[ stage_in ]], // vertexIn takes the buffer index 0
                              constant const Uniforms &uniforms [[ buffer(1) ]])
{
    VertexOut out;
    out.position = uniforms.projection * uniforms.view * uniforms.model * float4(vertexIn.position,1);
    // warning: normal calculation doesn't conform to not uniform scaling
    out.normal = normalize(float3(uniforms.model * float4(vertexIn.normal,0)));
    out.worldPos = uniforms.model * float4(vertexIn.position,1);
    return out;
}

fragment float4 fragment_function(const VertexOut vertexIn [[ stage_in ]],
                                  constant const Lighting &lighting [[ buffer(1) ]],
                                  constant const Material &material [[ buffer(2) ]])
{
    // iluminação Phong
    float3 ambient = material.ambient;
    
    float3 lightDir = normalize(lighting.direction);
    float nDotL = max(dot(vertexIn.normal, -lightDir),0.0);
    float3 diffuse = nDotL * material.diffuse;
    
    float3 r = reflect(lightDir, vertexIn.normal);
    float3 camDir = normalize(lighting.camPos - float3(vertexIn.worldPos));
    float vDotR = max(dot(camDir, r),0.0);
    float3 specular = pow(vDotR, material.shininnes);
    specular *= material.specular;
    
    float3 color = (ambient + diffuse + specular) * lighting.color;
    return float4(color, 1.0);
}
