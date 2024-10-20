#version 330 core

// The layout keyword shows where - in terms of array "stride index" - the
// attribute is positioned.
//
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 backgroundColor;

layout (location = 3) in vec3 texN;
layout (location = 4) in vec3 texS;
layout (location = 5) in vec3 texE;
layout (location = 6) in vec3 texW;

layout (location = 7) in vec3 texNE;
layout (location = 8) in vec3 texNW;
layout (location = 9) in vec3 texSE;
layout (location = 10) in vec3 texSW;


// Define a data type to be output by the shader to the next stages
//
out vec2 currentVertex;
out vec2 currentTex;
out vec4 currentBackgroundColor;

out vec3 currentTexN;
out vec3 currentTexS;
out vec3 currentTexE;
out vec3 currentTexW;

out vec3 currentTexNE;
out vec3 currentTexNW;
out vec3 currentTexSE;
out vec3 currentTexSW;

void main()
{
    // Pretty sure the point of this is to set gl_Position; and to provide output data to the
    // next pipeline stages.
    //
    gl_Position = vec4(vertex.x, vertex.y, 0, 1.0);

    // Struct data should be copied over for use by the fragment shader
    //
    currentVertex = vertex;
    currentTex = tex;
    currentBackgroundColor = backgroundColor;

    currentTexN = texN;
    currentTexS = texS;
    currentTexE = texE;
    currentTexW = texW;

    currentTexNE = texNE;
    currentTexNW = texNW;
    currentTexSE = texSE;
    currentTexSW = texSW;
}