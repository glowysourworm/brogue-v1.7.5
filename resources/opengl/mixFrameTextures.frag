#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTexture;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform sampler2D frame1Texture;
uniform float weight;

void main()
{
    // Sample the frame texture
    //vec4 output0 = texture(frame0Texture, currentTexture);
    //vec4 output1 = texture(frame1Texture, currentTexture);

    //outputColor = mix(output0, output1, weight);

    outputColor = texture(frame0Texture, currentTexture);
}