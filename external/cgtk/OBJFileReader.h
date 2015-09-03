//! @file    OBJFileReader.h
//! @author  Johan Nysjo
//! @date    <2012-08-05 Sun>
//!
//! @brief Header declaring the OBJFileReader class
//!

#pragma once

#include <glm/glm.hpp>

#include <stdint.h>
#include <vector>

namespace cgtk {
//! @class OBJFileReader OBJFileReader.h OBJFileReader.h
//!
//! @brief OBJ file reader class
//!
//! Reads a 3D model (represented as an indexed triangle mesh) from a
//! wavefront .obj (OBJ) file.
//!
class OBJFileReader {
public:
    //! Constructor
    //!
    OBJFileReader();

    //! Destructor
    //!
    ~OBJFileReader();

    //! Execute the reader.
    //!
    //! @param[in] filename The name of the OBJ file to be read.
    //! @return true if the reading succeeded, otherwise false.
    //!
    bool load(const char *filename);

    //! Get the vertices of the 3D model.
    //!
    //! @return An array of vertices.
    //!
    std::vector<glm::vec3> const &getVertices() const;

    //! Get the per-vertex normals of the 3D model.
    //!
    //! @return An array of normal vectors.
    //! 
    std::vector<glm::vec3> const &getNormals() const;

    //! Get the element indices of the 3D model.
    //!
    //! @return An array of element indices.
    //!
    std::vector<uint32_t> const &getIndices() const;
private:
    std::vector<glm::vec3> mVertices;
    std::vector<glm::vec3> mNormals;
    std::vector<uint32_t> mIndices;
};
} // namespace cgtk
