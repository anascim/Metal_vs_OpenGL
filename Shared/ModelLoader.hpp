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
    ModelLoader(const char*  modelPath)
    {
        string modelPathString = "Shared/suzanne_triangulated.obj";
        fstream modelFileStream(modelPathString);
        string line;
        cout << "trying to open: " << modelPath << endl;
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
            // PRINT VALUES
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
            modelFileStream.close();
            buildVertexData();
        }
        else
        {
            cout << "Failed to open model file at path: " << modelPathString << endl;
            cout << "Current path: " << __fs::filesystem::current_path() << std::endl;
        }
    }
    
    // criar uma lista de vértices com seus atributos, sem EBO
    // significa que muitos vertices serão repetidos, mas é mais simples
    vector<float> getVertexData()
    {
        return vertexData;
    }
    
    int getVertexLength()
    {
        return 8;
    }
    
    int getDataLength()
    {
        return vertexData.size();
    }
    
    vector<array<float,3>> getPositions()
    {
        return vertexPositions;
    }
    
    vector<array<float,3>> getNormals()
    {
        return vertexNormals;
    }
private:
    array<float, 3> getVectorFloats(string str) {
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
    
    void buildVertexData()
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
};
