#version 330 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec3 currentVertex;
//in vec2 currentTex;

// This is the combined scene texture - which is the scene images rendered with the base lighting
// applied (ambient + accent)
//
//uniform sampler2D sceneBaseTexture;

void main()
{
    // NOTE:  This coordinate transfer is set up with just basic 2D normalized coordinates. The
    //        texture 2D coordinates are neglected because they can be easily calculated
    //

    // Current Vertex holds the "heat" value for the flame menu as the z coordinate
    //
    if (currentVertex.z == 1)
        outputColor = vec4(1.0, 0.0, 0.0, 0.0);
    else if (currentVertex.z == -1)
        outputColor = vec4(0.0, 0.0, 1.0, 0.0);
    else
        outputColor = vec4(0.0, 0.0, 0.0, 0.0);
}