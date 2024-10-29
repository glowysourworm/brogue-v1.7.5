#pragma once

#include <cstdint>

namespace brogueHd
{
	// Implementation of Bitmap File Format:
	//
	// https://en.wikipedia.org/wiki/BMP_file_format
	//
	// MSFT's BITMAPV5HEADER:  https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapv5header
	//						   Slightly modified here to remove their macros:  DWORD, LONG, CIEXYZTRIPLE, and aliases.
	//

	// DWORD = unsigned long
	// LONG  = long
	// WORD  = unsigned short
	// CIEXYZTRIPLE = RBG TRIPLE (called CIEXYZ) (I renamed this simplePixel, and added the alpha channel)
	// FXPT2DOT30   = long
	//

	/*
	
		Bitmap Pixel Formats:  bV5BitCount

		Value 	Meaning
		0 	The number of bits per pixel is specified or is implied by the JPEG or PNG file format.

		1 	The bitmap is monochrome, and the bmiColors member of BITMAPINFO contains two entries. 
			Each bit in the bitmap array represents a pixel. If the bit is clear, the pixel is displayed 
			with the color of the first entry in the bmiColors color table. If the bit is set, the pixel 
			has the color of the second entry in the table.

		4 	The bitmap has a maximum of 16 colors, and the bmiColors member of BITMAPINFO contains up to 
			16 entries. Each pixel in the bitmap is represented by a 4-bit index into the color table. 
			For example, if the first byte in the bitmap is 0x1F, the byte represents two pixels. The first 
			pixel contains the color in the second table entry, and the second pixel contains the color in 
			the sixteenth table entry.

		8 	The bitmap has a maximum of 256 colors, and the bmiColors member of BITMAPINFO contains up to 
			256 entries. In this case, each byte in the array represents a single pixel.

		16 	The bitmap has a maximum of 2^16 colors. If the bV5Compression member of the BITMAPV5HEADER structure 
			is BI_RGB, the bmiColors member of BITMAPINFO is NULL. Each WORD in the bitmap array represents a 
			single pixel. The relative intensities of red, green, and blue are represented with five bits for 
			each color component. The value for blue is in the least significant five bits, followed by five 
			bits each for green and red. The most significant bit is not used. The bmiColors color table is 
			used for optimizing colors used on palette-based devices, and must contain the number of entries specified 
			by the bV5ClrUsed member of the BITMAPV5HEADER.If the bV5Compression member of the BITMAPV5HEADER 
			is BI_BITFIELDS, the bmiColors member contains three DWORD color masks that specify the red, green, 
			and blue components, respectively, of each pixel. Each WORD in the bitmap array represents a single pixel.

		When the bV5Compression member is BI_BITFIELDS, bits set in each DWORD mask must be contiguous and should not 
		overlap the bits of another mask. All the bits in the pixel do not need to be used.

		24 	The bitmap has a maximum of 2^24 colors, and the bmiColors member of BITMAPINFO is NULL. Each 3-byte triplet 
			in the bitmap array represents the relative intensities of blue, green, and red, respectively, for a pixel. 
			The bmiColors color table is used for optimizing colors used on palette-based devices, and must contain the 
			number of entries specified by the bV5ClrUsed member of the BITMAPV5HEADER structure.

		32 	The bitmap has a maximum of 2^32 colors. If the bV5Compression member of the BITMAPV5HEADER is BI_RGB, the 
			bmiColors member of BITMAPINFO is NULL. Each DWORD in the bitmap array represents the relative intensities 
			of blue, green, and red for a pixel. The value for blue is in the least significant 8 bits, followed by 8 
			bits each for green and red. The high byte in each DWORD is not used. The bmiColors color table is used for 
			optimizing colors used on palette-based devices, and must contain the number of entries specified by the 
			bV5ClrUsed member of the BITMAPV5HEADER.If the bV5Compression member of the BITMAPV5HEADER is BI_BITFIELDS, 
			the bmiColors member contains three DWORD color masks that specify the red, green, and blue components of each 
			pixel. Each DWORD in the bitmap array represents a single pixel.
	
	*/

