#pragma once
#include <string>
#include <stdexcept>

using namespace std;

template <class Type> class List {
private:
	int length;
	Type* list;

public:

	List() {
		length = 0;
		list = NULL;
	}

	List(int _length, Type *_list = NULL) {
		if (_length < 1)
			throw invalid_argument("Длина должна быть больше 0");

		length = _length;
		list = new Type[length];

		if (_list != NULL) {
			for (int i = 0; i < length; i++) {
				list[i] = _list[i];
			}
		}
	}

	List(int length, Type(*_function)(int)) : List(length) {
		for (int i = 0; i < length; i++)
			list[i] = _function(i);
	}
	
	List(int length, Type el, ...) : List(length, &el) {}
	
	explicit List(string text, Type(*func)(char) = NULL) {
		if (text.length() < 1)
			throw invalid_argument("Длина строки должна быть больше 0");

		length = text.length();
		list = new Type[length];

		const char* temp = text.c_str();

		for (int i = 0; i < length; i++) {
			if (func == NULL)
				list[i] = temp[i];
			else
				list[i] = func(temp[i]);
		}
	}
	
	List(const List& list) : List(list.length, list.list) {}
	
	~List() {
		if (list != NULL)
			delete[] list;
	}

	Type& operator[] (int i) {
		if (i < 0 || i > length)
			throw out_of_range("Выход за пределы списка. Размер списка:" + to_string(length) + ". Индекс:" + to_string(i) + ".");

		return list[i];
	}
	
	const Type operator[] (int i) const {
		if (i < 0 || i > length)
			throw out_of_range("Выход за пределы списка. Размер списка:" + to_string(length) + ". Индекс:" + to_string(i) + ".");

		return list[i];
	}

	List& operator =  (const List& list) {
		if (length != list.length) {
			if (list != NULL)
				delete[] list;
			
			this.list = new Type[list.length];
		}

		length = list.length;

		for (int i = 0; i < length; i++) {
			this.list[i] = list.list[i];
		}

		return *this;
	}

	int getLength() const { return length; }
	
	string toString() const {
		string res = "[";

		for (int i = 0; i < length; i++) {
			if (i != 0) {
				res += ", ";
			}

			res += to_string(list[i]);
		}

		res += "]";

		return res;
	}

	string toSentence(char(*func)(Type) = NULL) {
		string res = "";

		for (int i = 0; i < length; i++) {
			if (func == NULL)
				res += to_string(list[i]);
			else
				res += func(list[i]);
		}

		return res;
	}

	friend std::ostream& operator << (std::ostream& out, const List& list) {
		out << list.toString();
		return out;
	}
};