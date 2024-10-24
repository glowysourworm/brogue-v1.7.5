#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
//uniform float mixWeight;

void main()
{
    // Sample the frame texture
    //vec4 output0 = texture(frame0Texture, currentTexture);
    //vec4 output1 = texture(frame1Texture, currentTexture);

    //outputColor = mix(output0, output1, weight);

    //outputColor = texelFetch(frame0Texture, currentTextureUI, 0);
    outputColor = texture(frame0Texture, currentTextureUV);

    //outputColor = vec4(1,0,0,0);
}