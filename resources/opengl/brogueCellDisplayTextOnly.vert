#version 430 core

// The layout keyword shows where - in terms of array "stride index" - the
// attribute is positioned.
//
layout (location = 0) in vec2 vertexXY;
layout (location = 1) in vec2 textureUV;
layout (location = 2) in vec2 glyphUV;
layout (location = 3) in vec4 foregroundColor;
layout (location = 4) in vec4 backgroundColor;
layout (location = 5) in float outputSelector;

// Define a data type to be output by the shader to the next stages
//
out vec2 currentVertex;
out vec2 currentTextureUV;
out vec2 currentGlyphUV;
out vec4 currentForegroundColor;
out vec4 currentBackgroundColor;
out float currentOutputSelector;

void main()
{
    // NOTE:  This coordinate transfer is set up with just basic 2D normalized coordinates. The
    //        texture 2D coordinates are neglected because they can be easily calculated in the 
    //        fragment shader
    //

    // Pretty sure the point of this is to set gl_Position; and to provide output data to the
    // next pipeline stages.
    //
    gl_Position = vec4(vertexXY.x, vertexXY.y, 0, 1.0);

    // Struct data should be copied over for use by the fragment shader
    //
    currentVertex = vertexXY;
    currentTextureUV = textureUV;
    currentGlyphUV = glyphUV;
    currentForegroundColor = foregroundColor;
    currentBackgroundColor = backgroundColor;
    currentOutputSelector = outputSelector;
}