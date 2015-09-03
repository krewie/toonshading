// Assignment 2, Part 2
//
// Modify this file and the mesh.vert and mesh.frag shaders to
// implement the 3D model viewer.
//

#include "GLSLProgram.h"
#include "OBJFileReader.h"
#include "Trackball.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <AntTweakBar.h>

#include <iostream>
#include <cstdlib>
#include <algorithm>

// The attribute locations we will use in the vertex shader
enum AttributeLocation {
    POSITION = 0,
    NORMAL = 1
};

// Struct for representing an indexed triangle mesh
struct Mesh {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<uint32_t> indices;
};

// Struct for representing a vertex array object (VAO) created from a
// mesh. Used for rendering.
struct MeshVAO {
    GLuint vao;
    GLuint vertexVBO;
    GLuint normalVBO;
    GLuint indexVBO;
    int numVertices;
    int numIndices;
};

// Struct for global resources
struct Globals {
    int width;
    int height;
    cgtk::GLSLProgram program;
    cgtk::Trackball trackball;
    Mesh mesh;
    MeshVAO meshVAO;
    glm::vec3 lightDir;
    float lightdir_x;
    float lightdir_y;
    float lightdir_z;
    float material_kd;
    float outline_intensity;
    float zoomfactor;
    glm::vec3 bg_color;
    int colorlvl;

    glm::vec3 diffuseColor;
    glm::vec3 ambientColor;
    glm::vec3 outlineColor;

    Globals()
    {
        width = 800;
        height = 600;
        lightdir_x = 0.0f;
        lightdir_y = 0.0f;
        lightdir_z = 1.5f;
        lightDir = glm::vec3(lightdir_x, lightdir_y, lightdir_z);
        material_kd = 1.0f;
        diffuseColor = glm::vec3(1.0f, 0.0f, 0.0f);
        ambientColor = glm::vec3(0.1f, 0.0f, 0.0f);
        outlineColor = glm::vec3(0.0f, 0.0f, 0.0f);
        bg_color = glm::vec3(1.0, 0.5, 0.0);
        outline_intensity = 0.4;
        zoomfactor = 1.5f;
        colorlvl = 5;
    }
};

Globals globals;

void errorCallback(int error, const char* description)
{
    std::cerr << description << std::endl;
}

// Returns the value of the environment variable whose name is
// specified by the argument
std::string getEnvVar(const std::string &name)
{
    char *value = std::getenv(name.c_str());
    if (value == NULL) {
        return std::string();
    }
    else {
        return std::string(value);
    }
}

