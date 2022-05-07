#include "bmp.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "type.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;

#define convertW() type::convert<bmp::WORD>(array);
#define convertD() type::convert<bmp::DWORD>(array);
#define convertL() type::convert<bmp::LONG>(array);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	BMP::BMP()
		:_file_header(new bmp::FileHeader), _info_header(new bmp::InfoHeader), _file_info(new bmp::FileInfo) {}
	void BMP::load(std::string f) {
		ifstream file;
		file.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			file.open(f, ios::binary);

			load_header(file);
			get_dib_type();
			load_dib(file);
			load_pixsels(file);
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
	void BMP::load_header(std::istream& file) {
		constexpr uint32_t DIB_SIZE {sizeof(bmp::DWORD)};
		constexpr uint32_t FILE_HEADER_SIZE {14};

		type::Array array(FILE_HEADER_SIZE + DIB_SIZE);
		// ładowanie nagłówka pliku + rozmiar nagłówka DIB
		file.read(array.get(), FILE_HEADER_SIZE + DIB_SIZE);

		_file_header->type = convertW();

		string tmp = to_string();
		if(!(tmp == "BM" || tmp == "BA" || tmp == "CI" || tmp == "CP" || tmp == "IC" || tmp == "PT"))
			throw invalid_argument("Open file: it is not a BMP file " + tmp);

		_file_header->size = convertD();
		_file_header->reserved1 = convertW();
		_file_header->reserved2 = convertW();
		_file_header->offBits = convertD();

		// rozmiar nagłówka DIB
		_info_header->size = convertD();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::load_dib(std::istream& file) {
		constexpr uint32_t DIB_SIZE {sizeof(bmp::DWORD)};

		type::Array array(_info_header->size - DIB_SIZE);
		// ładowanie nagłówka DIB (rozmiar załadowany wcześniej!)
		file.read(array.get(), _info_header->size - DIB_SIZE);

		if(_file_info->dib_type == bmp::DIBHeaderType::CORE_HEADER_V1) {
			_info_header->cWidth = convertW();
			_info_header->cHeight = convertW();
			_info_header->planes = convertW();
			_info_header->bitCount = convertW();
		}
		else if(_file_info->dib_type == bmp::DIBHeaderType::CORE_HEADER_V2) {
			_info_header->cWidth = convertD();
			_info_header->cHeight = convertD();
			_info_header->planes = convertW();
			_info_header->bitCount = convertW();
			_info_header->compression = convertD();
			_info_header->sizeImage = convertD();
			_info_header->xRes = convertD();
			_info_header->yRes = convertD();
			_info_header->clrUsed = convertD();
			_info_header->clrImportant = convertD();
			_info_header->resUnit = convertW();
			_info_header->cReserved = convertW();
			_info_header->orientation = convertW();
			_info_header->halftoning = convertW();
			_info_header->halftoneSize1 = convertD();
			_info_header->halftoneSize2 = convertD();
			_info_header->colorSpace = convertD();
			_info_header->appData = convertD();
		}
		else {
			_info_header->width = convertL();
			_info_header->height = convertL();
			_info_header->planes = convertW();
			_info_header->bitCount = convertW();
			_info_header->compression = convertD();
			_info_header->sizeImage = convertD();
			_info_header->xPelsPerMeter = convertL();
			_info_header->yPelsPerMeter = convertL();
			_info_header->clrUsed = convertD();
			_info_header->clrImportant = convertD();

			if(_info_header->size >= bmp::IHV2_SIZE) {
				_info_header->redMask = convertD();
				_info_header->greenMask = convertD();
				_info_header->blueMask = convertD();
			}
			if(_info_header->size >= bmp::IHV3_SIZE)
				_info_header->alphaMask = convertD();

			if(_info_header->size >= bmp::IHV4_SIZE) {
				_info_header->cSType = convertD();
				get_endpoints(array.get());
				array.add(bmp::CIEXYZTRIPLE_SIZE);
				_info_header->gammaRed = convertD();
				_info_header->gammaGreen = convertD();
				_info_header->gammaBlue = convertD();
			}

			if(_info_header->size == bmp::IHV5_SIZE) {
				_info_header->intent = convertD();
				_info_header->profileData = convertD();
				_info_header->profileSize = convertD();
				_info_header->reserved = convertD();
			}
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::load_pixsels(std::istream& file) {

	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::to_string() {
		string tmp;
		tmp = static_cast<char>(_file_header->type);
		tmp += static_cast<char>(_file_header->type >> 8);
		return tmp;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::get_dib_type() {
		switch(_info_header->size) {
			case bmp::CHV1_SIZE: _file_info->dib_type = bmp::DIBHeaderType::CORE_HEADER_V1; break;
			case bmp::CHV2_SIZE: _file_info->dib_type = bmp::DIBHeaderType::CORE_HEADER_V2; break;
			case bmp::IHV1_SIZE: _file_info->dib_type = bmp::DIBHeaderType::INFO_HEADER_V1; break;
			case bmp::IHV2_SIZE: _file_info->dib_type = bmp::DIBHeaderType::INFO_HEADER_V2; break;
			case bmp::IHV3_SIZE: _file_info->dib_type = bmp::DIBHeaderType::INFO_HEADER_V3; break;
			case bmp::IHV4_SIZE: _file_info->dib_type = bmp::DIBHeaderType::INFO_HEADER_V4; break;
			case bmp::IHV5_SIZE: _file_info->dib_type = bmp::DIBHeaderType::INFO_HEADER_V5; break;
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::get_endpoints(char* array) {
		auto load_vector = [&](char* tab) -> bmp::Vector {
			bmp::Vector v;
			v.z = (*reinterpret_cast<bmp::FixedValue*>(tab));
			v.y = (*reinterpret_cast<bmp::FixedValue*>(tab + bmp::FXPT2DOT30_SIZE));
			v.x = (*reinterpret_cast<bmp::FixedValue*>(tab + (2*bmp::FXPT2DOT30_SIZE)));
			return v;
		};

		_info_header->endpoints.blue = load_vector(array);
		_info_header->endpoints.green = load_vector(array + bmp::CIEXYZ_SIZE);
		_info_header->endpoints.red = load_vector(array + (2*bmp::CIEXYZ_SIZE));
	}
}