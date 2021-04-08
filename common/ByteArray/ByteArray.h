#pragma once
#include <string>

typedef unsigned char byte;

using namespace std;

class ByteArray {
private:
	int length;
	byte* bytes;

public:
	ByteArray(int length, byte *byteArray = NULL);
	ByteArray(int length, byte(*_function)(int));
	ByteArray(int length, byte byte, ...);
	explicit ByteArray(string text);
	ByteArray(const ByteArray &byteArray);
	~ByteArray();

	byte & operator[] (int i);
	const byte operator[] (int i) const;

	ByteArray& operator =  (const ByteArray& byteArray);

	int getLength() const;
	string toString() const;
	string toNumArrayString() const;
	void copyTo(int srcPos, ByteArray* destination, int destPos, int length) const;
	ByteArray trim() const;

	friend std::ostream& operator << (std::ostream& out, const ByteArray& byteArray);
};