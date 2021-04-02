#pragma once
#include <string>

typedef unsigned char byte;

using namespace std;

class ByteArray {
private:
	byte* bytes;
	int length;

public:

	ByteArray(int length);
	explicit ByteArray(string text);
	ByteArray(const ByteArray &byteArray);
	~ByteArray();

	byte & operator[] (int i);
	const byte operator[] (int i) const;

	ByteArray& operator =  (const ByteArray& byteArray);
	//ByteArray& operator =  (ByteArray&& byteArray);

	int getLength() const;

	string toString() const;

	friend std::ostream& operator << (std::ostream& out, const ByteArray& byteArray);
};

