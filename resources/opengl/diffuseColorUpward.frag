#version 430 core

#define PI 3.1415926535897932384626433832795

// Output of the shader is the RBGA formatted color for the fragment (pixel.. or rasterized unit fragment)
//
out vec4 outputColor;

in vec2 currentVertex;
in vec2 currentTextureUV;
in vec2 currentGlyphUV;
in vec4 currentForegroundColor;
in vec4 currentBackgroundColor;
in float currentOutputSelector;

// Try and use a 2D sampler to work with the texture. The active texture should be the 0th texture.
uniform sampler2D flameTexture;
uniform vec2 cellSizeUV;
uniform ivec2 cellSizeUI;
uniform float weight;
uniform float fadePeriodTime;   // [0,1] time ratio for the color fade
//uniform int nextColorNumber;
uniform int fadePeriodRandom1;   // Random constant integers for the fade period
uniform int fadePeriodRandom2;
uniform vec4 flameBottomColor1;
uniform vec4 flameBottomColor2;
uniform vec4 flameBottomColor3;
uniform vec4 flameTitleColor1;
uniform vec4 flameTitleColor2;
uniform vec4 flameTitleColor3;

int pseudoRandom(int lastRandom)
{
    // https://en.wikipedia.org/wiki/Linear_congruential_generator
    // Numerical Recipes
    //
    int m = int(pow(2, 6));
    int a = 16645;
    int c = 101323;

    return ((a * lastRandom) + c) % m;
}
float nextFloat(out int lastRandom, float mean, float deviation)
{
    lastRandom = pseudoRandom(lastRandom);
    float value = lastRandom / float(pow(2, 6));

    // Min / Max color channel
    value = clamp(value, 0, 1);

    return clamp(value, mean - deviation, mean + deviation);
}
vec4 nextColor(vec4 nearColor, int lastRandom, float noise)
{
    vec4 result = vec4(0,0,0,0);
    int m = int(pow(2, 6));

    result.x = nextFloat(lastRandom, nearColor.x, noise);
    result.y = nextFloat(lastRandom, nearColor.y, noise);
    result.z = nextFloat(lastRandom, nearColor.z, noise);
    result.w = nextFloat(lastRandom, nearColor.w, noise);

    return result;
}
// spatialFrequency:    Number of "candles" across the bottom
// timeDivisor:         Divisor for the number of sinusoidal "waves" during the fade period. 
//                      Depends on the FPS rate of this shader.
// heatFloor:           Minimum value of the heat envelope.
//
float calculateHeatEnvelope(int column, 
                            int row, 
                            float width, 
                            float spatialFrequency, 
                            float timeSpatialDivisor, 
                            float timeAmplitudeDivisor, 
                            float heatFloor)
{
	// Column Sine-Wave Pseudo-Heat Function
	//
    float slope = (1 - heatFloor) / (width / 2);
	float triangleLeft = (slope * column) + heatFloor;
	float triangleRight = ((-1 * slope) * column) - heatFloor + 2;
	float heatEnvelope = (column < (width / 2.0f)) ? triangleLeft : triangleRight;

    float timeArgSpatial = 0.05 * sin(2 * PI * fadePeriodTime * timeSpatialDivisor);
    //float timeArgSpatial = 0;
    float timeArgAmplitude = fadePeriodTime * timeAmplitudeDivisor;
	
    float spaceArg = spatialFrequency * (column / width) / 2;
    float sinArg = spaceArg + timeArgSpatial;

	float sinValue = clamp(abs(sin(2 * PI * sinArg)), heatFloor, 1);
    float sinAmplitude = clamp(abs(cos(2 * PI * timeArgAmplitude)), heatFloor, 1);
    //float sinAmplitude = 1;

    return heatEnvelope * sinAmplitude * sinValue;
}

