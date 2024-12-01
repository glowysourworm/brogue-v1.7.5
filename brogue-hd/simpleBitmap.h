#pragma once

#include "bitmap.h"
#include "color.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleBuffer.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleMaskedInt32.h"
#include "simplePixel.h"
#include "simplePixelFilter.h"
#include "simpleString.h"
#include <cstdint>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iosfwd>

namespace brogueHd::simple
{
	class simpleBitmap : simpleObject
	{
	public:

		/// <summary>
		/// (MEMORY!) Creates an instance of the png file from the specified buffer. This 
		///			  will be deleted by this instance of this class.
		/// </summary>
		simpleBitmap();
		simpleBitmap(simpleBuffer* fileBuffer,
					 simpleBuffer* colorDataBuffer,
					 BitmapFileHeader* signature,
					 BITMAPCOREHEADER* coreHeader,
					 BITMAPV5HEADER* dibHeader,
					 BitmapOptionalMasks* optionalMasks,
					 uint32_t resolvedWidth,
					 uint32_t resolvedHeight);
		simpleBitmap(const simpleBitmap& copy);
		~simpleBitmap();

		static simpleBitmap* fromFile(const simpleString& filename);
		static simpleBitmap* fromFile(const char* filename);
		void toFile(const simpleString& filename);
		void toFile(const simpleString& filename, const simpleMapColorFilter& filter);

		int pixelWidth() const
		{
			return _resolvedWidth;
		}
		int pixelHeight() const
		{
			return _resolvedHeight;
		}

		simpleBuffer* getBuffer() const
		{
			return _fileBuffer;
		}

		simpleBuffer* getColorDataBuffer() const
		{
			return _colorDataBuffer;
		}

	protected:

		BitmapFileHeader* getSignature() const
		{
			return _signatureHeader;
		}
		BITMAPCOREHEADER* getCoreHeader() const
		{
			return _coreHeader;
		}
		BITMAPV5HEADER* getDIBHeader() const
		{
			return _dibHeader;
		}
		BitmapOptionalMasks* getOptionalMasks() const
		{
			return _optionalMasks;
		}


	protected:

		void decodeColorData(const simpleMapColorFilter& filter, simpleArray<simplePixel>& output);

	private:

		static BitmapFileHeader* decodeSignature(simpleBuffer* fileBuffer);
		static BITMAPCOREHEADER* decodeCoreHeader(simpleBuffer* fileBuffer);
		static BITMAPV5HEADER* decodeHeader(simpleBuffer* fileBuffer);
		static BitmapOptionalMasks* decodeOptionalMasks(simpleBuffer* fileBuffer, int offset, bool includeAlpha);

	private:

		// NOTE: This is still a very rough first draft. There are more details for the headers and specs to be filled out correctly.

		// Bitmap Format: (see bitmap.h) [ Signature Header, DIB Header, Optional Bitmasks, Optional Color Table, Optional Gap1, Pixel Array, ...]

		BitmapFileHeader* _signatureHeader;
		BITMAPCOREHEADER* _coreHeader;		// In case of old versions, this will be tried in the header section as well.
		BITMAPV5HEADER* _dibHeader;
		BitmapOptionalMasks* _optionalMasks;

	private:

		simpleBuffer* _fileBuffer;
		simpleBuffer* _colorDataBuffer;

		uint32_t _resolvedWidth;
		uint32_t _resolvedHeight;

	};

