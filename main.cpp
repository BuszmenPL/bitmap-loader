#include <iostream>
#include "bmp.h"

using namespace std;

int main(int argc, const char* argv[])
{
	if(argc != 2) {
		cerr << "Nieprawidłowa ilość argumentów!" << endl;
		return 0;
	}

	// załadowanie pliku i wyświetlenie informacji
	file::BMP bmp_file;
	bmp_file.load(argv[1]);
	bmp_file.info(cout);

	const uint32_t n = bmp_file.pixel_array_size();
	auto array = bmp_file.pixel_array();

	// negacja wartości
	for(uint32_t i{}; i<n; ++i)
		array[i] = (~(array[i]));

	// zapisanie pliku
	bmp_file.save(string{"negatyw_"} + argv[1]);

	return 0;
}