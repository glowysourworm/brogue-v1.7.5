#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTex;

// This is the texture of the scene drawn with full visibility and lighting (Light.White)
//
uniform sampler2D frameBlendTexture;

void main()
{
    // Sample the frame texture
    outputColor = texture(frameBlendTexture, currentTex);   
}