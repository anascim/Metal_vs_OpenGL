//
//  ModelLoader.hpp
//  Graphics_APIs
//
//  Created by Alex Nascimento on 29/08/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class ModelLoader
{
private:
    static constexpr int P_VEC_SIZE = 3;
    static constexpr int N_VEC_SIZE = 3;
    static constexpr int INDICES_SIZE = 3;
    vector<array<float,P_VEC_SIZE>> vertexPositions;
    vector<array<float,N_VEC_SIZE>> vertexNormals;
    vector<uint> vertexIndices;
    vector<uint> normalIndices;
    vector<float> vertexData;
    
public:
    ModelLoader(const char*  modelPath);
    vector<float> getVertexData();
    int getVertexLength();
    int getDataLength();
    vector<array<float,3>> getPositions();
    vector<array<float,3>> getNormals();
private:
    array<float, 3> getVectorFloats(string str);
    void buildVertexData();
    void printValues();
};
