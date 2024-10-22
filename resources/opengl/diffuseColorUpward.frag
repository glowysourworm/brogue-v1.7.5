#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTexture;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform vec2 cellSize;
uniform float weight;

void main()
{
    // Check scene boundary: UV coordinates
    //
    if (currentTexture.y - cellSize.y < 0 ||
        currentTexture.x - cellSize.x < 0 || 
        currentTexture.x + cellSize.x > 1)
        outputColor = texture(frame0Texture, currentTexture);

    else
    {
        // There's more to learn about texture sampling. Sub-texel precision must be
        // calculated when we're wandering off .. er..umm.. the GL.
        //
        ivec2 current = ivec2(currentTexture);
        ivec2 south = ivec2(currentTexture.x, currentTexture.y - cellSize.y);

        //vec2 current = currentTexture;
        //vec2 south = vec2(currentTexture.x, currentTexture.y - cellSize.y);

//        ivec2 southEast = ivec2(currentTexture.x - cellSize.x, currentTexture.y + cellSize.y);
//        ivec2 southWest = ivec2(currentTexture.x + cellSize.x, currentTexture.y + cellSize.y);

        //vec4 currentColor = texture(frame0Texture, current);
        //vec4 southColor = texture(frame0Texture, south);

        vec4 currentColor = texelFetch(frame0Texture, current, 0);
        vec4 southColor = texelFetch(frame0Texture, south, 0);
//        vec4 southEastColor = texelFetch(frame0Texture, southEast, 0);
//        vec4 southWestColor = texelFetch(frame0Texture, southWest, 0);

        //vec4 outputSouth = mix(southEastColor, southWestColor, 0.1);
        //vec4 outputSouth = mix(currentColor, southColor, 0.5);

        outputColor.x = (currentColor.x + southColor.x / 2.0f) * weight;
        outputColor.y = (currentColor.y + southColor.y / 2.0f) * weight;
        outputColor.z = (currentColor.z + southColor.z / 2.0f) * weight;
        outputColor.w = (currentColor.w + southColor.w / 2.0f) * weight;

        //outputColor = mix(currentColor, southColor, weight);
    }

    if (outputColor.w > 0.3f)
        outputColor.w = 0.3f;

    if (length(outputColor) < 0.1f)
        outputColor = vec4(0, 0, 0, 0);
}