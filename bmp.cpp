#include "bmp.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
using namespace std;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace file
{
	void BMP::load(std::string f) {
		ifstream file;
		file.exceptions(ifstream::failbit | ifstream::badbit);

		try {
			file.open(f, ios::binary);

			/* File Header */ 
			file >> _file_header->type;
			string type = to_string();

			if(!(type == "BM" || type == "BA" || type == "CI" || type == "CP" || type == "IC" || type == "PT"))
				throw invalid_argument("Open file: it is not a BMP file");

			file >> _file_header->size;
			file >> _file_header->reserved1;
			file >> _file_header->reserved2;
			file >> _file_header->offBits;
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
		os << "\tTyp/Sygnatura: " << to_string() << endl;
		os << "\tRozmiar: " << _file_header->size << endl;
		os << "\tZarezerwowany1: " << _file_header->reserved1 << endl;
		os << "\tZarezerwowany2: " << _file_header->reserved2 << endl;
		os << "\tOffset tablicy pikseli: " << _file_header->offBits << endl << endl;
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::string BMP::to_string() {
		return string{static_cast<char>(_file_header->type >> 8)} + static_cast<char>(_file_header->type);
	}
}