	simpleBitmap::simpleBitmap()
	{
		_signatureHeader = nullptr;
		_dibHeader = nullptr;
		_coreHeader = nullptr;
		_fileBuffer = nullptr;
		_colorDataBuffer = nullptr;
		_optionalMasks = nullptr;
	}
	simpleBitmap::simpleBitmap(simpleBuffer* fileBuffer,
							   simpleBuffer* colorDataBuffer,
							   BitmapFileHeader* signature,
							   BITMAPCOREHEADER* coreHeader,
							   BITMAPV5HEADER* dibHeader,
							   BitmapOptionalMasks* optionalMasks,
							   uint32_t resolvedWidth,
							   uint32_t resolvedHeight)
	{
		_fileBuffer = fileBuffer;
		_colorDataBuffer = colorDataBuffer;
		_signatureHeader = signature;
		_coreHeader = coreHeader;
		_dibHeader = dibHeader;
		_optionalMasks = optionalMasks;
		_resolvedWidth = resolvedWidth;
		_resolvedHeight = resolvedHeight;
	}
	simpleBitmap::simpleBitmap(const simpleBitmap& copy)
	{
		_signatureHeader = copy.getSignature();
		_dibHeader = copy.getDIBHeader();
		_coreHeader = copy.getCoreHeader();
		_colorDataBuffer = copy.getColorDataBuffer();
		_fileBuffer = copy.getBuffer();
		_optionalMasks = copy.getOptionalMasks();
		_resolvedWidth = copy.pixelWidth();
		_resolvedHeight = copy.pixelHeight();
	}
	simpleBitmap::~simpleBitmap()
	{
		delete _signatureHeader;
		delete _dibHeader;
		delete _coreHeader;
		delete _fileBuffer;
		delete _colorDataBuffer;
	}

	simpleBitmap* simpleBitmap::fromFile(const char* filename)
	{
		return simpleBitmap::fromFile(simpleString(filename));
	}

