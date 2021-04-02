#include "VernamCipher.h"
#include <ctime>

ByteArray algorithm::VernamCipher::generateKey(const int length) {
	ByteArray bytes = length;
	
	srand(time(0));

	for (int i = 0; i < length; i++) {
		bytes[i] = ((rand() % (255 + 1 - 0)) + 0);
	}

	return bytes;
}

const ByteArray algorithm::VernamCipher::encrypt(const string text, const ByteArray key) {
	ByteArray bytes = key.getLength();
	ByteArray text_ = ByteArray((string)text);
	
	for (int i = 0; i < key.getLength(); i++) {
		bytes[i] = key[i] ^ text_[i];
	}

	return bytes;
}

string algorithm::VernamCipher::decrypt(const ByteArray text, const ByteArray key) {
	ByteArray bytes = key.getLength();

	for (int i = 0; i < key.getLength(); i++) {
		bytes[i] = key[i] ^ text[i];
	}

	return bytes.toString();
}
