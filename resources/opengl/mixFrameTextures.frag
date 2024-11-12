#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform sampler2D frame1Texture;
uniform float mixWeight;
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

    // Sample the frame texture
    vec4 output0 = texture(frame0Texture, currentTextureUV);
    vec4 output1 = texture(frame1Texture, currentTextureUV);

    // Favor the non-background UI controls
    if (output1.w > 0)
        outputColor = mix(output1, output0, 1 - output1.w);
    else
        outputColor = output0;
}