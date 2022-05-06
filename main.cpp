#include <iostream>
#include "bmp.h"

using namespace std;

int main()
{
	file::BMP bmp_file;
	bmp_file.load("test.bmp");
	bmp_file.info(cout);

	return 0;
}