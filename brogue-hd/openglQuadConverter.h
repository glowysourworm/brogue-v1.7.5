#pragma once

#include "simpleGlData.h"

namespace brogueHd::frontend::opengl
{
	struct openglQuadConverter
	{
		openglQuadConverter()
		{
			_quadWidth = 0;
			_quadHeight = 0;
			_viewWidth = 0;
			_viewHeight = 0;
		}
		openglQuadConverter(float quadWidth, float quadHeight, float viewWidth, float viewHeight)
		{
			_quadWidth = quadWidth;
			_quadHeight = quadHeight;
			_viewWidth = viewWidth;
			_viewHeight = viewHeight;
		}
		openglQuadConverter(const openglQuadConverter& copy)
		{
			copyImpl(copy);
		}
		~openglQuadConverter(){};

		void operator=(const openglQuadConverter& copy)
		{
			copyImpl(copy);
		}

	public:

		/// <summary>
		/// Creates quad for the tile-based view based on the grid locator
		/// </summary>
		simpleQuad createQuadNormalizedXY_FromLocator(int column, int row)
		{
			return createQuadNormalizedXY_FromLocator(gridLocator(column, row));
		}

		/// <summary>
		/// Creates quad for the tile-based view based on the grid locator
		/// </summary>
		simpleQuad createQuadNormalizedUV_FromLocator(int column, int row)
		{
			return createQuadNormalizedUV_FromLocator(gridLocator(column, row));
		}

		/// <summary>
		/// Creates quad for the tile-based view based on the grid locator
		/// </summary>
		simpleiQuad createQuadUV_FromLocator(int column, int row)
		{
			return createQuadUV_FromLocator(gridLocator(column, row));
		}

		/// <summary>
		/// Creates quad for the tile-based view based on the grid locator
		/// </summary>
		simpleQuad createQuadNormalizedXY_FromLocator(const gridLocator& locator)
		{
			// UI Coordinates
			float pixelX = locator.column * _quadWidth;
			float pixelY = locator.row * _quadHeight;

			return createQuadNormalizedXY(pixelX, pixelY, _quadWidth, _quadHeight);
		}

		/// <summary>
		/// Creates quad for the tile-based view based on the grid locator
		/// </summary>
		simpleQuad createQuadNormalizedUV_FromLocator(const gridLocator& locator)
		{
			// UI Coordinates
			float pixelX = locator.column * _quadWidth;
			float pixelY = locator.row * _quadHeight;

			return createQuadNormalizedUV(pixelX, pixelY, _quadWidth, _quadHeight);
		}

		/// <summary>
		/// Creates quad for the tile-based view based on the grid locator
		/// </summary>
		simpleiQuad createQuadUV_FromLocator(const gridLocator& locator)
		{
			// UI Coordinates
			float pixelX = locator.column * _quadWidth;
			float pixelY = locator.row * _quadHeight;

			return createQuadUV(pixelX, pixelY, _quadWidth, _quadHeight);
		}

	public:

		/// <summary>
		/// Creates (freehand) Quad from UI coordinates converting to XY-Normalized
		/// </summary>
		/// <param name="pixelX">UI-coordinate X</param>
		/// <param name="pixelY">UI-coordinate Y</param>
		/// <param name="pixelWidth">UI-coordinate Width</param>
		/// <param name="pixelHeight">UI-coordinate Height</param>
		simpleQuad createQuadNormalizedXY(float pixelX, float pixelY, float pixelWidth, float pixelHeight)
		{
			vec2 topLeft = convertToNormalizedXY(pixelX, pixelY);
			vec2 bottomRight = convertToNormalizedXY(pixelX + pixelWidth, pixelY + pixelHeight);

			return simpleQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}

		/// <summary>
		/// Creates GLQuad from UI coordinates converting to UV-Normalized scene coordinates - used for texture overlays or post-processing
		/// </summary>
		/// <param name="pixelX">UI Pixel X</param>
		/// <param name="pixelY">UI Pixel Y</param>
		/// <param name="sceneWidth">UI scene width</param>
		/// <param name="sceneHeight">UI scene height</param>
		/// <returns>UV coordinate GLQuad with respect to the scene</returns>
		simpleQuad createQuadNormalizedUV(float pixelX, float pixelY, float pixelWidth, float pixelHeight)
		{
			vec2 topLeft = convertToNormalizedUV(pixelX, pixelY);
			vec2 bottomRight = convertToNormalizedUV(pixelX + pixelWidth, pixelY + pixelHeight);

			return simpleQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}

		/// <summary>
		/// Creates a GL quad that is in UV coordinates; but scaled to the UI size. This is used with texture rectangle / texelFetch.
		/// </summary>
		/// <param name="pixelX">UI Pixel X</param>
		/// <param name="pixelY">UI Pixel Y</param>
		/// <param name="pixelWidth">UI Pixel Width</param>
		/// <param name="pixelHeight">UI Pixel Height</param>
		simpleiQuad createQuadUV(float pixelX, float pixelY, float pixelWidth, float pixelHeight)
		{
			ivec2 topLeft = convertToUV(pixelX, pixelY);
			ivec2 bottomRight = convertToUV(pixelX + pixelWidth, pixelY + pixelHeight);

			return simpleiQuad(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
		}

	public:

		/// <summary>
		/// Converts from UI coordinates to XY-Normalized
		/// </summary>
		/// <param name="pixelX">UI-coordinate X</param>
		/// <param name="pixelY">UI-coordinate Y</param>
		vec2 convertToNormalizedXY(float pixelX, float pixelY)
		{
			float normalizedY = ((pixelY / (float)_viewHeight) * -2.0f) + 1.0f;
			float normalizedX = ((pixelX / (float)_viewWidth) * 2.0f) - 1.0f;

			return vec2(normalizedX, normalizedY);
		}

		/// <summary>
		/// Converts from UI coordinates to UV-Normalized coordinates - used for texture overlays or post-processing
		/// </summary>
		/// <param name="pixelX">UI Pixel X</param>
		/// <param name="pixelY">UI Pixel Y</param>
		vec2 convertToNormalizedUV(float pixelX, float pixelY)
		{
			// Have to invert the Y coordinate to work with the texture
			float normalizedU = pixelX / (float)_viewWidth;
			float normalizedV = 1 - (pixelY / (float)_viewHeight);

			return vec2(normalizedU, normalizedV);
		}

		/// <summary>
		/// Converts from UI coordinates to UV coordinates (NON-NORMALIZED). This is used for texture rectangles / texelFetch. The
		/// result will be in UV coordinates; but scaled to the scene UI size.
		/// </summary>
		/// <param name="pixelX">UI Pixel X</param>
		/// <param name="pixelY">UI Pixel Y</param>
		ivec2 convertToUV(float pixelX, float pixelY)
		{
			return ivec2(pixelX, _viewHeight - pixelY);
		}

	public:

		float getQuadWidth() const
		{
			return _quadWidth;
		}
		float getQuadHeight() const
		{
			return _quadHeight;
		}
		float getViewWidth() const
		{
			return _viewWidth;
		}
		float getViewHeight() const
		{
			return _viewHeight;
		}

	private:

		void copyImpl(const openglQuadConverter& copy)
		{
			_quadWidth = copy.getQuadWidth();
			_quadHeight = copy.getQuadHeight();
			_viewWidth = copy.getViewWidth();
			_viewHeight = copy.getViewHeight();
		}

	private:

		float _quadWidth;
		float _quadHeight;
		float _viewWidth;
		float _viewHeight;
	};
}