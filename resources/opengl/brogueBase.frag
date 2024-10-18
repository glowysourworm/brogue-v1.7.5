#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec3 currentBackgroundColor;

void main()
{
    outputColor = vec4(currentBackgroundColor, 1.0);
}