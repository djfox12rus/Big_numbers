#pragma once
#include "libs.h"
namespace Project {
	namespace Core {
		using namespace std;
		using namespace Project;
		
		
		typedef class real	{
		private:
			typedef struct byte {
				bool bit_0 : 1;
				bool bit_1 : 1;
				bool bit_2 : 1;
				bool bit_3 : 1;
				bool bit_4 : 1;
				bool bit_5 : 1;
				bool bit_6 : 1;
				bool bit_s : 1;

				byte();
				byte(char _ch);
				byte(const real::byte &_byte);
				void _byte_set_all();
				void _byte_reset_all();
				bool _byte_at(int _place);
				void _byte_set(int _place);
				void _byte_reset(int _place);
				void _byte_reverse_at(int _place);
				void _byte_reverse();
				byte &_byte_copy(const byte &_original);
				bool _byte_compare(const real::byte & _left);
				byte* _byte_divide(const real::byte &_left);
				bool _byte_summator(int _place, bool _x, bool _rank);
				byte _byte_hundred();

				//операторы строгого сравнения учитывают восьмой бит.
				bool operator <(const real::byte &_left);
				bool operator <(const uint8_t &_left);
				bool operator >(const real::byte &_left);
				bool operator >(const uint8_t &_left);
				//операторы нестрогого сравнения не учитывают восьмой бит
				bool operator <=(const real::byte &_left);
				bool operator <=(const char &_left);
				bool operator >=(const real::byte &_left);
				bool operator >=(const char &_left);

				bool operator == (const real::byte & _left);
				bool operator != (const real::byte &_left);
				byte operator ~();
				byte &operator =(const byte & _left);
				byte &operator =(const uint8_t & _left);
				operator uint8_t();
				operator char();				
				byte operator + (const real::byte &_left);
				bool operator += (const real::byte &_left);
				byte &operator ++ (int);
				byte operator - (const real::byte &_left);
				bool operator -= (const real::byte &_left);
				byte operator /(const byte & _left);
				byte operator %(const byte & _left);

			};

			typedef struct block {
				byte byte_1;
				byte byte_2;
				byte byte_3;
				byte byte_4;
				byte byte_5;
				byte byte_6;
				byte byte_7;

				block();
				block(byte _0);
				block(byte _0, byte _1, byte _2, byte _3, byte _4, byte _5, byte _6, byte _7);
				block(uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3, uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7);

				void _block_reset_all();
				void _block_0_byte_write(const byte &_0);
				byte _block_0_byte_read();
				byte _block_at(int _place);
				byte _block_read_byte(int _place);
				void _block_write_byte(int _place, const real::byte & _orig);
				block & operator =(const real::block &_left);
				bool operator += (const real::block &_left);
				operator byte();



			};


			//1 байт
			struct {
				bool sign : 1;
				bool exp_sign : 1;
				bool err : 1;
				bool integer : 1;
				bool rational : 1;
				bool real : 1;
				bool endless : 1;
			} type;
			
			block * mantissa_begin;

			block * mantissa_end;

			block * exp;

			block * denominator;

			void reset_type();
			
			//bool construct_from_int(int64_t _num);

		//	bool construct_from_wstr(wchar_t * str, wchar_t ** remap);

		//	char * block_push_back(uint64_t _new, unsigned int _count_blocks);

		//	char* block_push_fw(uint64_t _new);

		//	wstring* format(wstring* _out, int8_t count_i, size_t _int_len, size_t _decimal_len);

		//	int8_t mantissa_at(uint64_t _place);

		//	real add(real *_left);

		//	

		//	//bool push_fowards(unsigned char _new, unsigned int *_count_mant, unsigned char _exp);


		public:
			void test();


			//Нулевой конструктор (можно расматривать как конструктор нуля). Выделяются два блока, один под число, другой под степень. Всё инициализируется нулями.
			real() throw(std::bad_alloc);

		//	//Конструктор по целочисленному типу.
		//	real(int8_t _num);  //char
		//	real(int16_t _num); //short, иногда int
		//	real(int32_t _num); //int и long
		//	real(int64_t _num); //long long

		//	//Конструктор по типу с плавающей точкой
		//	//real(long double _num);
		//	//Конструктор по строке символов (произвольный). В случае sign = false записывается число без знака.
		//	real(wchar_t * str, wchar_t ** remap, bool sign);
		//	//Деструктор КО
		//	~real();

			//Первичная инициализация мантиссы. В дополнение к размеру _size выделяется также ещё один блок для степени.
			bool mantissa_alloc(int _size);		
			//Расширение числителя/самого числа на один блок.
			bool numerator_realloc();
			//Расширение знаменателя дроби на один блок.
			bool denominator_realloc();
			//Расширение степени на один блок.
			bool exp_realloc();
		//	
		//	wstring get_str(size_t _int_len, size_t _decimal_len);

		// 	int size_of();

		//	void copy(real *_original);

		//	//real& operator=(const long long _num);
		//	//real& operator=(const long double _num);
		//	//real& operator=(const real _num);

		//	/*real operator+=(const long long _num);
		//	real operator+=(const long double _num);
		//	real operator+=(const real _num);

		//	real operator-=(const long long _num);
		//	real operator-=(const long double _num);
		//	real operator-=(const real _num);

		//	real operator*=(const long long _num);
		//	real operator*=(const long double _num);
		//	real operator*=(const real _num);

		//	real operator/=(const long long _num);
		//	real operator/=(const long double _num);
		//	real operator/=(const real _num);

		//	real operator^=(const long long _num);
		//	real operator^=(const long double _num);
		//	real operator^=(const real _num);

		//	real operator+(const real _left);*/
		//	
			
		};
	}
}