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

    //outputColor = texture(frame0Texture, currentTexture);

    // Take adjacent cell coordinates and use to blend together the output:
    //

    // Brogue Flame Menu:  Average the lower 3 cells (adding some random noise)
    //
//
//    if (currentBackgroundColor.x == 1.0f || currentBackgroundColor.z == 1.0f)
//        outputColor = vec4(0, 0, 0, 0);
//
//    else
//    {
        vec2 south = vec2(currentTexture.x, currentTexture.y - (1 / 34.0f));
//        vec2 southWest = vec2(currentTexture.x - (1 / 100.0f), currentTexture.y - (1 / 34.0f));
//        vec2 southEast = vec2(currentTexture.x + (1 / 100.0f), currentTexture.y - (1 / 34.0f));

        vec4 current = texture(frame0Texture, currentTexture);
        vec4 southColor = texture(frame0Texture, south);
//        vec4 southEastColor = texture(frame0Texture, southEast);
//        vec4 southWestColor = texture(frame0Texture, southWest);
        outputColor = mix(current, southColor, weight);
        //outputColor = mix(outputColor, southEastColor, 0.5);
        //outputColor = mix(outputColor, southWestColor, 0.5);
    //}

//    outputColor = texture(frameTexture, currentTex);
//
//    vec4 south = vec4(0,0, 0, 0);
//    vec4 southWest = vec4(0,0, 0, 0);
//    vec4 southEast = vec4(0,0, 0, 0);
//
//    if (currentTexS.z > 0.0f)
//        south = texture(frameTexture, vec2(currentTexS.x, currentTexS.y));
//
//    if (currentTexSE.z > 0.0f)
//        southEast = texture(frameTexture, vec2(currentTexSE.x, currentTexSE.y));
//
//    if (currentTexSW.z > 0.0f)
//        southWest = texture(frameTexture, vec2(currentTexSW.x, currentTexSW.y));
//
//    if (south.w > 0.0f)
//        outputColor = mix(outputColor, south, 0.01);
//
//    if (southWest.w > 0.0f)
//        outputColor = mix(outputColor, southWest, 0.01);
//
//    if (southEast.w > 0.0f)
//        outputColor = mix(outputColor, southEast, 0.01);
}