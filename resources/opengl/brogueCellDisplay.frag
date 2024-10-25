#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;
in vec4 currentBackgroundColor;
in float currentOutputSelector;

// Current Framebuffer
//
uniform sampler2D frame0Texture;

void main()
{
    // Dipslay Cell
    //
    if (currentOutputSelector == 1)
        outputColor = currentBackgroundColor;

    // Display Frame buffer
    //
    else if (currentOutputSelector == 2)
        outputColor = texture(frame0Texture, currentTextureUV);

    // No Display
    else
        outputColor = vec4(0, 0, 0, 0);
        
}