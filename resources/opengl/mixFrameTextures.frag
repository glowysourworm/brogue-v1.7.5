#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frame0Texture;
uniform sampler2D frame1Texture;
uniform sampler2D openMenuTexture;
uniform sampler2D playbackMenuTexture;
uniform vec4 openMenuClipXY;
uniform vec2 openMenuScrollUV;
uniform vec4 playbackMenuClipXY;
uniform vec2 playbackMenuScrollUV;

void main()
{
    // Scrolling Behavior:  Add on an additional scroll offset to the current texture coordinate
    //
    vec2 openMenuUV = currentTextureUV + openMenuScrollUV;
    vec2 playbackUV = currentTextureUV + playbackMenuScrollUV;

    vec4 openMenuColor = texture(openMenuTexture, openMenuUV);
    vec4 playbackMenuColor = texture(playbackMenuTexture, playbackUV);

    // Clipping (x, y, width, height) (where (x,y) is the topLeft vertex)
    if (length(openMenuClipXY) > 0)
    {
        if (currentVertex.x < openMenuClipXY.x ||
            currentVertex.x > openMenuClipXY.x + openMenuClipXY.z ||
            currentVertex.y < openMenuClipXY.y - openMenuClipXY.w ||
            currentVertex.y > openMenuClipXY.y)
        {
            openMenuColor = vec4(0,0,0,0);
        }
    }

    if (length(playbackMenuClipXY) > 0)
    {
        if (currentVertex.x < playbackMenuClipXY.x ||
            currentVertex.x > playbackMenuClipXY.x + playbackMenuClipXY.z ||
            currentVertex.y < playbackMenuClipXY.y - playbackMenuClipXY.w ||
            currentVertex.y > playbackMenuClipXY.y)
        {
            playbackMenuColor = vec4(0,0,0,0);
        }
    }

    // Finish output
    outputColor = vec4(0,0,0,0);

    // Sample the frame texture
    vec4 output0 = texture(frame0Texture, currentTextureUV);
    vec4 output1 = texture(frame1Texture, currentTextureUV);

    // Favor the UI controls first (one of these may be turned on)
    if (openMenuColor.w > 0)
        outputColor = openMenuColor;

    else if (playbackMenuColor.w > 0)
        outputColor = playbackMenuColor;

    // Blend in the backgrounds
    if (outputColor.w < 1.0)
    {
        // Favor frame texture 1 (on top of the background)
        vec4 background = mix(output1, output0, 1 - output1.w);

        // Any room left in the alpha channel is used for the background(s)
        outputColor = mix(outputColor, background, 1 - outputColor.w);
    }
}