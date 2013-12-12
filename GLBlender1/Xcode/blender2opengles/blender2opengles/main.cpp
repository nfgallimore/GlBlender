//
//  main.cpp
//  blender2opengles
//
//  Created by Nick Gallimore on 12/11/13.
//  Copyright (c) 2013 Virtual Theologies. All rights reserved.
//

// C++ Standard Library
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Model Structure
typedef struct Model
{
    int vertices;
    int positions;
    int texels;
    int normals;
    int faces;
}
Model;

#define POS 3
#define TEX 2
#define NORM 3
#define FACE 9


/*
 * Reads OBJ file
 */
Model getOBJinfo(string fp)
{
    // Open your OBJ file
    Model model = {0};
    ifstream inOBJ;
    inOBJ.open(fp);
    if(!inOBJ.good())
    {
        cout << "ERROR OPENING OBJ FILE" << endl;
        exit(1);
    }
    
    // Read OBJ file
    while (!inOBJ.eof())
    {
        string line;
        getline(inOBJ, line);
        string type = line.substr(0, 2);
        
        if (type.compare("v ") == 0)
            model.positions++;
        
        else if (type.compare("vt") == 0)
            model.texels++;
        
        else if (type.compare("vn") == 0)
            model.normals++;
        
        else if (type.compare("f ") == 0)
            model.faces++;
    }
    
    // Update faces
    model.vertices = model.faces * 3;
    
    // Close OBJ file
    inOBJ.close();
    
    return model;
    
}

/*
 * Parses the OBJ data
 */
void extractOBJdata(string fp, float positions[][POS], float texels[][TEX],
    float normals[][NORM], int faces[][FACE])
{
    // Counters
    int p = 0;
    int t = 0;
    int n = 0;
    int f = 0;
    
    // Open OBJ file
    ifstream inOBJ;
    inOBJ.open(fp);
    if(!inOBJ.good())
    {
        cout << "ERROR OPENING OBJ FILE" << endl;
        exit(1);
    }
    
    // Read OBJ file
    while (!inOBJ.eof())
    {
        string line;
        getline(inOBJ, line);
        string type = line.substr(0, 2);
        
        // Positions
        if (type.compare("v ") == 0)
        {
            // Copy line for parsing
            char* str = new char[line.size()+1];
            memcpy(str, line.c_str(), line.size()+1);
            
            // Extract tokens
            strtok(str, " ");
            for (int i = 0; i < POS; i++)
            {
                positions[p][i] = atof(strtok(NULL, " "));
            }
            
            // Wrap up
            delete[] str;
            p++;
        }
        
        // Texels
        else if (type.compare("vt") == 0)
        {
            // Copy line for parsing
            char* str = new char[line.size()+1];
            memcpy(str, line.c_str(), line.size()+1);
            
            // Extract tokens
            strtok(str, " ");
            for (int i = 0; i < TEX; i++)
            {
                texels[t][i] = atof(strtok(NULL, " "));
            }
            
            // Wrap up
            delete[] str;
            t++;
        }
        
        // Normals
        else if (type.compare("vn") == 0)
        {
            char* str = new char[line.size()+1];
            memcpy(str, line.c_str(), line.size()+1);
            
            // Extract tokens
            strtok(str, " ");
            for (int i = 0; i < NORM; i++)
            {
                normals[n][i] = atof(strtok(NULL, " "));
            }
            
            // Wrap up
            delete[] str;
            n++;
        }
        
        // Faces
        else if (type.compare("f ") == 0)
        {
            char* str = new char[line.size()+1];
            memcpy(str, line.c_str(), line.size()+1);
            
            // Extract tokens
            strtok(str, " ");
            for (int i = 0; i < FACE; i++)
            {
                faces[f][i] = atof(strtok(NULL, " /"));
            }
            
            // Wrap up
            delete[] str;
            f++;
        }
    }
    
    // Close OBJ file
    inOBJ.close();

}


/* 
 * Creates a new header file
 */
void writeH(string fp, string name, Model model)
{
    // Create H file
    ofstream outH;
    outH.open(fp);
    if(!outH.good())
    {
        cout << "ERROR CREATING H FILE" << endl;
        exit(1);
    }
    
    // Write to H file
    outH << "// This is a .h file for the model: " << name << endl;
    outH << endl;
    
    // Write statistics
    outH << "// Positions: " << model.positions << endl;
    outH << "// Texels: " << model.texels << endl;
    outH << "// Normals: " << model.normals << endl;
    outH << "// Faces: " << model.faces << endl;
    outH << "// Vertices: " << model.vertices << endl;
    outH << endl;
    
    // Write declarations
    outH << "const int " << name << "Vertices;" << endl;
    outH << "const float " << name << "Positions[" << model.vertices * POS << "];" << endl;
    outH << "const float " << name << "Texels[" << model.vertices * TEX << "];" << endl;
    outH << "const float " << name << "Normals[" << model.vertices * NORM << "];" << endl;
    outH << endl;
    
    // Close H file
    outH.close();
}

/*
 *  Generates C file
 */
