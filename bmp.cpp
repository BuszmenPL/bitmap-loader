#include "bmp.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <bitset>
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
			load_mask(file);
			load_color_table(file);
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
		info_header(os);
		info_dib(os);

		if(_file_info->dib_type != bmp::DIBHeaderType::CORE_HEADER_V1 && _info_header->bitCount <= 8)
			info_color_table(os);

		info_pixels_table(os);

		if(_file_info->dib_type == bmp::DIBHeaderType::INFO_HEADER_V5 && _info_header->profileSize)
			info_icc(os);

		info_file(os);
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	uint32_t BMP::width() const {
		const bmp::DIBHeaderType t = _file_info->dib_type;
		if(t == bmp::DIBHeaderType::CORE_HEADER_V1 || t == bmp::DIBHeaderType::CORE_HEADER_V2)
			return _info_header->cWidth;
		else
			return _info_header->width;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	uint32_t BMP::height() const {
		const bmp::DIBHeaderType t = _file_info->dib_type;
		if(t == bmp::DIBHeaderType::CORE_HEADER_V1 || t == bmp::DIBHeaderType::CORE_HEADER_V2)
			return _info_header->cHeight;
		else
			return _info_header->height;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	uint32_t BMP::pixel_size() const {
		uint32_t n {1};

		if(_info_header->bitCount > 8)
			n = _info_header->bitCount / 8;

		return n;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	uint32_t BMP::pixel_array_size() const {
		return pixel_size() * width() * height();
	}

	/* Load */
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
	void BMP::load_mask(std::istream& file) {
		if(_file_info->dib_type == bmp::DIBHeaderType::INFO_HEADER_V1 &&
			(_info_header->compression == bmp::BI_BITFIELDS || _info_header->compression == bmp::BI_ALPHABITFIELDS)) {

			const uint32_t n{_info_header->compression == bmp::BI_BITFIELDS?(sizeof(bmp::DWORD)*3):(sizeof(bmp::DWORD)*4)};
			type::Array array(n);
			file.read(array.get(), n);

			_file_info->red_mask = convertD();
			_file_info->green_mask = convertD();
			_file_info->blue_mask = convertD();
			_file_info->exist_mask = true;

			if(_info_header->compression == bmp::BI_ALPHABITFIELDS) {
				_file_info->alpha_mask = convertD();
				_file_info->exist_alpha = true;
			}
		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::load_color_table(std::istream& file) {
		if(_info_header->clrUsed || _info_header->bitCount <= 8) {

		}
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::load_pixsels(std::istream& file) {
		const uint32_t size = pixel_array_size();

		_pixel_array = move(bmp::PixelArray(new bmp::BYTE [size]));
		file.seekg(_file_header->offBits, istream::beg);

		for(uint32_t i{}; i<height(); ++i) {
			file.read(reinterpret_cast<char*>(_pixel_array.get() + (width() * i)), width());
			file.seekg(4, istream::cur);
		}
	}

	/* Info */
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info_header(std::ostream& os) {
		os << "Nagłówek Pliku" << endl;
		os << "  Typ/Sygnatura: " << to_string() << endl;
		os << "  Rozmiar: " << _file_header->size << endl;
		os << "  Zarezerwowany1: " << _file_header->reserved1 << endl;
		os << "  Zarezerwowany2: " << _file_header->reserved2 << endl;
		os << "  Offset tablicy pikseli: " << _file_header->offBits << endl << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info_dib(std::ostream& os) {
		const bmp::DIBHeaderType type = _file_info->dib_type;

		os << "Nagłówek DIB" << endl;
		os << "  Rozmiar: " << _info_header->size << endl;
		os << "  Szerokość: " << width() << endl;
		os << "  Wysokość: " << height() << endl;
		os << "  Ilość płaszczyzn: " << _info_header->planes << endl;
		os << "  Głębia: " << _info_header->bitCount << endl;

		if(type != bmp::DIBHeaderType::CORE_HEADER_V1) {
			os << "  Kompresja: " << compression_to_string() << endl;
			os << "  Rozmiar tablicy pikseli: " << _info_header->sizeImage << endl;
		}

		if(type == bmp::DIBHeaderType::CORE_HEADER_V2) {
			os << "  Pozioma rozdzielczość obrazu: " << _info_header->xRes << endl;
			os << "  Pionowa rozdzielczość obrazu: " << _info_header->yRes << endl;
		}
		else {
			os << "  Pozioma rozdzielczość obrazu: " << _info_header->xPelsPerMeter << endl;
			os << "  Pionowa rozdzielczość obrazu: " << _info_header->yPelsPerMeter << endl;
		}

		if(type != bmp::DIBHeaderType::CORE_HEADER_V1) {
			os << "  Liczba kolorów w palecie kolorów: " << _info_header->clrUsed << endl;
			os << "  Liczba ważnych kolorów: " << _info_header->clrImportant << endl;
		}

		if(type == bmp::DIBHeaderType::CORE_HEADER_V2) {
			os << "  Jednostka: piksele/metr" << endl; // Jedyna zdefiniowana wartość to 0 (_info_header->resUnit)
			os << "  Zarezerwowany: " << _info_header->cReserved << endl;
			os << "  Orientacja: LEFT BOTTOM" << endl; // Jedyna zdefiniowana wartość to 0 (_info_header->orientation)
			os << "  Półtonowanie: " << halftoning_to_string() << endl;
			os << "  Półtonowanie parametr1: " << _info_header->halftoneSize1 << endl;
			os << "  Półtonowanie parametr2: " << _info_header->halftoneSize2 << endl;
			os << "  Przestrzeń koloru: RGB" << endl; // Jedyna zdefiniowana wartość to 0 (_info_header->colorSpace)
			os << "  Identyfikator: " << _info_header->appData << endl;
		}
		else if(type != bmp::DIBHeaderType::CORE_HEADER_V1 && _info_header->size >= bmp::IHV2_SIZE) {
			os << "  Maska czerwona:  " << bitset<32>(_info_header->redMask) << endl;
			os << "  Maska zielona:   " << bitset<32>(_info_header->greenMask) << endl;
			os << "  Maska niebieska: " << bitset<32>(_info_header->blueMask) << endl;

			if(_info_header->size >= bmp::IHV3_SIZE)
				os << "  Maska alfa:      " << bitset<32>(_info_header->alphaMask) << endl;

			if(_info_header->size >= bmp::IHV4_SIZE) {
				os << "  Typ przestrzeni kolorów: " << _info_header->cSType << endl;
				os << "  Endpoint: " << endpoint_to_string() << endl;
				os << "  Gamma czerwona: " << _info_header->gammaRed << endl;
				os << "  Gamma zielona: " << _info_header->gammaGreen << endl;
				os << "  Gamma niebieska: " << _info_header->gammaBlue << endl;
			}

			if(_info_header->size >= bmp::IHV5_SIZE) {
				os << "  intent: " << _info_header->intent << endl;
				os << "  Offset ICC: " << _info_header->profileData << endl;
				os << "  Rozmair ICC: " << _info_header->profileSize << endl;
				os << "  Zarezerwowany: " << _info_header->reserved << endl;
			}
		}

		os << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info_color_table(std::ostream& os) {

	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info_pixels_table(std::ostream& os) {

	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info_icc(std::ostream& os) {

	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void BMP::info_file(std::ostream& os) {
		os << "Dodatkowe informacje o pliku" << endl;
		os << "  Typ nagłówka DIB: " << dib_to_string() << endl;
		os << "  Rozmiar przestrzeni wyrównania1: " << _file_info->gab1 << endl;
		os << "  Rozmiar przestrzeni wyrównania2: " << _file_info->gab2 << endl;
		os << "  Dodatkowa maska: ";

		if(_file_info->exist_mask) {
			os << "Rozmaiar: " << (3 + (_file_info->exist_alpha))*4 << endl;
			os << "  Maska czerwona:  " << bitset<32>(_file_info->red_mask) << endl;
			os << "  Maska zielona:   " << bitset<32>(_file_info->green_mask) << endl;
			os << "  Maska niebieska: " << bitset<32>(_file_info->blue_mask) << endl;
			os << "  Maska alfa:      ";

			if(_file_info->exist_alpha)
				os << bitset<32>(_file_info->alpha_mask) << endl;
			else
				os << "Nie zdefiniowano" << endl;
		}
		else
			os << "Nie występuje" << endl;
	}

	/* Load */
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

	/* Info */
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::to_string() {
		string tmp;
		tmp = static_cast<char>(_file_header->type);
		tmp += static_cast<char>(_file_header->type >> 8);
		return tmp;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::dib_to_string() {
		const bmp::DIBHeaderType type = _file_info->dib_type;
		string text;

		switch(type) {
			case bmp::DIBHeaderType::CORE_HEADER_V1: text = "CORE_HEADER_V1 BITMAPCOREHEADER OS21XBITMAPHEADER"; break;
			case bmp::DIBHeaderType::CORE_HEADER_V2: text = "CORE_HEADER_V2 BITMAPCOREHEADER2 OS22XBITMAPHEADER"; break;
			case bmp::DIBHeaderType::INFO_HEADER_V1: text = "INFO_HEADER_V1 BITMAPINFOHEADER"; break;
			case bmp::DIBHeaderType::INFO_HEADER_V2: text = "INFO_HEADER_V2 BITMAPV2INFOHEADER"; break;
			case bmp::DIBHeaderType::INFO_HEADER_V3: text = "INFO_HEADER_V3 BITMAPV3INFOHEADER"; break;
			case bmp::DIBHeaderType::INFO_HEADER_V4: text = "INFO_HEADER_V4 BITMAPV4HEADER"; break;
			case bmp::DIBHeaderType::INFO_HEADER_V5: text = "INFO_HEADER_V5 BITMAPV5HEADER"; break;
		}

		return text;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::endpoint_to_string() {
		string text;
		return text;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::compression_to_string() {
		string text;

		switch(_info_header->compression) {
			case bmp::CompressionMethod::BI_RGB: text = "[BI_RGB, Metoda: Brak]"; break;
			case bmp::CompressionMethod::BI_RLE8: text = "[BI_RLE8, Metoda: RLE 8-bit/pixel ]"; break;
			case bmp::CompressionMethod::BI_RLE4: text = "[BI_RLE4, Metoda: RLE 4-bit/pixel ]"; break;
			case bmp::CompressionMethod::BI_BITFIELDS: text = "[BI_BITFIELDS, Metoda: Huffman 1D]"; break;
			case bmp::CompressionMethod::BI_JPEG: text = "[BI_JPEG, Metoda: RLE-24]"; break;
			case bmp::CompressionMethod::BI_PNG: text = "[BI_PNG, Metoda: RLE-24]"; break;
			case bmp::CompressionMethod::BI_ALPHABITFIELDS: text = "[BI_ALPHABITFIELDS, Metoda: RGBA bit field masks]"; break;
			case bmp::CompressionMethod::BI_CMYK: text = "[BI_CMYK, Metoda: Brak]"; break;
			case bmp::CompressionMethod::BI_CMYKRLE8: text = "[BI_CMYKRLE8, Metoda: RLE-8]"; break;
			case bmp::CompressionMethod::BI_CMYKRLE4: text = "[BI_CMYKRLE4, Metoda: RLE-4]"; break;
		}

		return text;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::halftoning_to_string() {
		string text;

		switch(_info_header->halftoning) {
			case bmp::HalftoningAlgorithm::NONE: text= "Brak"; break;
			case bmp::HalftoningAlgorithm::ERROR_DIFFUSION: text= "Error diffusion"; break;
			case bmp::HalftoningAlgorithm::PANDA: text= "PANDA: Processing Algorithm for Noncoded Document Acquisition"; break;
			case bmp::HalftoningAlgorithm::SUPER_CIRCLE: text= "Super-circle"; break;
		}

		return text;
	}
	/*std::string BMP::mask_to_string(bmp::DWORD m) {
		string text{"0x"};

		for(uint32_t i{sizeof(bmp::DWORD)*2}; i!=0; --i) {
			char c = static_cast<char>((m >> (4*(i-1))) & 0xf) + '0';

			if(c > '9')
				c += 7; // różnica pomiędzy liczbami a literami

			text += c;
		}

		return text;
	}*/
}