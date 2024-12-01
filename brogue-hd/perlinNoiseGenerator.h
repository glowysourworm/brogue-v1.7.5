#pragma once
#include "noiseGenerator.h"

namespace brogueHd::backend
{
	class perlinNoiseGenerator : public noiseGenerator
	{
	public:
		perlinNoiseGenerator(randomGenerator* randomGenerator);
		~perlinNoiseGenerator() override;

        /// <summary>
        /// Initializes perlin noise generation parameters (perlin noise map is a random elevation map)
        /// </summary>
        /// <param name="resultLow">Map-Low result threshold [-1, 1]</param>
        /// <param name="resultHigh">Map-High result threshold [-1, 1]</param>
        void initialize(int meshSizeX, int meshSizeY, float resultLow, float resultHigh);

		void run(const gridRect& parentBoundary,
		         const gridRect& relativeBoundary,
		         gridCallback<bool> callback) override;

	private:

        double perlinFade(double unitValue);

	private:

		randomGenerator* _randomGenerator;

        int _meshSizeX;                              // [2, boundary width]
        int _meshSizeY;                              // [2, boundary height]
        //float _spatialFrequency;                     // User:    [0, 1]  -> [boundary dimension / 2, 2] (in cells)
        simpleRange<float>* _resultRange;            // Result:  [-1, 1]
	};

	perlinNoiseGenerator::perlinNoiseGenerator(randomGenerator* randomGenerator)
		: noiseGenerator(randomGenerator)
	{
		_randomGenerator = randomGenerator;
        _resultRange = new simpleRange(-1.0f, 1.0f);
	}

	perlinNoiseGenerator::~perlinNoiseGenerator()
	{
		delete _resultRange;
	}

    inline void perlinNoiseGenerator::initialize(int meshSizeX, int meshSizeY, float resultLow, float resultHigh)
    {
        _meshSizeX = meshSizeX;
        _meshSizeY = meshSizeY;
        //_spatialFrequency = spatialFrequency;
        _resultRange->set(resultLow, resultHigh);
    }

