// Fragment shader: : Wave shading
// ================
#version 330

// Ouput data
out vec4 FragColor;

uniform vec4 Color;
in float height;

void main()
{
   FragColor = vec4(0,0,height+.5, 1);
}
