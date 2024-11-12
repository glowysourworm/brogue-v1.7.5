#version 430 core

// The layout keyword shows where - in terms of array "stride index" - the
// attribute is positioned.
//
layout (location = 0) in vec2 vertexCurrent;
layout (location = 1) in vec2 textureCurrent;
layout (location = 2) in vec2 textureS;
layout (location = 3) in vec2 textureSE;
layout (location = 4) in vec2 textureSW;
layout (location = 5) in vec4 backgroundColor;

// Define a data type to be output by the shader to the next stages
//
out vec2 currentVertex;
out vec2 currentTexture;
out vec2 currentTextureS;
out vec2 currentTextureSE;
out vec2 currentTextureSW;
out vec4 currentBackgroundColor;

void main()
{
    // NOTE:  This coordinate transfer is set up with just basic 2D normalized coordinates. The
    //        texture 2D coordinates are neglected because they can be easily calculated in the 
    //        fragment shader
    //

    // Pretty sure the point of this is to set gl_Position; and to provide output data to the
    // next pipeline stages.
    //
    gl_Position = vec4(vertexCurrent.x, vertexCurrent.y, 0, 1.0);

    // Struct data should be copied over for use by the fragment shader
    //
    currentVertex = vertexCurrent;
    currentTexture = textureCurrent;
    currentTextureS = textureS;
    currentTextureSE = textureSE;
    currentTextureSW = textureSW;
    currentBackgroundColor = backgroundColor;
}