// Returns the absolute path to the shader directory
std::string shaderDir(void)
{
    std::string rootDir = getEnvVar("ASSIGNMENT3_ROOT");
    if (rootDir.empty()) {
        std::cout << "Error: ASSIGNMENT3_ROOT is not set." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return rootDir + "/part1/src/shaders/";
}

// Returns the absolute path to the 3D model directory
std::string modelDir(void)
{
    std::string rootDir = getEnvVar("ASSIGNMENT3_ROOT");
    if (rootDir.empty()) {
        std::cout << "Error: ASSIGNMENT3_ROOT is not set." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return rootDir + "/3d_models/";
}

void loadProgram(const std::string &vertexShaderFilename,
                 const std::string &fragmentShaderFilename,
                 cgtk::GLSLProgram *program)
{
    std::string vertexShaderSource = cgtk::readGLSLSource(vertexShaderFilename);
    std::string fragmentShaderSource = cgtk::readGLSLSource(fragmentShaderFilename);

    program->setShaderSource(GL_VERTEX_SHADER, vertexShaderSource);
    program->setShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource);
    program->update();
    if (!program->isValid()) {
        std::cerr << "Error: Could not create program." << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void loadMesh(const std::string &filename, Mesh *mesh)
{
    cgtk::OBJFileReader reader;
    reader.load(filename.c_str());
    mesh->vertices = reader.getVertices();
    mesh->normals = reader.getNormals();
    mesh->indices = reader.getIndices();
}

void createMeshVAO(const Mesh &mesh, MeshVAO *meshVAO)
{
    // Generates and populates a VBO for the vertices
    glGenBuffers(1, &(meshVAO->vertexVBO));
    glBindBuffer(GL_ARRAY_BUFFER, meshVAO->vertexVBO);
    auto verticesNBytes = mesh.vertices.size() * sizeof(mesh.vertices[0]);
    glBufferData(GL_ARRAY_BUFFER, verticesNBytes, mesh.vertices.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for the vertex normals
    glGenBuffers(1, &(meshVAO->normalVBO));
    glBindBuffer(GL_ARRAY_BUFFER, meshVAO->normalVBO);
    auto normalsNBytes = mesh.normals.size() * sizeof(mesh.normals[0]);
    glBufferData(GL_ARRAY_BUFFER, normalsNBytes, mesh.normals.data(), GL_STATIC_DRAW);

    // Generates and populates a VBO for the element indices
    glGenBuffers(1, &(meshVAO->indexVBO));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVAO->indexVBO);
    auto indicesNBytes = mesh.indices.size() * sizeof(mesh.indices[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNBytes, mesh.indices.data(), GL_STATIC_DRAW);

    // Creates a vertex array object (VAO) for drawing the mesh
    glGenVertexArrays(1, &(meshVAO->vao));
    glBindVertexArray(meshVAO->vao);
    glBindBuffer(GL_ARRAY_BUFFER, meshVAO->vertexVBO);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, meshVAO->normalVBO);
    glEnableVertexAttribArray(NORMAL);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshVAO->indexVBO);
    glBindVertexArray(0); // unbinds the VAO

    // Additional information required by draw calls
    meshVAO->numVertices = mesh.vertices.size();
    meshVAO->numIndices = mesh.indices.size();
}

void initializeTrackball(void)
{
    double radius = double(std::min(globals.width, globals.height)) / 2.0;
    globals.trackball.setRadius(radius);
    glm::vec2 center = glm::vec2(globals.width, globals.height) / 2.0f;
    globals.trackball.setCenter(center);
}

void init(void)
{
    glClearColor(globals.bg_color.x, globals.bg_color.y, globals.bg_color.z, 1.0);

    loadProgram(shaderDir() + "mesh.vert",
                shaderDir() + "mesh.frag",
                &globals.program);

    loadMesh((modelDir() + "bunny.obj"), &globals.mesh);
    createMeshVAO(globals.mesh, &globals.meshVAO);

    initializeTrackball();
}

// MODIFY THIS FUNCTION
void drawMesh(cgtk::GLSLProgram &program, const MeshVAO &meshVAO)
{

    glm::mat4 model = globals.trackball.getRotationMatrix() * glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(90.0f+globals.zoomfactor, (float) globals.width/globals.height, 0.1f, 100.0f);

    glm::mat4 mvp = projection * view * model;
    glm::mat4 mv = view * model;


    program.enable();

    program.setUniformMatrix4f("mvp", mvp);
    program.setUniformMatrix4f("view", view);
    program.setUniformMatrix4f("model", model);
    program.setUniform3f("lightDir", globals.lightDir);
    program.setUniform3f("eye_position", glm::vec3(0.0f, 0.0f, 1.5f));
    program.setUniform1f("material_kd", globals.material_kd);
    program.setUniform1f("outline_intensity", globals.outline_intensity);

    program.setUniform3f("diffuseColor", globals.diffuseColor);
    program.setUniform3f("ambientColor", globals.ambientColor);
    program.setUniform3f("outlineColor", globals.outlineColor);

    program.setUniform1i("colorlvl", globals.colorlvl);


    glBindVertexArray(meshVAO.vao);
    glDrawElements(GL_TRIANGLES, meshVAO.numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    program.disable();

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); // ensures that polygons overlap correctly
    drawMesh(globals.program, globals.meshVAO);

}


void TW_CALL setDifflvl(const void *value, void *clientData)
{
  globals.material_kd = *(const float *) value;
}

void TW_CALL getDifflvl(void *value, void *clientData)
{
  *(float *)value = globals.material_kd;  // for instance
}

void TW_CALL setOutlinelvl(const void *value, void *clientData)
{
  globals.outline_intensity = *(const float *) value;
}

void TW_CALL getOutlinelvl(void *value, void *clientData)
{
  *(float *)value = globals.outline_intensity;  // for instance
}


void TW_CALL setColorlvl(const void *value, void *clientData)
{
  globals.colorlvl = *(const int *) value;
}

void TW_CALL getColorlvl(void *value, void *clientData)
{
  *(int *)value = globals.colorlvl;  // for instance
}


void TW_CALL setLightXpos(const void *value, void *clientData)
{
  globals.lightDir.x = *(const float *) value;
}

void TW_CALL getLightXpos(void *value, void *clientData)
{
  *(float *)value = globals.lightDir.x;  // for instance
}

void TW_CALL setLightYpos(const void *value, void *clientData)
{
  globals.lightDir.y = *(const float *) value;
}

void TW_CALL getLightYpos(void *value, void *clientData)
{
  *(float *)value = globals.lightDir.y;  // for instance
}

void TW_CALL setLightZpos(const void *value, void *clientData)
{
  globals.lightDir.z = *(const float *) value;
}

void TW_CALL getLightZpos(void *value, void *clientData)
{
  *(float *)value = globals.lightDir.z;  // for instance
}

void TW_CALL setBgcolorCallBack(const void *value, void *clientData)
{
  globals.bg_color = *(const glm::vec3 *)value;
  glClearColor(globals.bg_color.x, globals.bg_color.y, globals.bg_color.z, 1.0);
}

void TW_CALL getBgcolorCallBack(void *value, void *clientData)
{
  *(glm::vec3 *)value = globals.bg_color;  // for instance
}

// MODIFY THIS FUNCTION
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if( !TwEventKeyGLFW(key, action) )  // Send event to AntTweakBar
  {
  }
    // Define your keyboard shortcuts here
}

void mouseButtonPressed(int button, int x, int y)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        globals.trackball.setCenter(glm::vec2(x, y));
        globals.trackball.startTracking(glm::vec2(x, y));
    }
}

