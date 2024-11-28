#version 430 core

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;
in vec2 currentGlyphUV;
in vec4 currentForegroundColor;
in vec4 currentBackgroundColor;
in float currentOutputSelector;

// Current Framebuffer
//
uniform sampler2D frame0Texture;
uniform sampler2D fontTexture;
uniform float glyphThreshold;

void main()
{
    //outputColor = currentBackgroundColor;

    // Glyphs: The character for a cell is located in the glyph sheet - for every cell. So, if it
    //         is "white space" (' '), then you'll get the glyph location for the empty space. this
    //         should be a transparent color. So, we'll skip it if it's transparent.
    //
    vec4 glyphColor = texture(fontTexture, currentGlyphUV);
    vec4 frameSample = texture(frame0Texture, currentTextureUV);

    // Dipslay Cell
    //
    if (length(currentBackgroundColor) > 0 || length(glyphColor) > glyphThreshold)
    {
        if (length(glyphColor) > glyphThreshold)
            outputColor = mix(glyphColor, currentForegroundColor, 0.5);
        else
            outputColor = currentBackgroundColor;
    }

    // Display Frame buffer (DisplayCurrentFrame)
    //
    else if (length(frameSample) > 0)
        outputColor = frameSample;

    // No Display
    else
        outputColor = vec4(0, 0, 0, 0);
        
}