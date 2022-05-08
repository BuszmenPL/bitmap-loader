#include <iostream>
#include "bmp.h"

using namespace std;

int main(int argc, const char* argv[])
{
	if(argc != 2) {
		cerr << "Nieprawidłowa ilość argumentów!" << endl;
		return 0;
	}

	file::BMP bmp_file;
	bmp_file.load(argv[1]);
	bmp_file.info(cout);

	return 0;
}