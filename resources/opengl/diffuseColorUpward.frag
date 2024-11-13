#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTexture;
in vec2 currentTextureS;
in vec2 currentTextureSE;
in vec2 currentTextureSW;
in vec4 currentBackgroundColor;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform float weight;

void main()
{
    // Skip the edges
    if (currentTextureS.y < 0 || currentTextureSW.x < 0 || currentTextureSE.x > 1)
        outputColor = texture(frame0Texture, currentTexture);

    else
    {
        vec4 currentColor = texture(frame0Texture, currentTexture);
        vec4 southColor = texture(frame0Texture, currentTextureS);
        vec4 southWestColor = texture(frame0Texture, currentTextureSW);
        vec4 southEastColor = texture(frame0Texture, currentTextureSE);

        vec4 southEWColor = mix(southWestColor, southEastColor, 0.5);

        southColor = mix(southColor, southEWColor, 0.5);

        outputColor = mix(currentColor, southColor, weight);

        // Color Fade
        outputColor = mix(outputColor, vec4(0,0,0,0), 0.05);
    }

    if (length(outputColor) < 0.05)
        outputColor = vec4(0,0,0,0);
}