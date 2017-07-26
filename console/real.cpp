#include "real.h"
static int block_size = 7;
static int64_t max_int = 10000000000000000; //для сравнений с восемнадцатизначными числами, как альтернативу можно рассмотреть 2^15 для сравнения нужного бита,
											//но оно меньше.
											/*Ограничение количества семибитных блоков при записи мантиссы. Задаёт возможность хранения 16*max_blocks + 16 значащих цифр числа.
											сейчас 96.
											Степень ограничения условно не имеет.*/
static int max_blocks = 5;

namespace Project {
	namespace Core {
		using namespace Project;
		using namespace Project::Core;
		using namespace std;

		void real::reset_type()
		{
			type.sign = 0;
			type.exp_sign = 0;
			type.err = 0;
			type.integer = 0;
			type.rational = 0;
			type.real = 0;
			type.endless = 0;
		}

		void real::test()
		{
			bool t=0;
			byte a, b, x;
			uint8_t c, d;
			block b1;
			int s = sizeof(b1);
			real m = real();
			real k;
			for (int count = 0; count < 8; count++) {
				d = m.mantissa_begin[0]._block_read_byte(count);
				c = k.mantissa_begin[0]._block_read_byte(count);
			}
		}

		real::real() throw (std::bad_alloc)
		{
			reset_type();			
			mantissa_begin = nullptr;
			mantissa_end = nullptr;
			denominator = nullptr;
			exp = nullptr;
			if (!mantissa_alloc(1)) {
				throw std::bad_alloc();
			}
					
		}

		bool real::mantissa_alloc(int _size)
		{
			mantissa_begin = nullptr;
			short count = 0;
			while (!mantissa_begin) {
				if (count > 5) return false;
				mantissa_begin = (block*)calloc(_size + 1, block_size);//на всякий случай лучше занулить содержимое сразу.
				count++;				
			}
			exp = mantissa_begin + _size;
			mantissa_end = mantissa_begin +(_size+1);			
			return true;
		}

		bool real::numerator_realloc()
		{
			if (!mantissa_begin||!exp) return false;
			block * new_mant = nullptr;
			block * new_den = nullptr;
			block * new_exp = nullptr;
			short count = 0;
			size_t len = mantissa_end - mantissa_begin;//общая длина
			size_t num_len = 0;
			size_t den_len = 0;
			size_t exp_len = mantissa_end - exp;//длина степени
			while (!new_mant) {
				if (count > 5) return false;
				new_mant = (block*)calloc(len + 1, block_size);//на всякий случай лучше занулить содержимое сразу.
				count++;
			}
			//
			if (denominator){//только denominator может быть нулевым.
				num_len = denominator - mantissa_begin; //длина числителя
				den_len = exp - denominator; //длина знаменателя
				new_den = new_mant + (num_len + 1);
				new_exp = new_den + den_len;
				denominator = (block*)memcpy(new_den, denominator, den_len*block_size);
			}
			else{
				num_len = exp - mantissa_begin; //длина числителя
				new_exp = new_mant + (num_len + 1);
			}
			memcpy(new_mant, mantissa_begin, num_len*block_size);
			exp = (block*)memcpy(new_exp, exp, exp_len*block_size);
			//
			free(mantissa_begin);		
			mantissa_begin = new_mant;
			mantissa_end = mantissa_begin + (len + 1);
			return true;
		}

		bool real::denominator_realloc()
		{
			if (!mantissa_begin || !exp) return false;
			block * new_mant = nullptr;
			block * new_den = nullptr;
			block * new_exp = nullptr;
			short count = 0;
			size_t len = mantissa_end - mantissa_begin;//общая длина
			size_t num_len = 0;
			size_t den_len = 0;
			size_t exp_len = mantissa_end - exp;//длина степени
			while (!new_mant) {
				if (count > 5) return false;
				new_mant = (block*)calloc(len + 1, block_size);//на всякий случай лучше занулить содержимое сразу.
				count++;
			}
			//
			if (denominator) {//только denominator может быть нулевым.
				num_len = denominator - mantissa_begin; //длина числителя
				den_len = exp - denominator; //длина знаменателя
				new_den = new_mant + num_len;
				new_exp = new_den + (den_len + 1);
				denominator = (block*)memcpy(new_den, denominator, den_len*block_size);
			}
			else {
				num_len = exp - mantissa_begin; //длина числителя
				denominator = new_mant + num_len;
				new_exp = denominator + 1;				
			}
			memcpy(new_mant, mantissa_begin, num_len*block_size);
			exp = (block*)memcpy(new_exp, exp, exp_len*block_size);
			//
			free(mantissa_begin);
			mantissa_begin = new_mant;
			mantissa_end = mantissa_begin + (len + 1);
			return true;
		}