void writeCvertices(string fp, string name, Model model)
{
    // Create C file
    ofstream outC;
    outC.open(fp);
    if(!outC.good())
    {
        cout << "ERROR CREATING C FILE" << endl;
        exit(1);
    }
    
    // Write to C file
    outC << "// This is a .c file for the model: " << name << endl;
    outC << endl;
    
    // Header
    outC << "#include " << "\"" << name << ".h" << "\"" << endl;
    outC << endl;
    
    // Vertices
    outC << "const int " << name << "Vertices = " << model.vertices << ";" << endl;
    outC << endl;
    
    // Close C file
    outC.close();
}


/*
 * Writes Positions to C file
 */
void writeCpositions(string fp, string name, Model model, int faces[][FACE], float positions[][POS])
{
    // Append C file
    ofstream outC;
    outC.open(fp, ios::app);
    
    // Positions
    outC << "const float " << name << "Positions[" << model.vertices * POS << "] = " << endl;
    outC << "{" << endl;
    for (int i = 0; i < model.faces; i++)
    {
        int vA = faces[i][0] - 1;
        int vB = faces[i][3] - 1;
        int vC = faces[i][6] - 1;
        
        outC << positions[vA][0] << ", " << positions[vA][1] << ", " << positions[vA][2] << ", " << endl;
        outC << positions[vB][0] << ", " << positions[vB][1] << ", " << positions[vB][2] << ", " << endl;
        outC << positions[vC][0] << ", " << positions[vC][1] << ", " << positions[vC][2] << ", " << endl;
    }
    outC << "};" << endl;
    outC << endl;
    
    // Close C file
    outC.close();
    
}


/*
 * Writes Texels to C file
 */
void writeCtexels(string fp, string name, Model model, int faces[][FACE], float texels[][TEX])
{
    // Append C file
    ofstream outC;
    outC.open(fp, ios::app);
    
    // Texel's
    outC << "const float " << name << "Texels[" << model.vertices * TEX << "] = " << endl;
    outC << "{" << endl;
    for (int i = 0; i < model.faces; i++)
    {
        int vA = faces[i][1] - 1;
        int vB = faces[i][4] - 1;
        int vC = faces[i][7] - 1;
        
        outC << texels[vA][0] << ", " << texels[vA][1] << ", " << endl;
        outC << texels[vB][0] << ", " << texels[vB][1] << ", " << endl;
        outC << texels[vC][0] << ", " << texels[vC][1] << ", " << endl;
    }
    outC << "};" << endl;
    outC << endl;
    
    // Close C file
    outC.close();
    
}


/*
 * Writes Normals to C file
 */
void writeCnormals(string fp, string name, Model model, int faces[][FACE], float normals[][NORM])
{
    // Append C file
    ofstream outC;
    outC.open(fp, ios::app);
    
    // Normal's
    outC << "const float " << name << "Normals[" << model.vertices * NORM << "] = " << endl;
    outC << "{" << endl;
    for (int i = 0; i < model.faces; i++)
    {
        int vA = faces[i][2] - 1;
        int vB = faces[i][5] - 1;
        int vC = faces[i][8] - 1;
        
        outC << normals[vA][0] << ", " << normals[vA][1] << ", " << normals[vA][2] << ", " << endl;
        outC << normals[vB][0] << ", " << normals[vB][1] << ", " << normals[vB][2] << ", " << endl;
        outC << normals[vC][0] << ", " << normals[vC][1] << ", " << normals[vC][2] << ", " << endl;
    }
    outC << "};" << endl;
    outC << endl;
    
    // Close C file
    outC.close();
    
}


/*
 *  Main run of program
 */
int main(int argc, const char * argv[])
{
    // Arguments
    cout << argc << endl;
    cout << argv[0] << endl;
    cout << argv[1] << endl;
    
    // Files
    string nameOBJ = argv[1];
    string filepathOBJ = "source/" + nameOBJ + ".obj";
    string filepathH = "product/" + nameOBJ + ".h";
    string filepathC = "product/" + nameOBJ + ".c";
    
    // Model Info
    Model model = getOBJinfo(filepathOBJ);
    cout << "Model Info" << endl;
    cout << "Positions: " << model.positions << endl;
    cout << "Texels: " << model.texels << endl;
    cout << "Normals: " << model.normals << endl;
    cout << "Faces: " << model.faces << endl;
    cout << "Vertices: " << model.vertices << endl;
    
    // Model Data
    float positions[model.positions][POS];    // XYZ
    float texels[model.texels][TEX];          // UV
    float normals[model.normals][NORM];        // XYZ
    int faces[model.faces][FACE];              // PTN PTN PTN
    
    // Parse Data
    extractOBJdata(filepathOBJ, positions, texels, normals, faces);
    cout << "Model DATA" << endl;
    cout << "P1: " << positions[0][0] << "x " << positions[0][1] << "y " << positions[0][2] << "z" << endl;
    cout << "T1: " << texels[0][0] << "u " << texels[0][1] << "v " << endl;
    cout << "N1: " << normals[0][0] << "x " << normals[0][1] << "y " << normals[0][2] << "z" << endl;
    cout << "F1v1: " << faces[0][0] << "p " << faces[0][1] << "t " << faces[0][2] <<"n" << endl;
    
    // Write H file
    writeH(filepathH, nameOBJ, model);
    
    // Write C file
    writeCvertices(filepathC, nameOBJ, model);
    writeCpositions(filepathC, nameOBJ, model, faces, positions);
    writeCtexels(filepathC, nameOBJ, model, faces, texels);
    writeCnormals(filepathC, nameOBJ, model, faces, normals);
    
    return 0;
    
}
