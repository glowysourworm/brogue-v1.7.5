#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D frameTexture;
uniform sampler2D flameTexture;
uniform sampler2D titleMaskTexture;
uniform sampler2D uiTexture;
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
    vec4 frameOutput = texture(frameTexture, currentTextureUV);
    vec4 flameOutput = texture(flameTexture, currentTextureUV);
    vec4 maskOutput = texture(titleMaskTexture, currentTextureUV);
    vec4 uiOutput = texture(uiTexture, currentTextureUV);

    // Favor the UI controls first (one of these may be turned on)
    if (openMenuColor.w > 0)
        outputColor = openMenuColor;

    else if (playbackMenuColor.w > 0)
        outputColor = playbackMenuColor;

    else if (uiOutput.w > 0)
        outputColor = uiOutput;

    // Blend in the backgrounds
    if (outputColor.w < 1.0 && length(maskOutput) == 0)
    {
        // Any room left in the alpha channel is used for the background(s)
        outputColor = mix(outputColor, flameOutput, 1 - outputColor.w);
    }
}