		bool real::exp_realloc()
		{
			if (!mantissa_begin || !exp) return false;
			block * new_mant = nullptr;
			block * new_den = nullptr;
			block * new_exp = nullptr;
			short count = 0;
			size_t len = mantissa_end - mantissa_begin;//общая длина
			size_t num_len = 0;
			size_t den_len = 0;
			size_t exp_len = mantissa_end - exp;//длина степени
			while (!new_mant) {
				if (count > 5) return false;
				new_mant = (block*)calloc(len + 1, block_size);//на всякий случай лучше занулить содержимое сразу.
				count++;
			}
			//
			if (denominator) {//только denominator может быть нулевым.
				num_len = denominator - mantissa_begin; //длина числителя
				den_len = exp - denominator; //длина знаменателя
				new_den = new_mant + num_len;
				new_exp = new_den + den_len;
				denominator = (block*)memcpy(new_den, denominator, den_len*block_size);
			}
			else {
				num_len = exp - mantissa_begin; //длина числителя
				new_exp = new_mant + num_len;
			}
			memcpy(new_mant, mantissa_begin, num_len*block_size);
			exp = (block*)memcpy(new_exp, exp, exp_len*block_size);
			//
			free(mantissa_begin);
			mantissa_begin = new_mant;
			mantissa_end = mantissa_begin + (len + 1);
			return true;
		}

		

		


		
		
		//BYTE
		real::byte::byte()
		{
			_byte_reset_all();
		}
		real::byte::byte(char _ch)
		{//конструктор заполняет все биты на основе переданного char
			_byte_reset_all();
			uint8_t power_2 = 1;
			char temp = _ch;			
			bool buf;
			for (int count = 0; count < block_size+1; count++) {
				buf = (temp & power_2) >> count;
				if (buf)
					this->_byte_set(count);
				else
					this->_byte_reset(count);
				power_2 *= 2;
			}			
		}

		real::byte::byte(const real::byte & _byte)
		{
			this->bit_0 = _byte.bit_0;
			this->bit_1 = _byte.bit_1;
			this->bit_2 = _byte.bit_2;
			this->bit_3 = _byte.bit_3;
			this->bit_4 = _byte.bit_4;
			this->bit_5 = _byte.bit_5;
			this->bit_6 = _byte.bit_6;
			this->bit_s = _byte.bit_s;
		}
		
