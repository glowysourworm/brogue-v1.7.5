#version 430 core

// The layout keyword shows where - in terms of array "stride index" - the
// attribute is positioned.
//
layout (location = 0) in vec3 vertexXY_IsOdd;
layout (location = 1) in vec4 lineColor;

// Define a data type to be output by the shader to the next stages
//
out vec3 currentVertexXY_IsOdd;
out vec4 currentColor;

void main()
{
    // Pretty sure the point of this is to set gl_Position; and to provide output data to the
    // next pipeline stages.
    //
    gl_Position = vec4(vertexXY_IsOdd.x, vertexXY_IsOdd.y, 0, 1.0);

    // Struct data should be copied over for use by the fragment shader
    //
    currentVertexXY_IsOdd = vertexXY_IsOdd;
    currentColor = lineColor;
}