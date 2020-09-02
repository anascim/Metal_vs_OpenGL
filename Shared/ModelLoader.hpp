//
//  ModelLoader.hpp
//  Graphics_APIs
//
//  Created by Alex Nascimento on 29/08/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

#ifndef ModelLoader_hpp
#define ModelLoader_hpp

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "glm.hpp"

class ModelLoader
{
public:
    
    std::vector<float> vertexPositions;
    std::vector<float> vertexNormals;
    
    ModelLoader(std::string modelPath)
    {
        std::string modelString;
        std::fstream modelFileStream(modelPath);
        std::string line;
        if (modelFileStream.is_open())
        {
            while (getline(modelFileStream, line))
            {
                std::cout << line << std::endl;
            }
            modelFileStream.close();
        }
        
        else
        {
            std::cout << "Failed to open model file" << std::endl;
            std::cout << "Current path:" << std::__fs::filesystem::current_path() << std::endl;
        }
    }
};

#endif /* ModelLoader_hpp */
