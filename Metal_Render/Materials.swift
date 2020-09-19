//
//  Materials.swift
//  Metal_Render
//
//  Created by Alex Nascimento on 18/09/20.
//  Copyright © 2020 Alex Nascimento. All rights reserved.
//

// Material values from: http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html

var gold = Material(ambient: [0.2125, 0.1275, 0.054],
                    diffuse: [0.75164, 0.60648, 0.22648],
                    specular: [0.628281, 0.555802, 0.366065],
                    shininess: 51.2)

var bronze = Material(ambient: [0.2125, 0.1275, 0.054],
                      diffuse: [0.714, 0.4284, 0.18144],
                      specular: [0.393548, 0.271906, 0.166721],
                      shininess: 25.6)

var cyanPlastic = Material(ambient: [0.0, 0.1, 0.06],
                           diffuse: [0.0, 0.50980392, 0.50980392],
                           specular: [0.50196078, 0.50196078, 0.50196078],
                           shininess: 32.0)

var redRubber = Material(ambient: [0.05, 0.0, 0.0],
                         diffuse: [0.5, 0.4, 0.4],
                         specular: [0.7, 0.04, 0.04],
                         shininess: 10.0)

var greenRubber = Material(ambient: [0.0, 0.05, 0.0],
                           diffuse: [0.4, 0.5, 0.4],
                           specular: [0.04, 0.7, 0.04],
                           shininess: 10.0)

var materials: [Material] = [gold, bronze, cyanPlastic, redRubber, greenRubber]