		void real::byte::_byte_set_all()
		{
			bit_0 = 1;
			bit_1 = 1;
			bit_2 = 1;
			bit_3 = 1;
			bit_4 = 1;
			bit_5 = 1;
			bit_6 = 1;
			bit_s = 1;
		}
		void real::byte::_byte_reset_all()
		{
			bit_0 = 0;
			bit_1 = 0;
			bit_2 = 0;
			bit_3 = 0;
			bit_4 = 0;
			bit_5 = 0;
			bit_6 = 0;
			bit_s = 0;
		}
		bool real::byte::_byte_at(int _place)
		{
			switch (_place)
			{
			case 0:
				return bit_0;
			case 1:
				return bit_1;
			case 2:
				return bit_2;
			case 3:
				return bit_3;
			case 4:
				return bit_4;
			case 5:
				return bit_5;
			case 6:
				return bit_6;
			case 7:
				return bit_s;
			default:
				return false;
			}
		}
		void real::byte::_byte_set(int _place)
		{
			switch (_place)
			{
			case 0:
			{bit_0 = 1; break; }
			case 1:
			{bit_1 = 1; break; }
			case 2:
			{bit_2 = 1; break; }
			case 3:
			{bit_3 = 1; break; }
			case 4:
			{bit_4 = 1; break; }
			case 5:
			{bit_5 = 1; break; }
			case 6:
			{bit_6 = 1; break; }
			case 7:
			{bit_s = 1; break; }
			default:
				break;
			}
		}
		void real::byte::_byte_reset(int _place)
		{
			switch (_place)
			{
			case 0:
			{bit_0 = 0; break; }
			case 1:
			{bit_1 = 0; break; }
			case 2:
			{bit_2 = 0; break; }
			case 3:
			{bit_3 = 0; break; }
			case 4:
			{bit_4 = 0; break; }
			case 5:
			{bit_5 = 0; break; }
			case 6:
			{bit_6 = 0; break; }
			case 7:
			{bit_s = 0; break; }
			default:
				break;
			}
		}
		void real::byte::_byte_reverse_at(int _place)
		{
			switch (_place)
			{
			case 0:{
				if (bit_0 == 0)
				bit_0 = 1;
			else
				bit_0 = 0;
			break; }
			case 1:{
				if (bit_1 == 0)
				bit_1 = 1;
			else
				bit_1 = 0;
			break; }
			case 2:{
				if (bit_2 == 0)
				bit_2 = 1;
			else
				bit_2 = 0; 
			break; }
			case 3:{
				if (bit_3 == 0)
				bit_3 = 1;
			else
				bit_3 = 0;
			break; }
			case 4:{
				if (bit_4 == 0)
				bit_4 = 1;
			else
				bit_4 = 0;
			break; }
			case 5:{
				if (bit_5 == 0)
				bit_5 = 1;
			else
				bit_5 = 0;
			break; }
			case 6:{
				if (bit_6 == 0)
				bit_6 = 1;
			else
				bit_6 = 0; break; }
			/*case 7:
			{bit_s = ~bit_s; break; }*/
			default:
				break;
			}
		}
		void real::byte::_byte_reverse()
		{
			if (bit_0)
				bit_0 = 0;
			else
				bit_0 = 1;

			if (bit_1)
				bit_1 = 0;
			else
				bit_1 = 1;

			if (bit_2)
				bit_2 = 0;
			else
				bit_2 = 1;

			if (bit_3)
				bit_3 = 0;
			else
				bit_3 = 1;

			if (bit_4)
				bit_4 = 0;
			else
				bit_4 = 1;

			if (bit_5)
				bit_5 = 0;
			else
				bit_5 = 1;

			if (bit_6)
				bit_6 = 0;
			else
				bit_6 = 1;

			if (bit_s)
				bit_s = 0;
			else
				bit_s = 1;
		}
		real::byte & real::byte::_byte_copy(const byte &_original)
		{//так же копируется восьмой бит
			if (&_original == this)
				return *this;
			this->bit_0 = _original.bit_0;
			this->bit_1 = _original.bit_1;
			this->bit_2 = _original.bit_2;
			this->bit_3 = _original.bit_3;
			this->bit_4 = _original.bit_4;
			this->bit_5 = _original.bit_5;
			this->bit_6 = _original.bit_6;
			this->bit_s = _original.bit_s;
			return *this;
		}

		bool real::byte::_byte_compare(const real::byte & _left)
		{
			//учитываетсяя восьмой бит
			if (this->bit_s != _left.bit_s)
				return false;
			return (*this == _left);
		}

		real::byte * real::byte::_byte_divide(const real::byte & _left)
		{
			byte res, rem, buf, left, zero;
			left ._byte_copy(_left);
			if (left == zero){
				return nullptr;
			}
			rem._byte_copy(*this);			
			buf._byte_copy(rem);
			while (!(rem -= left)) {
				buf._byte_copy(rem);
				res++;				
			}
			rem = buf;
			byte* out = new byte[2];
			out[0] = res;
			out[1] = rem;
			return out;
		}

		bool real::byte::_byte_summator(int _place, bool _x, bool _rank)
		{
			bool buf = this->_byte_at(_place);
			if (buf && _x && _rank) {
				this->_byte_set(_place);
				return true;
			}
			else if (buf && _x || _x && _rank || buf && _rank) {
				this->_byte_reset(_place);
				return true;
			}
			else if (buf || _x || _rank) {
				this->_byte_set(_place);
				return false;
			}			
			return false;
		}

