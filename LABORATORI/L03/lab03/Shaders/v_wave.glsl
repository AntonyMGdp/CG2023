// Vertex shader: Wave shading
// ================
#version 330

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M; // position*rotation*scaling
uniform float t;

//uniform vec4 Color;
in vec4 Color;
out float height;


void main()
{
    float omega = 0.001;
    float a = 0.1;
    vec4 position = vec4(aPos, 1.0);
    height = a * sin(omega * t + 10 * position.x) * sin(omega * t+ 10 * position.z) ; 
    position.y = height;
    gl_Position = P * V * M *  position;
    
} 