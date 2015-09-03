//Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;

uniform mat4 mvp, view, model;

out vec3 world_pos;
out vec3 world_normal;

void main() {

  world_pos = mat3(model) * a_position.xyz;//careful here
  world_normal = normalize(mat3(model) * a_normal);

    gl_Position = mvp * a_position;
}