		real::byte real::byte::_byte_hundred()
		{
			byte out;
			out.bit_2 = 1;
			out.bit_5 = 1;
			out.bit_6 = 1;
			return out;
		}

		bool real::byte::operator<(const real::byte & _left)
		{			
			byte buf(_left);
			for (int count = block_size; count >= 0; count--) {
				if (this->_byte_at(count) < buf._byte_at(count)) {
					return true;
				}
			}
			return false;
		}

		bool real::byte::operator<(const uint8_t & _left)
		{
			byte buf(_left);
			return (*this < buf);
		}

		bool real::byte::operator>(const real::byte & _left)
		{
			byte buf(_left);			
			for (int count = block_size; count >= 0; count--) {
				if (this->_byte_at(count) > buf._byte_at(count)) {
					return true;
				}
			}
			return false;
		}

		bool real::byte::operator>(const uint8_t & _left)
		{
			byte buf(_left);
			return (*this > buf);
		}

		bool real::byte::operator<=(const real::byte & _left)
		{
			if (*this == _left)
				return true;
			byte buf(_left);
			for (int count = block_size; count > 0; count--) {
				if (this->_byte_at(count-1) < buf._byte_at(count-1)) {
					return true;
				}
			}
			return false;
		}

		bool real::byte::operator<=(const char & _left)
		{
			byte buf;
			buf = _left;
			return (*this <= buf);
		}

		bool real::byte::operator>=(const real::byte & _left)
		{
			if (*this == _left)
				return true;
			byte buf(_left);
			for (int count = block_size; count >0; count--) {
				if (this->_byte_at(count - 1) > buf._byte_at(count - 1)) {
					return true;
				}
			}
			return false;
		}

		bool real::byte::operator>=(const char & _left)
		{
			byte buf;
			buf = _left;
			return (*this >= buf);
		}


		bool real::byte::operator==(const real::byte & _left)
		{//данный оператор игнорирует восьмой бит. 
			byte buf;
			buf=_left;
			for (int count = block_size; count > 0; count--) {
				if (this->_byte_at(count - 1) != buf._byte_at(count - 1)) {
					return false;
				}
			}
			return true;
		}

		bool real::byte::operator!=(const real::byte & _left)
		{
			return ~(*this == _left);			
		}

		real::byte real::byte::operator~()
		{
			byte out;
			out._byte_copy(*this);
			if (out.bit_0)
				out.bit_0 = 0;
			else
				out.bit_0 = 1;

			if (out.bit_1)
				out.bit_1 = 0;
			else
				out.bit_1 = 1;

			if (out.bit_2)
				out.bit_2 = 0;
			else
				out.bit_2 = 1;

			if (out.bit_3)
				out.bit_3 = 0;
			else
				out.bit_3 = 1;

			if (out.bit_4)
				out.bit_4 = 0;
			else
				out.bit_4 = 1;

			if (out.bit_5)
				out.bit_5 = 0;
			else
				out.bit_5 = 1;

			if (out.bit_6)
				out.bit_6 = 0;
			else
				out.bit_6 = 1;

			return out;
		}

		real::byte & real::byte::operator=(const byte & _left)
		{
			if (&_left == this)
				return *this;			
			this->bit_0 = _left.bit_0;
			this->bit_1 = _left.bit_1;
			this->bit_2 = _left.bit_2;
			this->bit_3 = _left.bit_3;
			this->bit_4 = _left.bit_4;
			this->bit_5 = _left.bit_5;
			this->bit_6 = _left.bit_6;
			return *this;
		}

		real::byte & real::byte::operator=(const uint8_t & _left)
		{
			uint8_t power_2 = 1;
			uint8_t temp = _left;
			if (temp > 99) {
				temp = temp % 100;
			}
			bool buf;
			for (int count = 0; count < block_size; count++) {
				buf = (temp & power_2) >> count;
				if (buf)
					this->_byte_set(count);
				else
					this->_byte_reset(count);
				power_2 *= 2;
			}
			return *this;
		}

		real::byte::operator uint8_t()
		{
			uint8_t power_2 = 1;
			uint8_t out = 0;
			for (int count = 0; count < block_size; count++) {
				out += ((uint8_t)(this->_byte_at(count)))*power_2;
				power_2 *= 2;
			}
			return out;
		}