	simpleBitmap* simpleBitmap::fromFile(const simpleString& filename)
	{
		uintmax_t fileSize = 0;
		size_t colorDataBufferSize = 0;
		char* fileBufferIn = nullptr;
		char* colorDataBufferIn = nullptr;

		try
		{
			fileSize = std::filesystem::file_size(filename.c_str());

			if (fileSize <= 0)
				return nullptr;

			fileBufferIn = new char[fileSize];

			std::ifstream stream(filename.c_str());

			stream.get(fileBufferIn, fileSize);
			stream.close();

			simpleBuffer* fileBuffer = new simpleBuffer(fileBufferIn, (int)fileSize, false);

			// NOTE:  The headers are not guaranteed to be complete! So far, I've noticed that
			//		  the DIB header is usually worked out; but the main header just has the 
			//		  preamble (signature) first two characters to identify the bitmap format.
			//
			//		  (Signature 14 bytes, DIB Header [variable] bytes, gap(!?), color data (not sure!)
			//
			//		  So, the DIB header must follow the format, then leave off to the color table
			//		  directly afterwards. (seems to be what it does usually. We'll easily find out
			//		  with some unit testing)
			// 
			//		  Possible Solution:  The gaps can be accounted for based on the compression type.
			//							  There are some extra bit masks depending on the DIB's setting.
			//
			BitmapFileHeader* signature = decodeSignature(fileBuffer);
			BITMAPCOREHEADER* coreHeader = decodeCoreHeader(fileBuffer);
			BITMAPV5HEADER* dibHeader = decodeHeader(fileBuffer);

			uint32_t resolvedWidth = dibHeader->bV5Width;
			uint32_t resolvedHeight = dibHeader->bV5Height;

			// Try recovering the image data just using the data offest
			if (signature->ImageDataOffset > 0 && signature->ImageDataOffset < fileSize)
			{
				colorDataBufferSize = fileSize - signature->ImageDataOffset;

				if (colorDataBufferSize % 4 != 0)
					throw simpleException("Invalid bitmap file decoding:  Image Data alignment not aligned to 4 byte value");

				colorDataBufferIn = new char[colorDataBufferSize];

				if (resolvedWidth < fileSize && resolvedHeight < fileSize)
				{
					if (resolvedWidth * resolvedHeight * 4 != colorDataBufferSize)
						throw simpleException("Invalid bitmap file decoding:  Image Data doesn't align with header values");
				}

				// Re-calculate the height and width of the image; and correct the dib header.
				// ASSUMING FORMAT 32-bits (R8, G8, B8, A8)
				if (resolvedWidth < fileSize)
					resolvedHeight = (uint32_t)(colorDataBufferSize / 4.0f / (float)resolvedWidth);

				else if (resolvedHeight < fileSize)
					resolvedWidth = (uint32_t)(colorDataBufferSize / 4.0f / (float)resolvedHeight);

				// Try BITMAP CORE HEADER:
				if (resolvedWidth * resolvedHeight * 4 != colorDataBufferSize)
				{
					resolvedWidth = coreHeader->bcWidth;
					resolvedHeight = coreHeader->bcHeight;

					if (resolvedWidth < fileSize)
						resolvedHeight = (uint16_t)(colorDataBufferSize / 4.0f / (float)resolvedWidth);

					else if (resolvedHeight < fileSize)
						resolvedWidth = (uint16_t)(colorDataBufferSize / 4.0f / (float)resolvedHeight);

					else
						throw simpleException("Invalid bitmap file decoding:  Image Data doesn't align with header values");
				}

				//if (resolvedWidth * resolvedHeight * 4 != colorDataBufferSize)
				//	throw simpleException("Invalid bitmap file decoding:  Image Data doesn't align with header values");

				// Direct Buffer Copy
				for (int index = signature->ImageDataOffset; index < fileSize; index++)
				{
					colorDataBufferIn[index - signature->ImageDataOffset] = fileBufferIn[index];
				}
			}
			else
				throw simpleException("Invalid bitmap file decoding:  Either corrupt file or improper use of DIB header");


			//colorDataBufferSize = dibHeader->bV5Width * dibHeader->bV5Height * 4;

			//uint32_t extraMaskHeaderSize = fileSize - colorDataBufferSize - dibHeader->bV5Size - BitmapFileHeader::HEADER_SIZE;

			//// We're still trying to learn the different format (happen-stances). So, if we know the 
			//// pixel size of the bitmap, and the file sizes, we can then try and locate the image data.
			////
			//// This would be the case for either compressed / uncompressed bitmaps.
			////
			//if (extraMaskHeaderSize <= 16)
			//{
			//	// Check for compression flag:
			//	//
			//	if (dibHeader->bV5Compression == BI_BITFIELDS)
			//	{
			//		if (extraMaskHeaderSize == 16)
			//			throw simpleException("Invalid bitmap file decoding:  The compression header supposes BI_ALPHABITFIELDS, not BI_BITFIELDS");
			//	}

			//	// Color Table:  32-bit words, one per pixel (see bitmap.h for more documentation)
			//	//
			//	colorDataBufferSize = dibHeader->bV5Width * dibHeader->bV5Height * 4;
			//	colorDataBufferIn = new char[colorDataBufferSize];
			//}

			//else
			//{
			//	throw simpleException("Invalid bitmap file decoding:  Either corrupt file or improper use of DIB header");
			//}
			//
			//BitmapOptionalMasks* optionalMasks = decodeOptionalMasks(fileBuffer, BitmapFileHeader::HEADER_SIZE + dibHeader->bV5Size, false);

			simpleBuffer* colorDataBuffer = new simpleBuffer(colorDataBufferIn, colorDataBufferSize, true);

			return new simpleBitmap(fileBuffer, colorDataBuffer, signature, coreHeader, dibHeader, nullptr, resolvedWidth, resolvedHeight);
		}
		catch (std::exception& ex)
		{
			throw simpleException(simpleExt::format("Error reading bitmap file : {}", ex.what()));
		}
	}
	void simpleBitmap::toFile(const simpleString& filename)
	{
		// TODO: NEED NO-OP FILTER
		toFile(filename, simpleMapColorFilter(colors::black(), colors::black()));
	}
	void simpleBitmap::toFile(const simpleString& filename, const simpleMapColorFilter& filter)
	{
		try
		{
			// Size of the image data should be one pixel = one word (32-bits)
			//simpleArray<simplePixel> colorData(_colorDataBuffer->getBufferSize() / 4);

			//// Decode the pixel data - applying the filter to the data
			//decodeColorData(filter, colorData);

			//// Create an output buffer
			//char* buffer = new char[_colorDataBuffer->getBufferSize()];

			//// Create a simpleBuffer to help with the output encoding
			//simpleBuffer outputBuffer(buffer, _colorDataBuffer->getBufferSize(), true);

			//// Encode all the pixel data to the stream - checking word alignment
			//for (int index = 0; index < colorData.count(); index++)
			//{
			//	outputBuffer.encode32(index * 4, colorData.get(index).getRGBA());
			//}

			std::ofstream stream(filename.c_str());

			stream.write(_fileBuffer->getBuffer(), _fileBuffer->getBufferSize());
			stream.flush();
			stream.close();

			//delete [] buffer;
		}
		catch (std::exception& ex)
		{
			throw simpleException(simpleExt::format("Error reading png file:  {}", ex.what()));
		}
	}

