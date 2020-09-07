//
//  ModelLoader.cpp
//  Graphics_APIs
//
//  Created by Alex Nascimento on 29/08/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#include "ModelLoader.hpp"

ModelLoader::ModelLoader(const char*  modelPath)
{
    fstream modelFileStream(modelPath);
    string line;
    if (modelFileStream.is_open())
    {
        while (getline(modelFileStream, line))
        {
            size_t found = line.find(" ");
            if (found != string::npos)
            {
                switch (line[0]) {
                    case 'v':
                        switch (line[1]) {
                            case ' ':
                                vertexPositions.push_back(getVectorFloats(line));
                                break;
                            case 't':
                                // vertex textures
                                break;
                            case 'n':
                                vertexNormals.push_back(getVectorFloats(line));
                                break;
                        }
                        break;
                    case 'f': {
                        size_t first = line.find(" ");
                        size_t second = line.find(" ", first+1);
                        
                        while (first != line.size())
                        {
                            string svert = line.substr(first, second-first);
                            size_t slash1 = svert.find("/");
                            size_t slash2 = svert.find("/", slash1+1);
                            vertexIndices.push_back(stoi(svert.substr(0, slash1)));
                            normalIndices.push_back(stoi(svert.substr( slash2+1, svert.size() )));
                            first = second;
                            second = line.find(" ", first+1);
                            if (second == string::npos) second = line.size();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
        modelFileStream.close();
        buildVertexData();
    }
    else
    {
        cout << "Failed to open model file at path: " << modelPath << endl;
        cout << "Current path: " << __fs::filesystem::current_path() << std::endl;
    }
}
vector<float> ModelLoader::getVertexData()
{
    return vertexData;
}

int ModelLoader::getVertexLength()
{
    return 8;
}

int ModelLoader::getDataLength()
{
    return vertexData.size();
}

void ModelLoader::printValues()
{
    for (int i = 0; i < vertexPositions.size(); i++)
    {
        cout << "v" << i << ": ";
        for (int j = 0; j < vertexPositions[i].size(); j++)
        {
            cout << vertexPositions[i][j] << " ";
        }
        cout << "\n";
    }
    for (int i = 0; i < vertexNormals.size(); i++)
    {
        cout << "vn" << i << ": ";
        for (int j = 0; j < vertexNormals[i].size(); j++)
        {
            cout << vertexNormals[i][j] << " ";
        }
        cout << "\n";
    }
}

vector<array<float,3>> ModelLoader::getPositions()
{
    return vertexPositions;
}

vector<array<float,3>> ModelLoader::getNormals()
{
    return vertexNormals;
}

array<float, 3> ModelLoader::getVectorFloats(string str) {
    vector<float> resultvec;
    size_t first = str.find(" ");
    size_t second = str.find(" ", first+1);
    while (first != str.size())
    {
        string foundstr = str.substr(first+1, second-first);
        resultvec.push_back(stof(foundstr));
        first = second;
        second = str.find(" ", first+1);
        if (second == string::npos) second = str.size();
    }
    array<float, 3> resultarr;
    std::copy(resultvec.begin(), resultvec.end(), &resultarr[0]);
    return resultarr;
}

// cria uma lista de vértices com seus atributos, sem EBO
// muitos vertices serão repetidos, mas é mais simples
void ModelLoader::buildVertexData()
{
    vector<float> vertices;
    size_t vIndex = 0;
    for (int j = 0; j < vertexIndices.size(); j++)
    {
        for (int k = 0; k < P_VEC_SIZE; k++)
        {
            vertices.push_back(vertexPositions[vertexIndices[vIndex]-1][k]);
        }
        vertices.push_back(0.0f); // alignment
        for (int k = 0; k < N_VEC_SIZE; k++)
        {
            vertices.push_back(vertexNormals[normalIndices[vIndex]-1][k]);
        }
        vertices.push_back(0.0f); // alignment
        vIndex++;
    }
    vertexData = vertices;
}
