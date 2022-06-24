#ifndef TYPE_H
#define TYPE_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <cstdint>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace type
{
	// Tablica przechowująca ciąg bajtów z pliku
	class Array
	{
		public:
			Array(uint32_t size);
			~Array();

			Array(const Array&) =delete;
			Array(Array&&) =delete;

			Array& operator=(const Array&) =delete;
			Array& operator=(Array&&) =delete;

			void add(uint32_t size);
			char* get();

		private:
			char* array;
			uint32_t sz;
	};

	// szablon do konwersji danych
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	template <typename T>
	T convert(Array& array) {
		T tmp = *(reinterpret_cast<T*>(array.get()));
		array.add(sizeof(T));
		return tmp;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif