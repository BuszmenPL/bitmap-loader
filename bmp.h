#ifndef BMP_H
#define BMP_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <string>
#include <ostream>
#include <istream>
#include "def.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	class BMP
	{
		public:
			BMP();
			~BMP() =default;

			BMP(const BMP&) =delete;
			BMP(BMP&&) =delete;

			BMP& operator=(const BMP&) =delete;
			BMP& operator=(BMP&&) =delete;

			void load(std::string f);
			void save(std::string f);
			void info(std::ostream& os);

			uint32_t width() const;
			uint32_t height() const;
			uint32_t pixel_size() const;
			uint32_t pixel_array_size() const;

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

			void load_header(std::istream&);
			void load_dib(std::istream&);
			void load_pixsels(std::istream&);

			void info_header(std::ostream&);
			void info_dib(std::ostream&);
			void info_color_table(std::ostream&);
			void info_pixels_table(std::ostream&);
			void info_icc(std::ostream&);
			void info_file(std::ostream&);

			void get_dib_type();
			void get_endpoints(char*);

			std::string to_string();
			std::string dib_to_string();
			std::string endpoint_to_string();
			std::string compression_to_string();
			std::string halftoning_to_string();
			//std::string mask_to_string(bmp::DWORD);
	};
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif