#ifndef DEF_H
#define DEF_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <cstdint>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	namespace bmp
	{
		using BYTE = uint8_t;
		using WORD = uint16_t;
		using DWORD = uint32_t;
		using LONG = int32_t;

		struct FixedValue
		{
			DWORD fractional :30;
			DWORD integer :2;
		};

		using FXPT2DOT30 = FixedValue;

		struct Vector
		{
			FXPT2DOT30 x;
			FXPT2DOT30 y;
			FXPT2DOT30 z;
		};

		using CIEXYZ = Vector;

		struct ColorSpace
		{
			CIEXYZ red;
			CIEXYZ green;
			CIEXYZ blue;
		};

		using CIEXYZTRIPLE = ColorSpace;

		struct BMPFileHeader
		{
			WORD type;
			DWORD size;
			WORD reserved1;
			WORD reserved2;
			DWORD offBits;
		};

		struct DIBCoreHeaderV1
		{
			DWORD size;
			WORD width;
			WORD height;
			WORD planes;
			WORD bitCount;
		};

		struct DIBCoreHeaderV2
		{
			DWORD size;
			WORD width;
			WORD height;
			WORD planes;
			WORD bitCount;
			DWORD compression;
			DWORD sizeImage;
			DWORD xRes;
			DWORD yRes;
			DWORD clrUsed;
			DWORD clrImportant;
			WORD resUnit;
			WORD reserved;
			WORD orientation;
			WORD halftoning;
			DWORD halftoneSize1;
			DWORD halftoneSize2;
			DWORD colorSpace;
			DWORD appData;
		};

		struct DIBHeaderV1
		{
			DWORD size;
			LONG width;
			LONG height;
			WORD planes;
			WORD bitCount;
			DWORD compression;
			DWORD sizeImage;
			LONG xPelsPerMeter;
			LONG yPelsPerMeter;
			DWORD clrUsed;
			DWORD clrImportant;
		};

		struct DIBHeaderV2
		{
			DWORD size;
			LONG width;
			LONG height;
			WORD planes;
			WORD bitCount;
			DWORD compression;
			DWORD sizeImage;
			LONG xPelsPerMeter;
			LONG yPelsPerMeter;
			DWORD clrUsed;
			DWORD clrImportant;
			DWORD redMask;
			DWORD greenMask;
			DWORD blueMask;
		};

		struct DIBHeaderV3
		{
			DWORD size;
			LONG width;
			LONG height;
			WORD planes;
			WORD bitCount;
			DWORD compression;
			DWORD sizeImage;
			LONG xPelsPerMeter;
			LONG yPelsPerMeter;
			DWORD clrUsed;
			DWORD clrImportant;
			DWORD redMask;
			DWORD greenMask;
			DWORD blueMask;
			DWORD alphaMask;
		};

		struct DIBHeaderV4
		{
			DWORD size;
			LONG width;
			LONG height;
			WORD planes;
			WORD bitCount;
			DWORD compression;
			DWORD sizeImage;
			LONG xPelsPerMeter;
			LONG yPelsPerMeter;
			DWORD clrUsed;
			DWORD clrImportant;
			DWORD redMask;
			DWORD greenMask;
			DWORD blueMask;
			DWORD alphaMask;
			DWORD cSType;
			CIEXYZTRIPLE endpoints;
			DWORD gammaRed;
			DWORD gammaGreen;
			DWORD gammaBlue;
		};

		struct DIBHeaderV5
		{
			DWORD size;
			LONG width;
			LONG height;
			WORD planes;
			WORD bitCount;
			DWORD compression;
			DWORD sizeImage;
			LONG xPelsPerMeter;
			LONG yPelsPerMeter;
			DWORD clrUsed;
			DWORD clrImportant;
			DWORD redMask;
			DWORD greenMask;
			DWORD blueMask;
			DWORD alphaMask;
			DWORD cSType;
			CIEXYZTRIPLE endpoints;
			DWORD gammaRed;
			DWORD gammaGreen;
			DWORD gammaBlue;
			DWORD intent;
			DWORD profileData;
			DWORD profileSize;
			DWORD reserved;
		};

		using BITMAPFILEHEADER = BMPFileHeader;

		using BITMAPCOREHEADER = DIBCoreHeaderV1;
		using BITMAPCOREHEADER2 = DIBCoreHeaderV2;
		using BITMAPINFOHEADER = DIBHeaderV1;
		using BITMAPV2INFOHEADER = DIBHeaderV2;
		using BITMAPV3INFOHEADER = DIBHeaderV3;
		using BITMAPV4HEADER = DIBHeaderV4;
		using BITMAPV5HEADER = DIBHeaderV5;

		using OS21XBITMAPHEADER = BITMAPCOREHEADER;
		using OS22XBITMAPHEADER = BITMAPCOREHEADER2;

		struct BMPHeader :public DIBHeaderV5
		{
			DWORD xRes;
			DWORD yRes;
			WORD resUnit;
			WORD osReserved;
			WORD orientation;
			WORD halftoning;
			DWORD halftoneSize1;
			DWORD halftoneSize2;
			DWORD colorSpace;
			DWORD appData;
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif