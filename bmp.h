#ifndef BMP_H
#define BMP_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <string>
#include <ostream>
#include "def.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	class BMP
	{
		public:
			BMP() =default;
			~BMP() =default;

			BMP(const BMP&) =delete;
			BMP(BMP&&) =delete;

			BMP& operator=(const BMP&) =delete;
			BMP& operator=(BMP&&) =delete;

			void load(std::string f);
			void save(std::string f);
			void info(std::ostream& os);

			bmp::InfoHeader* info_header()
				{ return this->_info_header.get(); }

			bmp::DWORD* color_table()
				{ return this->_color_table.get(); }

			bmp::BYTE* pixel_array()
				{ return this->_pixel_array.get(); }

		private:
			using FileHeader = std::unique_ptr<bmp::FileHeader>;
			using InfoHeader = std::unique_ptr<bmp::InfoHeader>;
			using FileInfo = std::unique_ptr<bmp::FileInfo>;

			FileHeader _file_header;
			InfoHeader _info_header;
			bmp::ColorTable _color_table;
			bmp::PixelArray _pixel_array;
			bmp::ICCColorProfile _color_profile;
			FileInfo _file_info;

			std::string to_string();
	};
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif