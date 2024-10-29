#pragma once

#include <filesystem>
#include <fstream>
#include "bitmap.h"
#include "simpleString.h"
#include "simpleArray.h"

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
		simpleBitmap(const char* buffer, size_t bufferSize);
		simpleBitmap(BitmapFileHeader* signature, BITMAPV5HEADER* dibHeader, simpleArray<int32_t>* colorData);
		simpleBitmap(const simpleBitmap& copy);
		~simpleBitmap();

		static simpleBitmap* fromFile(const simpleString& filename);
		static simpleBitmap* fromFile(const char* filename);
		void toFile(const simpleString& filename);

		const char* getRawBuffer() const
		{
			return _rawBuffer;
		}
		const size_t getRawBufferSize() const
		{
			return _rawBufferSize;
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
		simpleArray<int32_t>* getColorData() const
		{
			return _colorData;
		}
		int32_t getStride() const
		{
			return _stride;
		}

	private:

		static BitmapFileHeader* decodeSignature(const char* buffer);
		static BITMAPV5HEADER* decodeHeader(const char* buffer);
		static simpleArray<int32_t>* decodeColorData(const char* buffer);

	private:

		static int8_t decodeAtOffset8(const char* buffer, int offset);
		static int16_t decodeAtOffset16(const char* buffer, int offset);
		static int32_t decodeAtOffset32(const char* buffer, int offset);

	private:
		// Bitmap Format:					// Signature Header, DIB Header, (Optional Pieces)..., Image Data, (Optional Pieces)...
		BitmapFileHeader* _signatureHeader;
		BITMAPV5HEADER* _dibHeader;			

		simpleArray<int32_t>* _colorData;

		// Width of the bitmap color 2D array. This is padded to 4-BYTE pixel data buckets. So, the
		// total pixel width of the bitmap is essentially the stride. The true color data is stored 
		// somewhere in there; and must be decoded.
		//
		int32_t _stride;

	private:

		const char* _rawBuffer;
		size_t _rawBufferSize;
	};

	simpleBitmap::simpleBitmap()
	{
		_signatureHeader = nullptr;
		_dibHeader = nullptr;
		_colorData = nullptr;
		_stride = 0;
	}
	simpleBitmap::simpleBitmap(const char* buffer, size_t bufferSize)
	{
		_rawBuffer = buffer;
		_rawBufferSize = bufferSize;
	}
	simpleBitmap::simpleBitmap(BitmapFileHeader* signature, BITMAPV5HEADER* dibHeader, simpleArray<int32_t>* colorData)
	{
		_signatureHeader = signature;
		_dibHeader = dibHeader;
		_colorData = colorData;
	}
	simpleBitmap::simpleBitmap(const simpleBitmap& copy)
	{
		_signatureHeader = copy.getSignature();
		_dibHeader = copy.getDIBHeader();
		_colorData = copy.getColorData();
		_stride = copy.getStride();
	}
	simpleBitmap::~simpleBitmap()
	{
		delete _signatureHeader;
		delete _dibHeader;
		delete _colorData;

		delete _rawBuffer;
	}

	simpleBitmap* simpleBitmap::fromFile(const char* filename)
	{
		return simpleBitmap::fromFile(simpleString(filename));
	}

	simpleBitmap* simpleBitmap::fromFile(const simpleString& filename)
	{
		uintmax_t fileSize = 0;
		size_t decompressedSize = 0;
		char* bufferIn;
		char* bufferOut;

		try
		{
			fileSize = std::filesystem::file_size(filename.c_str());

			if (fileSize <= 0)
				return false;

			bufferIn = new char[fileSize];

			std::ifstream stream(filename.c_str());
			
			stream.get(bufferIn, fileSize);
			stream.close();

			return new simpleBitmap(bufferIn, fileSize);
		}
		catch (std::exception& ex)
		{
			simpleException::show("Error reading png file:  {}", ex.what());
		}
	}

	void simpleBitmap::toFile(const simpleString& filename)
	{


	}

	BitmapFileHeader* simpleBitmap::decodeSignature(const char* buffer)
	{
		BitmapFileHeader* header = new BitmapFileHeader();

		if (buffer[0] == 'B' && buffer[1] == 'M')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::BM;

		else if (buffer[0] == 'B' && buffer[1] == 'A')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::BA;

		else if (buffer[0] == 'C' && buffer[1] == 'I')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::CI;

		else if (buffer[0] == 'C' && buffer[1] == 'P')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::CP;

		else if (buffer[0] == 'I' && buffer[1] == 'C')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::IC;

		else if (buffer[0] == 'P' && buffer[1] == 'T')
			header->SignatureDecoded = BitmapFileHeader::BMPSignature::PT;

		else 
			simpleException::show("Unhandled bitmap signature type:  simpleBitmap.h");

		// Reserved data not needed:  Just grab these data points at their specified offsets
		//
		header->Signature = decodeAtOffset16(buffer, 0);
		header->FileSize = decodeAtOffset32(buffer, 2);
		header->ImageDataOffset = decodeAtOffset32(buffer, 10);
	}
	BITMAPV5HEADER* simpleBitmap::decodeHeader(const char* buffer)
	{
		BITMAPV5HEADER* header = new BITMAPV5HEADER();

		return nullptr;
	}
	simpleArray<int32_t>* simpleBitmap::decodeColorData(const char* buffer, int offset)
	{
		return new simpleArray<int32_t>();
	}

	int8_t simpleBitmap::decodeAtOffset8(const char* buffer, int offset)
	{
		int32_t byte1 = 0x000F;

		return byte1 & buffer[offset];
	}
	int16_t simpleBitmap::decodeAtOffset16(const char* buffer, int offset)
	{
		int32_t byte1 = 0x000F;
		int32_t byte2 = 0x00F0;

		int32_t result = buffer[offset] & byte1;

		result |= (buffer[offset + 1] << 8) & byte2;
	}
	int32_t simpleBitmap::decodeAtOffset32(const char* buffer, int offset)
	{
		int32_t byte1 = 0x000F;
		int32_t byte2 = 0x00F0;
		int32_t byte3 = 0x0F00;
		int32_t byte4 = 0xF000;

		int32_t result = buffer[offset] & byte1;

		result |= (buffer[offset + 1] << 8) & byte2;
		result |= (buffer[offset + 2] << 16) & byte3;
		result |= (buffer[offset + 3] << 24) & byte4;
	}
}