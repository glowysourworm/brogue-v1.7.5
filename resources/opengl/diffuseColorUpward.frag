#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;
in vec4 currentBackgroundColor;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform int cellWidthUI;
uniform int cellHeightUI;
uniform vec2 cellSizeUV;
uniform ivec2 sceneSizeUI;
uniform float weight;

void main()
{
    if (currentTextureUV.y - cellSizeUV.y <= 0 ||
        currentTextureUV.x - cellSizeUV.x <= 0 || 
        currentTextureUV.x + cellSizeUV.x >= 1)
        outputColor = texture(frame0Texture, currentTextureUV);

    else
    {
        // (i, j) = floor(uv * (width, height)),

        float texelX = floor(currentTextureUV.x * sceneSizeUI.x);
        float texelY = floor(currentTextureUV.y * sceneSizeUI.y);
        float texelY1 = floor((currentTextureUV.y + cellSizeUV.y) * sceneSizeUI.y);

        vec2 current = vec2(1 / texelX, 1 / texelY);
        vec2 south = vec2(1 / texelX, 1 / texelY1);

//        vec4 currentColor = texture(frame0Texture, current, 0);
//        vec4 southColor = texture(frame0Texture, south, 0);

        vec4 currentColor = texelFetch(frame0Texture, ivec2(texelX, texelY), 0);
        vec4 southColor = texelFetch(frame0Texture, ivec2(texelX, texelY1), 0);

        vec4 outputColorTest = mix(currentColor, southColor, weight);

        if (length(outputColorTest) > 0.05)
            outputColor = outputColorTest;

        else
            outputColor = texture(frame0Texture, currentTextureUV);
    }
}