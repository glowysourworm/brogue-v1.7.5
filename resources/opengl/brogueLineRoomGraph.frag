#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec3 currentVertexXY_IsOdd;
in vec4 currentColor;

void main()
{
    //if (currentVertexXY_IsOdd.z > 0)
        outputColor = currentColor;
   // else
    //    outputColor = vec4(0,0,0,0);
}