void main()
{
    /*
        Design:  The heat sources are at specific cells / tiles in the grid. These will have all
                 three background colors set to certain values. The next color number says which
                 two values should be interpolated along the fade period timeline.

                 During the fade period, the color should blend from the previous value to the 
                 next value.

                 The color randomization will happen with a random integer uniform + the (column, row)
                 location of the cell - calculated in texel space. This will offset the color index.

                 Other color randomization will happen as a function of the (column, row) + the
                 number randomization + a noise float [0, 1] to get a deterministic (pseudo random
                 to the user) color "near" the index color. This will blend in nicely over the
                 fade period.
    */

    // Texel Space - (CRITICAL!) This fixed a sampling error trying to feedback other samples into the 
    //                           frame buffer. So, for the title menu, the round-off errors were apparent
    //                           which wasn't easy to fix, until other issues were eliminated.
    //
    ivec2 locationUI = ivec2(floor(currentTextureUV.x / cellSizeUV.x), floor(currentTextureUV.y / cellSizeUV.y));

    // Calculate other adjacent cells in UV -> UI coordinates (careful for inverted y-coordinate)
    //
    ivec2 southLocationUI = ivec2(locationUI.x, locationUI.y - 1);
    ivec2 southEastLocationUI = ivec2(locationUI.x + 1, locationUI.y - 1);
    ivec2 southWestLocationUI = ivec2(locationUI.x - 1, locationUI.y - 1);

    bool bottomRow = (currentTextureUV.y - cellSizeUV.y) < 0;

    // Heat Source Region
    if (length(currentBackgroundColor) > 0)
    {
        /*
            Heat Source Layering:  1, 3, 5, 7 (spatial "frequencies" = numer of "candles" across the bottom)
                                   Use more noise at higher frequencies; and mix the result to produce a nice
                                   bellowing output.

                                   Each layer has the 6 color calculations based on location - which may be
                                   filtered again with added noise - and should be deterministically calculated
                                   to repeat right along with the base colors - so the unfolding should be
                                   continuous; and seeded by the two fade period random integers.
        */

        // Pseudo Random Color Number = function(fade period random, locationUI, static background color)
        //
        float noiseLow = 0.15;
        float noiseMed = 0.25;
        float noiseHigh = 0.35;
        float heatFloor1 = 0.35;
        float heatFloor2 = 0.0;
        float heatFloor3 = 0.0;
        float heatOverdrive = 1.25;
        float colorOverdrive = 1;

        int spatialFrequency1 = 1;
        int spatialFrequency2 = 1;
        int spatialFrequency3 = 11;

        int temporalFrequency1 = 1;      // Fade Period Multiplier (speed up the relative updating of the other color streams)
        int temporalFrequency2 = 2;
        int temporalFrequency3 = 5;

        float spatialWaveSpeed1 = 1;     // Need to work this out; but should cause "candle waving"
        float spatialWaveSpeed2 = 0;     
        float spatialWaveSpeed3 = 1;
        
        float bellowSpeed1 = 0.01;          // This is an amplitude bellow based on the period; and will utilize the heat floor.
        float bellowSpeed2 = 0;
        float bellowSpeed3 = 0;

        vec4 currentBackgroundColor1 = bottomRow ? flameBottomColor1 : flameTitleColor1;
        vec4 currentBackgroundColor2 = bottomRow ? flameBottomColor2 : flameTitleColor2;
        vec4 currentBackgroundColor3 = bottomRow ? flameBottomColor3 : flameTitleColor3;

        vec4 lastBackground1 = nextColor(currentBackgroundColor1, fadePeriodRandom2 * locationUI.x, noiseLow);
        vec4 lastBackground2 = nextColor(currentBackgroundColor2, fadePeriodRandom2 * locationUI.x, noiseMed);
        vec4 lastBackground3 = nextColor(currentBackgroundColor3, fadePeriodRandom2 * locationUI.x, noiseHigh);

        // -> Feeds the next background color set (as long as fadePeriodRandom1 (feeds) -> fadePeriodRandom2)
        //
        vec4 nextBackground1 = nextColor(currentBackgroundColor1, fadePeriodRandom1 * locationUI.x, noiseLow);
        vec4 nextBackground2 = nextColor(currentBackgroundColor2, fadePeriodRandom1 * locationUI.x, noiseMed);
        vec4 nextBackground3 = nextColor(currentBackgroundColor3, fadePeriodRandom1 * locationUI.x, noiseHigh);

        // Apply Heat Envelope (calculating by a constant shouldn't alter the determinability)
        float output1 = calculateHeatEnvelope(locationUI.x, locationUI.y, 100, spatialFrequency1, spatialWaveSpeed1, bellowSpeed1, heatFloor1) * heatOverdrive;    // Low Spatial Frequency / Low Noise
        float output2 = calculateHeatEnvelope(locationUI.x, locationUI.y, 100, spatialFrequency2, spatialWaveSpeed2, bellowSpeed2, heatFloor2) * heatOverdrive;    // Med Spatial Frequency / Med Noise
        float output3 = calculateHeatEnvelope(locationUI.x, locationUI.y, 100, spatialFrequency3, spatialWaveSpeed3, bellowSpeed3, heatFloor3) * heatOverdrive;    // High Spatial Frequency / High Noise

        vec4 candleOutput1 = output1 * mix(lastBackground1, nextBackground1, fadePeriodTime) * colorOverdrive;
        vec4 candleOutput2 = output2 * mix(lastBackground2, nextBackground2, fadePeriodTime) * colorOverdrive;
        vec4 candleOutput3 = output3 * mix(lastBackground3, nextBackground3, fadePeriodTime) * colorOverdrive;

        outputColor = mix(outputColor, candleOutput1, 1) * heatOverdrive;
        outputColor = mix(outputColor, candleOutput2, candleOutput2.w) * heatOverdrive;
        outputColor = mix(outputColor, candleOutput3, candleOutput3.w) * heatOverdrive;
    }

    // Diffusion Region
    else
    {
        ivec2 currentUI = ivec2(locationUI.x * cellSizeUI.x, locationUI.y * cellSizeUI.y);
        ivec2 southUI = ivec2(southLocationUI.x * cellSizeUI.x, southLocationUI.y * cellSizeUI.y);
        ivec2 southEastUI = ivec2(southEastLocationUI.x * cellSizeUI.x, southEastLocationUI.y * cellSizeUI.y);
        ivec2 southWestUI = ivec2(southWestLocationUI.x * cellSizeUI.x, southWestLocationUI.y * cellSizeUI.y);

        // Current Color
        vec4 currentColor = texelFetch(flameTexture, currentUI, 0);
        vec4 southColor = texelFetch(flameTexture, southUI, 0);
        vec4 southEastColor = texelFetch(flameTexture, southEastUI, 0);
        vec4 southWestColor = texelFetch(flameTexture, southWestUI, 0);

        // Final Color Mix
        vec4 southEWColor = vec4(0,0,0,0);

        southEWColor = mix(southWestColor, southEastColor, 0.5);
        southColor = mix(southColor, southEWColor, 0.5);
        //outputColor = mix(currentColor, southColor, 1 - currentColor.w);

        outputColor = mix(currentColor, southColor, 0.95);

        // Color Fade 5% per iteration
        outputColor = mix(outputColor, vec4(0,0,0,0), 0.05);
    }

    // Skip the edges
    //if (currentTextureS.y < 0 || currentTextureSW.x < 0 || currentTextureSE.x > 1)
    //    outputColor = texture(flameTexture, currentTexture);

//    else
//    {
//        vec2 cellSizeUV = vec2(currentTextureSE.x - currentTextureS.x, currentTexture.y - currentTextureS.y);
//
//        // Texel Space
//        ivec2 locationUI = ivec2(floor(currentTexture.x / cellSizeUV.x), floor(currentTexture.y / cellSizeUV.y));
//        ivec2 southLocationUI = ivec2(floor(currentTextureS.x / cellSizeUV.x), floor(currentTextureS.y / cellSizeUV.y));
//        ivec2 southEastLocationUI = ivec2(floor(currentTextureSE.x / cellSizeUV.x), floor(currentTextureSE.y / cellSizeUV.y));
//        ivec2 southWestLocationUI = ivec2(floor(currentTextureSW.x / cellSizeUV.x), floor(currentTextureSW.y / cellSizeUV.y));
//
//        //ivec2 cellSizeUI = ivec2(floor(1.0 / cellSizeUV.x), floor(1.0 / cellSizeUV.y));
//        //ivec2 cellSizeUI = ivec2(16, 27);
//
//        ivec2 currentUI = ivec2(locationUI.x * cellSizeUI.x, locationUI.y * cellSizeUI.y);
//        ivec2 southUI = ivec2(southLocationUI.x * cellSizeUI.x, southLocationUI.y * cellSizeUI.y);
//        ivec2 southEastUI = ivec2(southEastLocationUI.x * cellSizeUI.x, southEastLocationUI.y * cellSizeUI.y);
//        ivec2 southWestUI = ivec2(southWestLocationUI.x * cellSizeUI.x, southWestLocationUI.y * cellSizeUI.y);
//
////        // UV Space
////        vec4 currentColor = texture(flameTexture, currentTexture);
////        vec4 southColor = texture(flameTexture, currentTextureS);
////        vec4 southWestColor = texture(flameTexture, currentTextureSW);
////        vec4 southEastColor = texture(flameTexture, currentTextureSE);
////
////        vec4 southEWColor = mix(southWestColor, southEastColor, 0.5);
//
//        // Texel Space
//        vec4 currentColor = texelFetch(flameTexture, currentUI, 0);
//        vec4 southColor = texelFetch(flameTexture, southUI, 0);
//        vec4 southEastColor = texelFetch(flameTexture, southEastUI, 0);
//        vec4 southWestColor = texelFetch(flameTexture, southWestUI, 0);
//
//        // Final Color Mix
//        vec4 southEWColor = vec4(0,0,0,0);
//
//        southEWColor = mix(southWestColor, southEastColor, 0.5);
//        southColor = mix(southColor, southEWColor, 0.5);
//        outputColor = mix(currentColor, southColor, 1 - currentColor.w);
//
//        // Color Fade 5% per iteration
//        // else
//            outputColor = mix(outputColor, vec4(0,0,0,0), 0.1);
//    }
//
//    if (length(outputColor) < 0.05)
//        outputColor = vec4(0,0,0,0);
}