#ifndef DEF_H
#define DEF_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <cstdint>
#include <memory>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	namespace bmp
	{
		using BYTE = uint8_t;
		using WORD = uint16_t;
		using DWORD = uint32_t;
		using LONG = int32_t;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct FixedValue
		{
			DWORD fractional :30;
			DWORD integer :2;
		};

		using FXPT2DOT30 = FixedValue;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct Vector
		{
			FXPT2DOT30 x;
			FXPT2DOT30 y;
			FXPT2DOT30 z;
		};

		using CIEXYZ = Vector;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct ColorSpace
		{
			CIEXYZ red;
			CIEXYZ green;
			CIEXYZ blue;
		};

		using CIEXYZTRIPLE = ColorSpace;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct FileHeader
		{
			WORD type;
			DWORD size;
			WORD reserved1;
			WORD reserved2;
			DWORD offBits;
		};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct DIBCoreHeaderV1
		{
			DWORD size;
			WORD width;
			WORD height;
			WORD planes;
			WORD bitCount;
		};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

		enum CompressionMethod
		{
			BI_RGB =0,
			BI_RLE8,
			BI_RLE4,
			BI_BITFIELDS,
			BI_JPEG,
			BI_PNG,
			BI_ALPHABITFIELDS,
			BI_CMYK,
			BI_CMYKRLE8,
			BI_CMYKRLE4
		};

		enum HalftoningAlgorithm
		{
			NONE =0,
			ERROR_DIFFUSION,
			PANDA,
			SUPER_CIRCLE
		};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		using BITMAPFILEHEADER = FileHeader;

		using BITMAPCOREHEADER = DIBCoreHeaderV1;
		using BITMAPCOREHEADER2 = DIBCoreHeaderV2;
		using BITMAPINFOHEADER = DIBHeaderV1;
		using BITMAPV2INFOHEADER = DIBHeaderV2;
		using BITMAPV3INFOHEADER = DIBHeaderV3;
		using BITMAPV4HEADER = DIBHeaderV4;
		using BITMAPV5HEADER = DIBHeaderV5;

		using OS21XBITMAPHEADER = BITMAPCOREHEADER;
		using OS22XBITMAPHEADER = BITMAPCOREHEADER2;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct InfoHeader :public DIBHeaderV5
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
		};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		enum class DIBHeaderType
		{
			CORE_HEADER_V1,
			CORE_HEADER_V2,
			INFO_HEADER_V1,
			INFO_HEADER_V2,
			INFO_HEADER_V3,
			INFO_HEADER_V4,
			INFO_HEADER_V5,

			BITMAPCOREHEADER = CORE_HEADER_V1,
			BITMAPCOREHEADER2 = CORE_HEADER_V2,
			BITMAPINFOHEADER = INFO_HEADER_V1,
			BITMAPV2INFOHEADER = INFO_HEADER_V2,
			BITMAPV3INFOHEADER = INFO_HEADER_V3,
			BITMAPV4HEADER = INFO_HEADER_V4,
			BITMAPV5HEADER = INFO_HEADER_V5,

			OS21XBITMAPHEADER = BITMAPCOREHEADER,
			OS22XBITMAPHEADER = BITMAPCOREHEADER2
		};
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		using ColorTable = std::unique_ptr<DWORD []>;
		using PixelArray = std::unique_ptr<BYTE []>;
		using ICCColorProfile = std::unique_ptr<BYTE []>;
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		struct FileInfo
		{
			DIBHeaderType dib_type;
			uint32_t gab1;
			uint32_t gab2;
			uint8_t red_mask;
			uint8_t green_mask;
			uint8_t blue_mask;
			uint8_t alpha_mask;
		};
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif