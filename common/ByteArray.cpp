#include "ByteArray.h"
#include <stdexcept>
#include <iostream>

ByteArray::ByteArray(int _length) {
	if (_length < 1)
		throw new invalid_argument("Длина должна быть больше 0");

	length = _length;
	bytes = new byte[length];
}

ByteArray::ByteArray(const string text) {

	if (text.length() < 1)
		throw new invalid_argument("Длина строки должна быть больше 0");

	length = text.length();
	bytes = new byte[length];

	const char* temp = text.c_str();

	for (int i = 0; i < length; i++) {
		bytes[i] = temp[i];
	}
}

ByteArray::ByteArray(const ByteArray& byteArray) {
	length = byteArray.length;
	bytes = new byte[length];

	for (int i = 0; i < length; i++) {
		bytes[i] = byteArray.bytes[i];
	}
}

ByteArray::~ByteArray() {
	delete[] bytes;
}

byte & ByteArray::operator[](int i) {
	if (i < 0 || i > length)
		throw new out_of_range("Выход за пределы массива. Размер массива:" + to_string(length) + ". Индекс:" + to_string(i) + ".");

	return bytes[i];
}

const byte ByteArray::operator[](int i) const {
	if (i < 0 || i > length)
		throw new out_of_range("Выход за пределы массива. Размер массива:" + to_string(length) + ". Индекс:" + to_string(i) + ".");

	return bytes[i];
}

ByteArray& ByteArray::operator=(const ByteArray& byteArray) {
	if (length != byteArray.length) {
		delete[] bytes;
		bytes = new byte[byteArray.length];
	}

	length = byteArray.length;

	for (int i = 0; i < length; i++) {
		bytes[i] = byteArray.bytes[i];
	}

	return *this;
}

//ByteArray& ByteArray::operator=(ByteArray&& byteArray) {
//	swap(length, byteArray.length);
//	swap(bytes, byteArray.bytes);
//
//	return *this;
//}

int ByteArray::getLength() const { return length; }

string ByteArray::toString() const {
	return string((char*)bytes, length);
}

std::ostream& operator<<(std::ostream& _out, const ByteArray& _a) {
	_out << _a.toString();
	return _out;
}