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

void main()
{
    // Glyphs: The character for a cell is located in the glyph sheet - for every cell. So, if it
    //         is "white space" (' '), then you'll get the glyph location for the empty space. this
    //         should be a transparent color. So, we'll skip it if it's transparent.
    //
    vec4 glyphColor = texture(fontTexture, currentGlyphUV);
    float glyphThreshold = 0.03;

    // Dipslay Cell
    //
    if (currentOutputSelector == 1)
    {
        if (glyphColor.x > glyphThreshold && glyphColor.y > glyphThreshold && glyphColor.z > glyphThreshold && glyphColor.w > glyphThreshold)
            outputColor = mix(glyphColor, currentForegroundColor, 0.5);
        else
            outputColor = currentBackgroundColor;
    }

    // Display Frame buffer (DisplayCurrentFrame)
    //
    else if (currentOutputSelector == 2)
        outputColor = texture(frame0Texture, currentTextureUV);

    // No Display
    else
        outputColor = vec4(0, 0, 0, 0);
        
}