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
uniform sampler2D flameTexture;
uniform ivec2 cellSizeUI;
uniform float weight;

void main()
{
    // Skip the edges
    if (currentTextureS.y < 0 || currentTextureSW.x < 0 || currentTextureSE.x > 1)
        outputColor = texture(flameTexture, currentTexture);

    else
    {
        vec2 cellSizeUV = vec2(currentTextureSE.x - currentTextureS.x, currentTexture.y - currentTextureS.y);

        // Texel Space
        ivec2 locationUI = ivec2(floor(currentTexture.x / cellSizeUV.x), floor(currentTexture.y / cellSizeUV.y));
        ivec2 southLocationUI = ivec2(floor(currentTextureS.x / cellSizeUV.x), floor(currentTextureS.y / cellSizeUV.y));
        ivec2 southEastLocationUI = ivec2(floor(currentTextureSE.x / cellSizeUV.x), floor(currentTextureSE.y / cellSizeUV.y));
        ivec2 southWestLocationUI = ivec2(floor(currentTextureSW.x / cellSizeUV.x), floor(currentTextureSW.y / cellSizeUV.y));

        //ivec2 cellSizeUI = ivec2(floor(1.0 / cellSizeUV.x), floor(1.0 / cellSizeUV.y));
        //ivec2 cellSizeUI = ivec2(16, 27);

        ivec2 currentUI = ivec2(locationUI.x * cellSizeUI.x, locationUI.y * cellSizeUI.y);
        ivec2 southUI = ivec2(southLocationUI.x * cellSizeUI.x, southLocationUI.y * cellSizeUI.y);
        ivec2 southEastUI = ivec2(southEastLocationUI.x * cellSizeUI.x, southEastLocationUI.y * cellSizeUI.y);
        ivec2 southWestUI = ivec2(southWestLocationUI.x * cellSizeUI.x, southWestLocationUI.y * cellSizeUI.y);

//        // UV Space
//        vec4 currentColor = texture(flameTexture, currentTexture);
//        vec4 southColor = texture(flameTexture, currentTextureS);
//        vec4 southWestColor = texture(flameTexture, currentTextureSW);
//        vec4 southEastColor = texture(flameTexture, currentTextureSE);
//
//        vec4 southEWColor = mix(southWestColor, southEastColor, 0.5);

        // Texel Space
        vec4 currentColor = texelFetch(flameTexture, currentUI, 0);
        vec4 southColor = texelFetch(flameTexture, southUI, 0);
        vec4 southEastColor = texelFetch(flameTexture, southEastUI, 0);
        vec4 southWestColor = texelFetch(flameTexture, southWestUI, 0);

        // Final Color Mix
        vec4 southEWColor = vec4(0,0,0,0);

        southEWColor = mix(southWestColor, southEastColor, 0.5);
        southColor = mix(southColor, southEWColor, 0.5);
        outputColor = mix(currentColor, southColor, weight);

        // Color Fade 5% per iteration
        // else
            outputColor = mix(outputColor, vec4(0,0,0,0), 0.05);
    }

    if (length(outputColor) < 0.05)
        outputColor = vec4(0,0,0,0);
}