	BitmapFileHeader* simpleBitmap::decodeSignature(simpleBuffer* fileBuffer)
	{
		BitmapFileHeader* header = new BitmapFileHeader();

		if (fileBuffer->getChar(0) == 'B' && fileBuffer->getChar(1) == 'M')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::BM;

		else if (fileBuffer->getChar(0) == 'B' && fileBuffer->getChar(1) == 'A')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::BA;

		else if (fileBuffer->getChar(0) == 'C' && fileBuffer->getChar(1) == 'I')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::CI;

		else if (fileBuffer->getChar(0) == 'C' && fileBuffer->getChar(1) == 'P')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::CP;

		else if (fileBuffer->getChar(0) == 'I' && fileBuffer->getChar(1) == 'C')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::IC;

		else if (fileBuffer->getChar(0) == 'P' && fileBuffer->getChar(1) == 'T')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::PT;

		else
			throw simpleException("Unhandled bitmap signature type:  simpleBitmap.h");

		// Reserved data not needed:  Just grab these data points at their specified offsets
		//
		header->Signature = fileBuffer->decode16(0);
		header->FileSize = fileBuffer->decode32(2);
		header->ImageDataOffset = fileBuffer->decode32(10);

		return header;
	}
	BITMAPCOREHEADER* simpleBitmap::decodeCoreHeader(simpleBuffer* fileBuffer)
	{
		BITMAPCOREHEADER* header = new BITMAPCOREHEADER();

		// See bitmap.h or wikipedia documentation online. This could be part of the DIB header; but
		// it would be the older (oldest) format to carry information about the BMP file. The newer
		// ones were put right over the top of the older ones; and they have different field types (!)
		uint32_t offset = 14;

		header->bcSize = fileBuffer->decode32(offset);
		header->bcWidth = fileBuffer->decode16(offset + 4);
		header->bcHeight = fileBuffer->decode16(offset + 6);
		header->bcPlanes = fileBuffer->decode16(offset + 8);
		header->bcBitCount = fileBuffer->decode16(offset + 10);

		return header;
	}
	BITMAPV5HEADER* simpleBitmap::decodeHeader(simpleBuffer* fileBuffer)
	{
		BITMAPV5HEADER* header = new BITMAPV5HEADER();

		// BMP Signature Header (14 bytes total)
		//
		// BMP DIB Header Format (variable bytes)
		//
		// https://en.wikipedia.org/wiki/BMP_file_format
		//

		int dibHeaderOffset = 14;
		int cursor = dibHeaderOffset;

		// Header Size (4 bytes)
		header->bV5Size = fileBuffer->decode32(cursor);

		cursor += 4;

		// Bitmap Width in Pixels (4 bytes)
		header->bV5Width = fileBuffer->decode32(cursor);

		cursor += 4;

		// Bitmap Height in Pixels (4 bytes)
		header->bV5Height = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Planes (2 bytes)
		header->bV5Planes = fileBuffer->decode16(cursor);

		cursor += 2;

		// Bits Per Pixel (2 bytes)
		header->bV5BitCount = fileBuffer->decode16(cursor);

		cursor += 2;

		// Compression (4 bytes) (If this is set, then the bit masks are used for each color channel)
		header->bV5Compression = fileBuffer->decode32(cursor);

		cursor += 4;

		// Size of the raw bitmap data (4 bytes)
		header->bV5SizeImage = fileBuffer->decode32(cursor);

		cursor += 4;

		// Horizontal DPI (4 bytes)
		header->bV5XPelsPerMeter = fileBuffer->decode32(cursor);

		cursor += 4;

		// Vertical DPI (4 bytes)
		header->bV5YPelsPerMeter = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Indices (4 bytes) (see bitmap.h)
		header->bV5ClrUsed = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Indices Important! (4 bytes) (The number of color indices that are required for displaying the bitmap)
		header->bV5ClrImportant = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Mask Red (4 bytes)
		header->bV5RedMask = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Mask Green (4 bytes)
		header->bV5GreenMask = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Mask Blue (4 bytes)
		header->bV5BlueMask = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Mask Alpha (4 bytes)
		header->bV5AlphaMask = fileBuffer->decode32(cursor);

		cursor += 4;

		// Color Space Type (4 bytes) (see bitmap.h)
		header->bV5CSType = fileBuffer->decode32(cursor);

		cursor += 4;

		// ... Several more fields to go.

		return header;
	}
	BitmapOptionalMasks* simpleBitmap::decodeOptionalMasks(simpleBuffer* fileBuffer, int offset, bool includeAlpha)
	{
		// (see bitmap.h) This optional header entry should be the optional bitmaks. Reading the DIB header
		//				  shows what size this entry is:  12, or 16 bytes (with the alpha channel)
		//
		BitmapOptionalMasks* result = new BitmapOptionalMasks();

		result->red = fileBuffer->decode32(offset);
		result->green = fileBuffer->decode32(offset + 4);
		result->blue = fileBuffer->decode32(offset + 8);

		if (includeAlpha)
			result->alpha = fileBuffer->decode32(offset + 12);

		result->alphaUsed = includeAlpha;

		return result;
	}
	void simpleBitmap::decodeColorData(const simpleMapColorFilter& filter, simpleArray<simplePixel>& output)
	{
		if (output.count() != _dibHeader->bV5Width * _dibHeader->bV5Height)
			throw simpleException("Invalid color data array count:  simpleBitmap.h");

		// Color Table:  According to wikipedia's article the color table is an array of 32-bit words
		//				 that begin the image data from the bottom left hand corner to the top right.
		//
		// We're going to be looping over the image data bytes. The pixel data should be then copied
		// out of the 32-bit word using the approriate method - which is based on the file format.
		//

		int cursor = 0;

		int32_t redMask = 0x000000FF;
		int32_t greenMask = 0x0000FF00;
		int32_t blueMask = 0x00FF0000;
		int32_t alphaMask = 0xFF000000;

		while (cursor < output.count() * 4)	// word size
		{
			int32_t pixelData = _colorDataBuffer->decode32(cursor);

			simpleMaskedInt32 red(pixelData, redMask, 24);
			simpleMaskedInt32 green(pixelData, greenMask, 16);
			simpleMaskedInt32 blue(pixelData, blueMask, 8);
			simpleMaskedInt32 alpha(pixelData, alphaMask, 0);

			simplePixel pixel(red, green, blue, alpha, 255);

			// Mask depending on format: TODO! We're assuming (R, G, B, A) <-> (8, 8, 8, 8) (bits)
			output.set(cursor, filter.apply(pixel));

			cursor++;
		}
	}
}