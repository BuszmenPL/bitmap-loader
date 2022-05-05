#ifndef BMP_H
#define BMP_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <string>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
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
		BITMAPV3INFOHEADER, = INFO_HEADER_V3,
		BITMAPV4HEADER = INFO_HEADER_V4,
		BITMAPV5HEADER = INFO_HEADER_V5,

		OS21XBITMAPHEADER = BITMAPCOREHEADER,
		OS22XBITMAPHEADER = BITMAPCOREHEADER2
	};

	class BMP
	{
		public:
			BMP();
			BMP(std::string f);
			~BMP();

			BMP(const BMP&) =delete;
			BMP(BMP&&) =delete;

			BMP& operator=(const BMP&) =delete;
			BMP& operator=(BMP&&) =delete;

			void load(std::string f);

		private:
			DIBHeaderType type;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif