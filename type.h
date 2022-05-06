#ifndef TYPE_H
#define TYPE_H 1
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <cstdint>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
namespace type
{
	class Array
	{
		public:
			Array();
			~Array();

			Array(const Array&) =delete;
			Array(Array&&) =delete;

			Array& operator=(const Array&) =delete;
			Array& operator=(Array&&) =delete;

			void alloc(uint32_t size);
			void add(uint32_t);
			char* get();

		private:
			char* array;
			uint32_t sz;

			void clear();
	};
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