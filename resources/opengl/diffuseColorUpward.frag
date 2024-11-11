#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;
in vec2 currentGlyphUV;
in vec4 currentForegroundColor;
in vec4 currentBackgroundColor;
in float currentOutputSelector;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform sampler2D frame1Texture;
uniform sampler2D fontTexture;
uniform vec2 cellSizeUV;
uniform float weight;

void main()
{
//    bool bottom = currentTextureUV.y - cellSizeUV.y < 0;
//    bool left = currentTextureUV.x - cellSizeUV.x < 0;
//    bool right = currentTextureUV.x + cellSizeUV.x > 1;
//
//    // Skip the edges
//    if (bottom || left || right)
//        outputColor = texture(frame0Texture, currentTextureUV);
//
//    else
//    {
        vec2 current = currentTextureUV;
        vec2 south = vec2(currentTextureUV.x, (currentTextureUV.y - cellSizeUV.y));
        vec2 southEast = vec2(currentTextureUV.x + cellSizeUV.x, currentTextureUV.y - cellSizeUV.y);
        vec2 southWest = vec2(currentTextureUV.x - cellSizeUV.x, currentTextureUV.y - cellSizeUV.y);

        vec4 southColor = texture(frame0Texture, south);
        vec4 southWestColor = texture(frame0Texture, southWest);
        vec4 southEastColor = texture(frame0Texture, southEast);

        vec4 currentColor = texture(frame0Texture, current);

        vec4 southEWColor = mix(southWestColor, southEastColor, 0.5);

        southColor = mix(southColor, southEWColor, weight);

        outputColor = mix(currentColor, southColor, weight);
//    }
//
//    if (length(abs(outputColor)) < 0.05)
//        outputColor = vec4(0,0,0,0);
}