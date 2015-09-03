// Fragment shader
#version 150

//Fragment shader contour detection
//Blinn-Phong with same color for RGB

out vec4 FragColor;
uniform vec3 lightDir;
uniform vec3 eye_position;

uniform float material_kd;
uniform float outline_intensity;

in vec3 world_pos;
in vec3 world_normal;

//number of colorlvls
//for diffuse color
// perhaps 3 colors

uniform int colorlvl;
float scaleFactor = 1.0 / colorlvl;
uniform vec3 diffuseColor;
uniform vec3 ambientColor;
uniform vec3 outlineColor;

vec3 color;

void main(){

	vec3 L = normalize( lightDir - world_pos);
	vec3 V = normalize( eye_position - world_pos);
  vec3 H = normalize(L + V );

	float diffuse = max(0, dot(L,world_normal));
	vec3 diffuseColor = diffuseColor * material_kd * floor(diffuse * colorlvl) * scaleFactor;

  float edgeDetection = (dot(V, world_normal) >  outline_intensity) ? 1 : 0;

	if(edgeDetection == 1)
		color = ambientColor + diffuseColor;
	else
		color = outlineColor;

  FragColor = vec4(color, 1);
}
