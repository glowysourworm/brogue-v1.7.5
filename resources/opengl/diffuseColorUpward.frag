#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTexture;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform float weight;

void main()
{
    // NOTE:  This coordinate transfer is set up with just basic 2D normalized coordinates. The
    //        texture 2D coordinates are neglected because they can be easily calculated
    //

        vec2 south = vec2(currentTexture.x, currentTexture.y - (1 / 34.0f));
//        vec2 southWest = vec2(currentTexture.x - (1 / 100.0f), currentTexture.y - (1 / 34.0f));
//        vec2 southEast = vec2(currentTexture.x + (1 / 100.0f), currentTexture.y - (1 / 34.0f));

        vec4 current = texture(frame0Texture, currentTexture);
        vec4 southColor = texture(frame0Texture, south);
//        vec4 southEastColor = texture(frame0Texture, southEast);
//        vec4 southWestColor = texture(frame0Texture, southWest);
        outputColor = mix(current, southColor, weight);

}