	/*
	
		Compression:  bV5Compression

		Value 			Meaning

		BI_RGB 			An uncompressed format.

		BI_RLE8 		A run-length encoded (RLE) format for bitmaps with 8 bpp. The compression format is a two-byte 
						format consisting of a count byte followed by a byte containing a color index. If bV5Compression 
						is BI_RGB and the bV5BitCount member is 16, 24, or 32, the bitmap array specifies the actual 
						intensities of blue, green, and red rather than using color table indexes. For more information, 
						see Bitmap Compression.

		BI_RLE4 		An RLE format for bitmaps with 4 bpp. The compression format is a two-byte format consisting of 
						a count byte followed by two word-length color indexes. For more information, see Bitmap Compression.

		BI_BITFIELDS 	Specifies that the bitmap is not compressed and that the color masks for the red, green, and blue 
						components of each pixel are specified in the bV5RedMask, bV5GreenMask, and bV5BlueMask members. 
						This is valid when used with 16- and 32-bpp bitmaps.

		BI_JPEG 		Specifies that the image is compressed using the JPEG file Interchange Format. JPEG compression 
						trades off compression against loss; it can achieve a compression ratio of 20:1 with little noticeable loss.

		BI_PNG 			Specifies that the image is compressed using the PNG file Interchange Format.
	
	*/


	/// <summary>
	/// https://en.wikipedia.org/wiki/BMP_file_format
	/// </summary>
	struct BitmapFileHeader
	{
		/// <summary>
		/// See explanations of these 2-character signatures on wikipedia. The BM signature
		/// is the default standard signature. 
		/// </summary>
		enum BMPSignature
		{
			BM,
			BA,
			CI,
			CP,
			IC,
			PT
		};

		// 2 Bytes (ASCII Signature)
		const char* WINDOWS_SIGNATURE = "BM";
		const char* OS2_STRUCT_BMP_ARRAY = "BA";
		const char* OS2_STRUCT_COLOR_ICON = "CI";
		const char* OS2_CONST_COLOR_POINTER = "CP";
		const char* OS2_STRUCT_ICON = "IC";
		const char* OS2_POINTER = "PT";

		// This is the decoded value for the below signature data.
		BMPSignature SignatureDecoded;

		// The actual ASCII Signature
		int16_t Signature;

		// 4 bytes (Size of the BMP file in bytes)
		int32_t FileSize;

		// 2 bytes (Reserved - set to 0)
		int16_t Reserved1;

		// 2 bytes (Reserved - set to 0)
		int16_t Reserved2;

		// 4 bytes (Pointer to image data in the file)
		int32_t ImageDataOffset;
	};

	enum BitmapPixelFormat : int
	{
		PerPixel_1Bit = 0,
		PerPixel_2Bits = 1,
		PerPixel_4Bits = 2,
		PerPixel_8Bits = 3,
		PerPixel_16Bits = 4,
		PerPixel_32Bits = 5
	};

	/// <summary>
	/// MSFT's Header, modified for our data types
	/// </summary>
	struct BITMAPV5HEADER {

		/// <summary>
		/// Size of this header in bytes
		/// </summary>
		unsigned long        bV5Size;					

		/// <summary>
		/// The width of the bitmap, in pixels. If bV5Compression is BI_JPEG or BI_PNG, 
		/// the bV5Width member specifies the width of the decompressed JPEG or PNG image in pixels.
		/// </summary>
		long				 bV5Width;					

		/// <summary>
		/// The height of the bitmap, in pixels. If the value of bV5Height is positive, the bitmap 
		/// is a bottom-up DIB and its origin is the lower-left corner. If bV5Height value is negative, 
		/// the bitmap is a top-down DIB and its origin is the upper-left corner. If bV5Height is negative, 
		/// indicating a top - down DIB, bV5Compression must be either BI_RGB or BI_BITFIELDS.Top - down 
		/// DIBs cannot be compressed. If bV5Compression is BI_JPEG or BI_PNG, the bV5Height member specifies 
		/// the height of the decompressed JPEG or PNG image in pixels.
		/// </summary>
		long				 bV5Height;					