		real::byte::operator char()
		{
			char power_2 = 1;
			char out = 0;
			for (int count = 0; count < block_size; count++) {
				out += ((char)(this->_byte_at(count)))*power_2;
				power_2 *= 2;
			}
			return out;
		}
		
		real::byte real::byte::operator+(const real::byte &_left)
		{
			byte left;
			left._byte_copy(_left);
			bool buf = false;
			for (int count = 0; count < block_size + 1; count++) {
				buf = left._byte_summator(count, this->_byte_at(count), buf);
			}			
			return left;
		}

		bool real::byte::operator+=(const real::byte &_left)
		{//оператор прибавляет к данному экземпляру число слева. Возвращается 1 если произошёл выход за пределы отрезка [0;99]. При этом записывается результат с вычетом сотни.
			byte left; 
			left = _left;			
			byte _this;
			_this = *this;
			_this ._byte_copy(_this + left);
			byte * div = _this._byte_divide(_byte_hundred());
			*this = div[1];
			if (div[0] > byte()) {
				return true;
			}
			return false;
		}

		real::byte & real::byte::operator++(int)
		{
			byte one = (uint8_t)1;
			*this = *this + one;
			return *this;
		}

		real::byte real::byte::operator-(const real::byte & _left)
		{//поскольку необходимо лишь вычитание чисел в пределах [0;99] _left должно быть только положительным числом.
			byte left;
			left = _left;
			left._byte_reverse();
			left++;
			byte buf;
			buf._byte_copy(*this + left);
			if (buf.bit_s){//отрицательное число берётся по модулю.
				buf._byte_reverse();
				buf++;
			}			
			return buf;
		}

		bool real::byte::operator-=(const real::byte & _left)
		{
			byte left;
			left = _left;
			left._byte_reverse();
			left++;
			byte buf;			
			buf._byte_copy(*this + left);
			if (buf.bit_s) {//отрицательное число берётся по модулю.
				buf._byte_reverse();
				buf++;
				this->_byte_copy( buf);
				return true;
			}
			else{
				this->_byte_copy( buf);
				return false;
			}			
		}
		real::byte real::byte::operator/(const byte & _left)
		{
			byte* res;
			res = this->_byte_divide(_left);
			if (res == nullptr) {
				byte s;
				s._byte_set_all();
				return s;
			}
			byte out = res[0];
			delete[]res;
			return out;
		}
		real::byte real::byte::operator%(const byte & _left)
		{
			byte* res;
			res = this->_byte_divide(_left);
			if (res == nullptr) {
				return byte();
			}
			byte out = res[1];
			delete[]res;
			return out;
		}



		//BLOCK

		real::block::block()
		{
			_block_reset_all();
		}

		real::block::block(byte _0)
		{
			_block_reset_all();
			_block_0_byte_write(_0);
		}

		real::block::block(byte _0, byte _1, byte _2, byte _3, byte _4, byte _5, byte _6, byte _7)
		{
			byte_1 = _1;
			byte_2 = _2;
			byte_3 = _3;
			byte_4 = _4;
			byte_5 = _5;
			byte_6 = _6;
			byte_7 = _7;
			_block_0_byte_write(_0);
		}

		real::block::block(uint8_t _0, uint8_t _1, uint8_t _2, uint8_t _3, uint8_t _4, uint8_t _5, uint8_t _6, uint8_t _7)
		{
			byte_1 = _1;
			byte_2 = _2;
			byte_3 = _3;
			byte_4 = _4;
			byte_5 = _5;
			byte_6 = _6;
			byte_7 = _7;
			byte buf = _0;
			_block_0_byte_write(buf);
		}



		void real::block::_block_reset_all()
		{
			byte_1._byte_reset_all();
			byte_2._byte_reset_all();
			byte_3._byte_reset_all();
			byte_4._byte_reset_all();
			byte_5._byte_reset_all();
			byte_6._byte_reset_all();
			byte_7._byte_reset_all();
		}

