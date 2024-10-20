#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
//out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTex;
in vec4 currentBackgroundColor;

in vec3 currentTexN;
in vec3 currentTexS;
in vec3 currentTexE;
in vec3 currentTexW;

in vec3 currentTexNE;
in vec3 currentTexNW;
in vec3 currentTexSE;
in vec3 currentTexSW;

// Try and use a 2D sampler to work with the texture. The adjacent texture coordinates have been
// added to this shader pipeline to sample adjacent cells on the map.
//
//uniform sampler2D frameTexture;

void main()
{
    // NOTE:  This coordinate transfer is set up with just basic 2D normalized coordinates. 
    //
    //gl_FragColor = texture(frameTexture, currentTex);

    gl_FragColor = currentBackgroundColor;

    //outputColor = vec4(currentBackgroundColor.x, currentBackgroundColor.y, currentBackgroundColor.z, 1.0);

//    vec3 redPrimaryFlameColor   = vec3(0.8, 0.2, 0.2);
//    vec3 redSecondaryFlameColor = vec3(0.4, 0.1, 0.0);
//    vec3 blueFlameColor         = vec3(0.1, 0.1, 0.8);
//    vec3 black                  = vec3(0, 0, 0);
//    vec3 white                  = vec3(1, 1, 1);
//
//    // NOTE:  This coordinate transfer is set up with just basic 2D normalized coordinates. The
//    //        texture 2D coordinates are neglected because they can be easily calculated
//    //
//
//    vec3 outputBlend;
//
//    // Current Vertex holds the "heat" value for the flame menu as the z coordinate
//    //
//    // Text Mask
//    if (currentVertex.z == 100)
//        outputBlend = black;
//
//    // Blue Flame Wreath
//    else if (currentVertex.z > 0.8)
//        outputBlend = mix(white, blueFlameColor, currentVertex.z);
//
//    else if (currentVertex.z > 0.7)
//        outputBlend = mix(blueFlameColor, redPrimaryFlameColor, currentVertex.z);
//
//    // Red Flames
//    else if (currentVertex.z > 0.3)
//        outputBlend = mix(redPrimaryFlameColor, redSecondaryFlameColor, currentVertex.z);
//
//    else if (currentVertex.z > 0.1)
//        outputBlend = mix(redSecondaryFlameColor, black, currentVertex.z);
//
//    else
//        outputBlend = black;
//
//    outputColor = vec4(outputBlend, 1.0);
}