		/// <summary>
		/// The number of planes for the target device. This value must be set to 1.
		/// </summary>
		unsigned short       bV5Planes;

		/// <summary>
		/// The number of bits that define each pixel and the maximum number of colors in the bitmap.
		/// This member can be one of the following values. (See above bitmap.h for explanation of formats)
		/// </summary>
		unsigned short       bV5BitCount;

		/// <summary>
		/// Specifies that the bitmap is not compressed. The bV5RedMask, bV5GreenMask, and bV5BlueMask 
		/// members specify the red, green, and blue components of each pixel. This is valid when used 
		/// with 16- and 32-bpp bitmaps. This member can be one of the following values.
		/// </summary>
		unsigned long        bV5Compression;

		/// <summary>
		/// The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps. If bV5Compression 
		/// is BI_JPEG or BI_PNG, bV5SizeImage is the size of the JPEG or PNG image buffer.
		/// </summary>
		unsigned long        bV5SizeImage;

		/// <summary>
		/// The horizontal resolution, in pixels-per-meter, of the target device for the bitmap. An application 
		/// can use this value to select a bitmap from a resource group that best matches the characteristics of 
		/// the current device.
		/// </summary>
		long				 bV5XPelsPerMeter;

		/// <summary>
		/// The vertical resolution, in pixels-per-meter, of the target device for the bitmap.
		/// </summary>
		long				 bV5YPelsPerMeter;

		/// <summary>
		/// The number of color indexes in the color table that are actually used by the bitmap. If this value is 
		/// zero, the bitmap uses the maximum number of colors corresponding to the value of the bV5BitCount member 
		/// for the compression mode specified by bV5Compression.If bV5ClrUsed is nonzero and bV5BitCount is less 
		/// than 16, the bV5ClrUsed member specifies the actual number of colors the graphics engine or device 
		/// driver accesses.If bV5BitCount is 16 or greater, the bV5ClrUsed member specifies the size of the color 
		/// table used to optimize performance of the system color palettes.If bV5BitCount equals 16 or 32, the optimal 
		/// color palette starts immediately following the BITMAPV5HEADER.If bV5ClrUsed is nonzero, the color table 
		/// is used on palettized devices, and bV5ClrUsed specifies the number of entries.
		/// </summary>
		unsigned long        bV5ClrUsed;

		/// <summary>
		/// The number of color indexes that are required for displaying the bitmap. If this value is zero, all colors are required.
		/// </summary>
		unsigned long        bV5ClrImportant;

		/// <summary>
		/// Color mask that specifies the red component of each pixel, valid only if bV5Compression is set to BI_BITFIELDS.
		/// </summary>
		unsigned long        bV5RedMask;

		/// <summary>
		/// Color mask that specifies the green component of each pixel, valid only if bV5Compression is set to BI_BITFIELDS.
		/// </summary>
		unsigned long        bV5GreenMask;

		/// <summary>
		/// Color mask that specifies the blue component of each pixel, valid only if bV5Compression is set to BI_BITFIELDS.
		/// </summary>
		unsigned long        bV5BlueMask;

		/// <summary>
		/// Color mask that specifies the alpha component of each pixel.
		/// </summary>
		unsigned long        bV5AlphaMask;

		/// <summary>
		/// The color space of the DIB. The following table specifies the values for bV5CSType. (See online for MSFT's table)
		/// </summary>
		unsigned long        bV5CSType;
		// CIEXYZTRIPLE bV5Endpoints;			// See MSFT's explanation. This involves the color space for the bitmap. 
		unsigned long        bV5GammaRed;
		unsigned long        bV5GammaGreen;
		unsigned long        bV5GammaBlue;
		unsigned long        bV5Intent;
		unsigned long        bV5ProfileData;
		unsigned long        bV5ProfileSize;
		unsigned long        bV5Reserved;
	};
}