		void real::block::_block_0_byte_write(const byte &_0)
		{//полагается что _7 находится в [0,99]
			byte_1.bit_s = _0.bit_0;
			byte_2.bit_s = _0.bit_1;
			byte_3.bit_s = _0.bit_2;
			byte_4.bit_s = _0.bit_3;
			byte_5.bit_s = _0.bit_4;
			byte_6.bit_s = _0.bit_5;
			byte_7.bit_s = _0.bit_6;
		}

		real::byte real::block::_block_0_byte_read()
		{
			byte out;
			out.bit_0 = this->byte_1.bit_s;
			out.bit_1 = this->byte_2.bit_s;
			out.bit_2 = this->byte_3.bit_s;
			out.bit_3 = this->byte_4.bit_s;
			out.bit_4 = this->byte_5.bit_s;
			out.bit_5 = this->byte_6.bit_s;
			out.bit_6 = this->byte_7.bit_s;
			return out;
		}
		//Возвращает число (однозначное) стоящее на месте _place.
		real::byte real::block::_block_at(int _place)
		{
			bool byte_rank = _place % 2;
			int block_rank = _place / 2;
			byte buf;//0
			byte ten = (uint8_t)10;
			switch (block_rank) {
			case 0: {
				buf = this->_block_0_byte_read();
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 1: {
				buf = this->byte_1;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 2: {
				buf = this->byte_2;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 3: {
				buf = this->byte_3;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 4: {
				buf = this->byte_4;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 5: {
				buf = this->byte_5;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 6: {
				buf = this->byte_6;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			case 7: {
				buf = this->byte_7;
				if (byte_rank) {
					buf = buf % ten;
				}
				else {
					buf = buf / ten;
				}
				return buf;
			}
			default: return buf;
			}
		}

		real::byte real::block::_block_read_byte(int _place)
		{
			switch(_place){
			case 0:{
			return _block_0_byte_read();
			}
			case 1: return byte_1;
			case 2: return byte_2;
			case 3: return byte_3;
			case 4: return byte_4;
			case 5: return byte_5;
			case 6: return byte_6;
			case 7: return byte_7;
			default: return byte();//0
			}
		}

		void real::block::_block_write_byte(int _place, const real::byte & _orig)
		{
			switch (_place) {
			case 0: {
				_block_0_byte_write(_orig);
				break;
			}
			case 1: {
				byte_1 = _orig;
				break;
			}
			case 2: {
				byte_2 = _orig;
				break;
			}
			case 3: {
				byte_3 = _orig;
				break;
			}
			case 4: {
				byte_4 = _orig;
				break;
			}
			case 5: {
				byte_5 = _orig;
				break;
			}
			case 6: {
				byte_6 = _orig;
				break;
			}
			case 7: {
				byte_7 = _orig;
				break;
			}
			default:break;
			}
		}

		

		real::block & real::block::operator=(const real::block & _left)
		{//бессмысленная перегрузка. И беспощадная
			this->byte_1._byte_copy(_left.byte_1);
			this->byte_2._byte_copy(_left.byte_2);
			this->byte_3._byte_copy(_left.byte_3);
			this->byte_4._byte_copy(_left.byte_4);
			this->byte_5._byte_copy(_left.byte_5);
			this->byte_6._byte_copy(_left.byte_6);
			this->byte_7._byte_copy(_left.byte_7);
			return *this;
		}

		bool real::block::operator+=(const real::block &_left)
		{
			byte buf;
			block _this = *this;
			block left = _left;
			bool rank = 0, rank_buf = 0;//rank проверяет выход за пределы отрезка [0,99]. Если выход случился - необходимо на следующем шаге прибавлять единицу.
			for (int count = block_size; count >= 0; count--) {
				buf = _this._block_read_byte(count);
				rank_buf = (buf += left._block_read_byte(count));
				rank = (buf += (uint8_t)rank);
				if (rank_buf)//в данном случае и rank_buf, и rank одновременно единицей не не будут. даже при сложении 99+99+1 = 199, т.е. выход за пределы сотни происходит единожды.
					rank = rank_buf;
				_this._block_write_byte(count, buf);
			}
			*this = _this;
			return rank;//выход за пределы сотни в самом старшем байте.
		}

		real::block::operator real::byte()
		{
			byte out = this->byte_7;
			return out;
		}
		
}
}