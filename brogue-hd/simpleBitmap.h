#pragma once

#include <filesystem>
#include <fstream>
#include "bitmap.h"
#include "simpleString.h"
#include "simpleArray.h"
#include "simpleBuffer.h"
#include "simplePixel.h"
#include "simplePixelFilter.h"

namespace brogueHd::simple
{
	class simpleBitmap : hashableObject
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
					 BITMAPV5HEADER* dibHeader,
					 BitmapOptionalMasks* optionalMasks);
		simpleBitmap(const simpleBitmap& copy);
		~simpleBitmap();

		static simpleBitmap* fromFile(const simpleString& filename);
		static simpleBitmap* fromFile(const char* filename);
		void toFile(const simpleString& filename);
		void toFile(const simpleString& filename, const simpleMapColorFilter& filter);

		int pixelWidth() const
		{
			return _dibHeader->bV5Width;
		}
		int pixelHeight() const
		{
			return _dibHeader->bV5Height;
		}

		simpleBuffer* getBuffer() const
		{
			return _fileBuffer;
		}

	protected:

		BitmapFileHeader* getSignature() const
		{
			return _signatureHeader;
		}
		BITMAPV5HEADER* getDIBHeader() const
		{
			return _dibHeader;
		}
		simpleBuffer* getColorDataBuffer() const
		{
			return _colorDataBuffer;
		}
		BitmapOptionalMasks* getOptionalMasks() const
		{
			return _optionalMasks;
		}


	protected:

		void decodeColorData(const simpleMapColorFilter& filter, simpleArray<simplePixel>& output);

	private:

		static BitmapFileHeader* decodeSignature(simpleBuffer* fileBuffer);
		static BITMAPV5HEADER* decodeHeader(simpleBuffer* fileBuffer);
		static BitmapOptionalMasks* decodeOptionalMasks(simpleBuffer* fileBuffer, int offset, bool includeAlpha);

	private:

		// NOTE: This is still a very rough first draft. There are more details for the headers and specs to be filled out correctly.

		// Bitmap Format: (see bitmap.h) [ Signature Header, DIB Header, Optional Bitmasks, Optional Color Table, Optional Gap1, Pixel Array, ...]

		BitmapFileHeader*	 _signatureHeader;
		BITMAPV5HEADER*		 _dibHeader;			
		BitmapOptionalMasks* _optionalMasks;

	private:

		simpleBuffer* _fileBuffer;
		simpleBuffer* _colorDataBuffer;

	};

	simpleBitmap::simpleBitmap()
	{
		_signatureHeader = nullptr;
		_dibHeader = nullptr;
		_fileBuffer = nullptr;
		_colorDataBuffer = nullptr;
		_optionalMasks = nullptr;
	}
	simpleBitmap::simpleBitmap(simpleBuffer* fileBuffer,
							   simpleBuffer* colorDataBuffer,
							   BitmapFileHeader* signature,
							   BITMAPV5HEADER* dibHeader,
							   BitmapOptionalMasks* optionalMasks)
	{
		_fileBuffer = fileBuffer;
		_colorDataBuffer = colorDataBuffer;
		_signatureHeader = signature;
		_dibHeader = dibHeader;
		_optionalMasks = optionalMasks;
	}
	simpleBitmap::simpleBitmap(const simpleBitmap& copy)
	{
		_signatureHeader = copy.getSignature();
		_dibHeader = copy.getDIBHeader();
		_colorDataBuffer = copy.getColorDataBuffer();
		_fileBuffer = copy.getBuffer();
		_optionalMasks = copy.getOptionalMasks();
	}
	simpleBitmap::~simpleBitmap()
	{
		delete _signatureHeader;
		delete _dibHeader;
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
		size_t colorDataBufferSize;
		char* fileBufferIn;
		char* colorDataBufferIn;

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
			BITMAPV5HEADER* dibHeader = decodeHeader(fileBuffer);

			//// Color Table:  32-bit words, one per pixel (see bitmap.h for more documentation)
			////
			//colorDataBufferSize = dibHeader->bV5Width * dibHeader->bV5Height * 4;
			//colorDataBufferIn = new char[colorDataBufferSize];	

			//uint32_t extraBitmaskHeader = 0;

			//// Check for compression flag:
			////
			//if (dibHeader->bV5Compression == BI_BITFIELDS)
			//	extraBitmaskHeader = 12;

			//// Try accounting for the file pieces
			//uint32_t extraSpace = fileSize - colorDataBufferSize - dibHeader->bV5Size - BitmapFileHeader::HEADER_SIZE - extraBitmaskHeader;
			//
			//if (extraSpace > 0)
			//{
			//	if (extraSpace == 4)
			//		simpleException::show("Extra bitmask located:  Check for BI_ALPHABITFIELDS.");

			//	else
			//		simpleException::show("Invalid bitmap file decoding:  Either corrupt file or improper use of DIB header");
			//}

			//BitmapOptionalMasks* optionalMasks = decodeOptionalMasks(fileBuffer, BitmapFileHeader::HEADER_SIZE + dibHeader->bV5Size, false);

			//// Direct Buffer Copy
			//std::copy_n(&fileBufferIn[BitmapFileHeader::HEADER_SIZE + dibHeader->bV5Size + extraBitmaskHeader], colorDataBufferSize, colorDataBufferIn);

			//simpleBuffer* colorDataBuffer = new simpleBuffer(colorDataBufferIn, colorDataBufferSize, true);

			return new simpleBitmap(fileBuffer, nullptr, signature, dibHeader, nullptr);
		}
		catch (std::exception& ex)
		{
			simpleException::show("Error reading bitmap file:  {}", ex.what());
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
			simpleArray<simplePixel> colorData(_colorDataBuffer->getBufferSize() / 4);
			
			// Decode the pixel data - applying the filter to the data
			decodeColorData(filter, colorData);

			// Create an output buffer
			char* buffer = new char[_colorDataBuffer->getBufferSize()];

			// Create a simpleBuffer to help with the output encoding
			simpleBuffer outputBuffer(buffer, _colorDataBuffer->getBufferSize(), true);

			// Encode all the pixel data to the stream - checking word alignment
			for (int index = 0; index < colorData.count(); index++)
			{
				outputBuffer.encode32(index * 4, colorData.get(index).getRGBA());
			}

			std::ofstream stream(filename.c_str());

			stream.write(buffer, _colorDataBuffer->getBufferSize());
			stream.flush();
			stream.close();

			delete [] buffer;
		}
		catch (std::exception& ex)
		{
			simpleException::show("Error reading png file:  {}", ex.what());
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
			simpleException::show("Unhandled bitmap signature type:  simpleBitmap.h");

		// Reserved data not needed:  Just grab these data points at their specified offsets
		//
		header->Signature = fileBuffer->decode16(0, simpleBuffer::byte1);
		header->FileSize = fileBuffer->decode32(2);
		header->ImageDataOffset = fileBuffer->decode32(10);

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
		header->bV5Planes = fileBuffer->decode16(cursor, simpleBuffer::bufferByte::byte1);

		cursor += 2;

		// Bits Per Pixel (2 bytes)
		header->bV5BitCount = fileBuffer->decode16(cursor, simpleBuffer::bufferByte::byte1);

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
			simpleException::show("Invalid color data array count:  simpleBitmap.h");

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