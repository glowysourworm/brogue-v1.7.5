#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec4 currentBackgroundColor;

uniform vec4 clipXY;
uniform vec2 scrollXY;

void main()
{
    // Clipping (x, y, width, height) (where (x,y) is the topLeft vertex)
    if (length(clipXY) > 0)
    {
        if (currentVertex.x < clipXY.x ||
            currentVertex.x > clipXY.x + clipXY.z ||
            currentVertex.y < clipXY.y - clipXY.w ||
            currentVertex.y > clipXY.y)
        {
            outputColor = vec4(0,0,0,0);
            return;
        }
    }

    outputColor = currentBackgroundColor;
}