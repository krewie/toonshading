//! @file    OBJFileReader.cpp
//! @author  Johan Nysjo
//! @date    <2012-08-05 Sun>
//!
//! @brief Source file with definitions for OBJFileReader.h
//!

#include "OBJFileReader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

// Unnamed namespace (for helper functions and constants)
namespace {
const std::string VERTEX_LINE("v ");
  
const std::string FACE_LINE("f ");
  
void computeNormals(std::vector<glm::vec3> const &vertices,
                    std::vector<uint32_t> const &indices,
                    std::vector<glm::vec3> &normals)
{
    normals.resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Compute per-vertex normals by averaging the unnormalized face normals
    uint32_t vertexIndex0, vertexIndex1, vertexIndex2;
    glm::vec3 normal;
    int numIndices = indices.size();
    for (int i = 0; i < numIndices; i += 3) {
        vertexIndex0 = indices[i];
        vertexIndex1 = indices[i + 1];
        vertexIndex2 = indices[i + 2];
        normal = glm::cross(vertices[vertexIndex1] - vertices[vertexIndex0],
                            vertices[vertexIndex2] - vertices[vertexIndex0]);
        normals[vertexIndex0] += normal;
        normals[vertexIndex1] += normal;
        normals[vertexIndex2] += normal;
    }
    
    int numNormals = normals.size();
    for (int i = 0; i < numNormals; i++) {
        normals[i] = glm::normalize(normals[i]);
    }
}
}

using namespace cgtk;

OBJFileReader::OBJFileReader() :
    mVertices(0),
    mNormals(0),
    mIndices(0)
{
    std::cout << "Called OBJFileReader constructor" << std::endl;
}

OBJFileReader::~OBJFileReader()
{
}

bool OBJFileReader::load(const char *filename)
{
    // Open OBJ file
    std::ifstream OBJFile(filename);
    if (!OBJFile.is_open()) {
        std::cerr << "Could not open " << filename << std::endl;
        return false;
    }
  
    // Extract vertices and indices
    std::string line;
    glm::vec3 vertex;
    uint32_t vertexIndex0, vertexIndex1, vertexIndex2;
    while (!OBJFile.eof()) {
        std::getline(OBJFile, line);
        if (line.substr(0, 2) == VERTEX_LINE) {
            std::istringstream vertexLine(line.substr(2));
            vertexLine >> vertex.x;
            vertexLine >> vertex.y;
            vertexLine >> vertex.z;
            mVertices.push_back(vertex);
        }
        else if (line.substr(0, 2) == FACE_LINE) {
            std::istringstream faceLine(line.substr(2));
            faceLine >> vertexIndex0;
            faceLine >> vertexIndex1;
            faceLine >> vertexIndex2;
            mIndices.push_back(vertexIndex0 - 1);
            mIndices.push_back(vertexIndex1 - 1);
            mIndices.push_back(vertexIndex2 - 1);
        }
        else {
            // Ignore line
        }
    }
  
    // Close OBJ file
    OBJFile.close();
  
    // Compute normals
    computeNormals(mVertices, mIndices, mNormals);

    // Display log message
    std::cout << "Loaded OBJ file " << filename << std::endl;
    int numTriangles = mIndices.size() / 3;
    std::cout << "Number of triangles: " << numTriangles << std::endl;
  
    return true;
}

std::vector<glm::vec3> const &OBJFileReader::getVertices() const
{
    return mVertices;
}

std::vector<glm::vec3> const &OBJFileReader::getNormals() const
{
    return mNormals;
}

std::vector<uint32_t> const &OBJFileReader::getIndices() const
{
    return mIndices;
}