	void perlinNoiseGenerator::run(const gridRect& parentBoundary,
	                               const gridRect& relativeBoundary,
	                               gridCallback<bool> callback)
	{
        // Perlin Noise Generation: The definition of this is simply a random map that allows
		//                          for a smooth elevation transition between map (mesh) cells.
        //                          This transition is produced by requiring that:
        //
        //                              1) The map be continuous on the mesh cell boundary
        //                              2) The map "derivative" be continuous on the mesh cell boundary
        //
        //                          The random vectors defined on the mesh corners are used to
        //                          interpolate a smooth value in the center region of the mesh. So,
        //                          the overall effect is a smooth / random elevation map that simulates
        //                          something like a real "manifold" (non-technical term).
        //

        // Scale the frequency input
        //float scaledFrequency = simpleMath::clamp(_spatialFrequency, 0.01f, 1.0f);  // divide by zero

        _meshSizeX = simpleMath::clamp(_meshSizeX, 2, relativeBoundary.width);
        _meshSizeY = simpleMath::clamp(_meshSizeY, 2, relativeBoundary.height);

        // Use a non-offset grid for the map (making sure to translate back)
        gridRect mapBoundary(0, 0, relativeBoundary.width, relativeBoundary.height);

        // Initialize the output map
        grid<float> map(mapBoundary, mapBoundary);

        // Calculate the mesh:  Allow the mesh to fall outside the grid boundary; but only compute
        //                      the mesh values that have an effect on the grid cells.
        //

        // Mesh Cell Width:  Creates a mesh cell width between 2 and the width of the grid
        //int meshCellWidth = simpleMath::clamp((int)(1 / scaledFrequency), 2, relativeBoundary.width);

        // Mesh Cell Height:  [2, height]
        //int meshCellHeight = simpleMath::clamp((int)(1 / scaledFrequency), 2, relativeBoundary.height);

        int meshCellWidth = _meshSizeX;
        int meshCellHeight = _meshSizeY;

        // Generate mesh that hangs over the edges of the grid (AT LEAST BY ONE MESH CELL)
        //
        int meshWidth = (int)(relativeBoundary.width / (double)_meshSizeX) + 2;          // Let it overhang (see coordinate transforms)
        int meshHeight = (int)(relativeBoundary.height / (double)_meshSizeY) + 2;

        if (meshWidth * _meshSizeX < relativeBoundary.width ||
            meshHeight * _meshSizeY < relativeBoundary.height)
        {
            throw simpleException("Mesh improperly formed. Increase mesh size:  perlinNoiseGenerator.h");
        }

        // The mesh bounds don't live in the same coordinate space. This is for the grid component.
        gridRect meshBounds(0, 0, meshWidth, meshHeight);

        // Create mesh - using points to describe random unit vectors
        grid<simpleVector<float>> mesh(meshBounds, meshBounds);

        // Initialize the mesh
        for (int i = 0; i < meshWidth; i++)
        {
            // Create random vector 
            for (int j = 0; j < meshHeight; j++)
            {
                // U[-1, 1]
                float xRand = -1.0f + (_randomGenerator->next() * 2);
                float yRand = -1.0f + (_randomGenerator->next() * 2);

                mesh.set(i, j, simpleVector<float>(xRand, yRand));
            }
                
        }

        float maxValue = std::numeric_limits<float>::min();
        float minValue = std::numeric_limits<float>::max();

        perlinNoiseGenerator* that = this;

        // Iterate the map - creating interpolated values
        //
        mapBoundary.iterate([&map, &mesh, &that, &minValue, &maxValue, &meshCellWidth, &meshCellHeight] (int i, int j)
        {
            // Calculate the mesh cell coordinates for the given map cell
            //
            int meshColumn = (int)((double)i / (double)meshCellWidth);
            int meshRow = (int)((double)j / (double)meshCellHeight);

            // Calculate the actual mesh locations with respect to the grid
            // it overlays.
            //
            int meshX0 = meshColumn * meshCellWidth;
            int meshX1 = (meshColumn + 1) * meshCellWidth;
            int meshY0 = meshRow * meshCellHeight;
            int meshY1 = (meshRow + 1) * meshCellHeight;

            // Compute Distance Vectors for each point from the mesh cell vertex locations
            //
            simpleVector<float> vectorX0Y0(i - meshX0, j - meshY0);
            simpleVector<float> vectorX1Y0(meshX1 - i, j - meshY0);
            simpleVector<float> vectorX0Y1(i - meshX0, meshY1 - j);
            simpleVector<float> vectorX1Y1(meshX1 - i, meshY1 - j);

            // Compute Dot Products for each vector with the corresponding mesh vector
            //
            float dotX0Y0 = vectorX0Y0.dot(mesh.get(meshColumn, meshRow));
            float dotX1Y0 = vectorX1Y0.dot(mesh.get(meshColumn + 1, meshRow));
            float dotX0Y1 = vectorX0Y1.dot(mesh.get(meshColumn, meshRow + 1));
            float dotX1Y1 = vectorX1Y1.dot(mesh.get(meshColumn + 1, meshRow + 1));

            // Calculate influence weights for each of the mesh corners by interpolating the dot products
            // with respect to the normalized distance (from each corner). An easing function is applied 
            // as a weight to specify "how far left, or how far right" the coordinate is. (similarly in the Y-direction)
            //
            // https://mzucker.github.io/html/perlin-noise-math-faq.html
            //
            // (Better notes here)
            //
            // https://rmarcus.info/blog/2018/03/04/perlin-noise.html
            //

            // Calculate normalized coordinates for the point relative to the mesh cell
            //
            float normalizedX = (i - meshX0) / ((float)(meshX1 - meshX0));
            float normalizedY = (j - meshY0) / ((float)(meshY1 - meshY0));
            float easeX = that->perlinFade(normalizedX);
            float easeY = that->perlinFade(normalizedY);
            float weightA = simpleMath::interpolate(dotX0Y0, dotX1Y0, easeX);
            float weightB = simpleMath::interpolate(dotX0Y1, dotX1Y1, easeX);
            float weight = simpleMath::interpolate(weightA, weightB, easeY);

            map.set(i, j, weight, true);

            // Store the extreme points to normalize the output
            //
            if (weight > maxValue)
                maxValue = weight;

            if (weight < minValue)
                minValue = weight;

            return iterationCallback::iterate;
        });

        // Create normalization constants:  The equation y(x) = (x - B)(2 / (B - A)) + 1  maps [A, B] -> [-1, 1]
        //                                  So, given A := minValue and B  := maxValue, the output will be [-1, 1]
        //
        float slope = 2.0f / (maxValue - minValue);
        float intercept = 1 - ((2.0f * maxValue) / (maxValue - minValue));

        // Normalize the output and apply post processing callback
        //
        simpleRange<float>* resultRange = _resultRange;

        mapBoundary.iterate([&map, &relativeBoundary, &resultRange, &slope, &intercept, &callback] (int i, int j)
        {
            // Normalize the map value
            map.set(i, j, (slope * map.get(i, j)) + intercept, true);

            // Perform callback:  Show true values where the result is in threshold range
            //                    and be sure to translate back into their coordinates.
            //
        	callback(i + relativeBoundary.column, j + relativeBoundary.row, resultRange->contains(map.get(i, j)));

            return iterationCallback::iterate;
        });
	}

    double perlinNoiseGenerator::perlinFade(double unitValue)
    {
        if (unitValue > 1.0 ||
            unitValue < 0.0)
            throw simpleException("Trying to generate Perlin easing value out of bounds:  perlinNoiseGenerator::perlinFade");

        // https://rmarcus.info/blog/2018/03/04/perlin-noise.html (this may be worth parameterizing and playing with; but it produces
        //                                                         good results)
        //
        return (6 * simpleMath::power(unitValue, 5)) - (15 * simpleMath::power(unitValue, 4)) + (10 * simpleMath::power(unitValue, 3));
    }
}