void mouseButtonReleased(int button, int x, int y)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        globals.trackball.stopTracking();
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  //if(globals.zoomfactor > -90.f && globals.zoomfactor < 90.f)
    globals.zoomfactor += yoffset;
  //else
  //  globals.zoomfactor = 89.f;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    if(!TwEventMouseButtonGLFW(button, action)){
      if (action == GLFW_PRESS) {
          mouseButtonPressed(button, x, y);
      }
      else {
          mouseButtonReleased(button, x, y);
      }
  }
}

void moveTrackball(int x, int y)
{
    if (globals.trackball.tracking()) {
        globals.trackball.move(glm::vec2(x, y));
    }
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
  if( !TwEventMousePosGLFW(x, y) )  // Send event to AntTweakBar
  {
    moveTrackball(x, y);
  }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    globals.width = width;
    globals.height = height;
    globals.trackball.setRadius(double(std::min(width, height)) / 2.0);
    globals.trackball.setCenter(glm::vec2(width, height) / 2.0f);
    glViewport(0, 0, width, height);
}

int main(void)
{
    // Create window and load extensions
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(globals.width, globals.height,
        "Toon shading", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    GLenum status = glewInit();
    if (status != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(globals.width, globals.height);

    TwBar *myBar;
    myBar = TwNewBar("Toon config");

    TwAddVarCB(myBar, "X pos", TW_TYPE_FLOAT, setLightXpos, getLightXpos , &globals.lightdir_x, " step=0.5 group=Light label='x-pos' ");
    TwAddVarCB(myBar, "Y pos", TW_TYPE_FLOAT, setLightYpos, getLightYpos , &globals.lightdir_y, " step=0.5 group=Light label='y-pos' ");
    TwAddVarCB(myBar, "Z pos", TW_TYPE_FLOAT, setLightZpos, getLightZpos , &globals.lightdir_z, " step=0.01 group=Light label='z-pos' ");

    TwAddVarRW(myBar, "Ambient color", TW_TYPE_COLOR3F, &globals.ambientColor, "group=Material colormode=hls");

    TwAddVarRW(myBar, "Diffuse color", TW_TYPE_COLOR3F, &globals.diffuseColor, "group=Material colormode=hls");
    TwAddVarCB(myBar, "Diffuse intensity", TW_TYPE_FLOAT,setDifflvl, getDifflvl, &globals.material_kd, "group=Material step=0.01");

    TwAddVarRW(myBar, "Outline color", TW_TYPE_COLOR3F, &globals.outlineColor, "group=Material colormode=hls");
    TwAddVarCB(myBar, "Outline intensity", TW_TYPE_FLOAT, setOutlinelvl, getOutlinelvl , &globals.outline_intensity, " step=0.01 min=0.0 max=1.0 group=Material");

    TwAddVarCB(myBar, "Background color", TW_TYPE_COLOR3F, setBgcolorCallBack, getBgcolorCallBack, &globals.bg_color[0], "group=Misc colormode=hls");

    TwAddVarCB(myBar, "Color levels", TW_TYPE_INT8, setColorlvl, getColorlvl , &globals.colorlvl, " step=1 min=2 max=6 group=Material");


    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    // Initialize rendering
    init();

    // Start rendering loop
    while (!glfwWindowShouldClose(window)) {
        display();
        TwDraw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
