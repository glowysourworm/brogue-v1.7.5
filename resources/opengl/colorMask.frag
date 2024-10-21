#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec4 currentBackgroundColor;

uniform vec4 maskColor;

void main()
{
    if (currentBackgroundColor == maskColor)
        outputColor = vec4(0,0,0,0);

    else
        outputColor = currentBackgroundColor;
}