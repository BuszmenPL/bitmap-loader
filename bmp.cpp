#include "bmp.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "type.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	BMP::BMP()
		:_file_header(new bmp::FileHeader), _info_header(new bmp::InfoHeader), _file_info(new bmp::FileInfo) {}
	void BMP::load(std::string f) {
		constexpr uint32_t FILE_HEADER_SIZE {14};

		ifstream file;
		file.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			file.open(f, ios::binary);

			type::Array array;
			array.alloc(FILE_HEADER_SIZE);

			/* File Header */
			file.read(array.get(), FILE_HEADER_SIZE);
			_file_header->type = type::convert<bmp::WORD>(array);
			string tmp = to_string();

			if(!(tmp == "BM" || tmp == "BA" || tmp == "CI" || tmp == "CP" || tmp == "IC" || tmp == "PT"))
				throw invalid_argument("Open file: it is not a BMP file " + tmp);

			_file_header->size = type::convert<bmp::DWORD>(array);
			_file_header->reserved1 = type::convert<bmp::WORD>(array);
			_file_header->reserved2 = type::convert<bmp::WORD>(array);
			_file_header->offBits = type::convert<bmp::DWORD>(array);
		}
		catch(ifstream::failure& error) {
			throw runtime_error(error.what());
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::save(std::string f) {

	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info(std::ostream& os) {
		os << "Nagłówek Pliku" << endl;
		os << "  Typ/Sygnatura: " << to_string() << endl;
		os << "  Rozmiar: " << _file_header->size << endl;
		os << "  Zarezerwowany1: " << _file_header->reserved1 << endl;
		os << "  Zarezerwowany2: " << _file_header->reserved2 << endl;
		os << "  Offset tablicy pikseli: " << _file_header->offBits << endl << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::to_string() {
		string tmp;
		tmp = static_cast<char>(_file_header->type);
		tmp += static_cast<char>(_file_header->type >> 8);
		return tmp;
	}
}