#include "ByteArray.h"
#include <stdexcept>
#include <iostream>

ByteArray::ByteArray(int _length, byte* byteArray) {
	if (_length < 0)
		throw new invalid_argument("Длина должна быть больше 0");

	length = _length;
	bytes = new byte[length];

	if (byteArray != NULL) {
		for (int i = 0; i < length; i++) {
			bytes[i] = byteArray[i];
		}
	}
}

ByteArray::ByteArray(int length, byte(*_function)(int)) : ByteArray(length) {
	for (int i = 0; i < length; i++)
		bytes[i] = _function(i);
}

ByteArray::ByteArray(int length, byte byte, ...) : ByteArray(length, &byte) {}

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

ByteArray::ByteArray(const ByteArray& byteArray) : ByteArray(byteArray.length, byteArray.bytes) {}

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

int ByteArray::getLength() const { return length; }

string ByteArray::toString() const {
	return string((char*)bytes, length);
}

string ByteArray::toNumArrayString() const {

	string res = "[";

	for (int i = 0; i < length; i++) {
		if (i != 0) {
			res += ", ";
		}

		res += to_string((int)bytes[i]);
	}

	res += "]";

	return res;
}

void ByteArray::copyTo(int srcPos, ByteArray* destination, int destPos, int countEl) const {

	if (srcPos < 0 || srcPos > length)
		throw new out_of_range("Стартовая позиция находится за пределами массива. Размер массива:" + to_string(countEl) + ". Индекс:" + to_string(srcPos) + ".");

	if (countEl < 0)
		throw new invalid_argument("Количество элементов не может быть отрицательным");

	
	if (countEl > length || srcPos + countEl > length)
		throw new invalid_argument("В массиве источнике меньше элементов, чем необходимо скопировать");


	for (int i = srcPos, j = destPos; i < srcPos + countEl; i++, j++) {
		destination->bytes[j] = bytes[i];
	}
}

ByteArray ByteArray::trim() const {
	int count = 0;

	int i = length - 1;
	while (bytes[i] == 0) {
		count++;
		i--;
	}

	ByteArray tmp = length - count - 1;
	copyTo(0, &tmp, 0, tmp.length);
	
	return tmp;
}

std::ostream& operator<<(std::ostream& _out, const ByteArray& _a) {
	_out << _a.toString();
	return _out;
}