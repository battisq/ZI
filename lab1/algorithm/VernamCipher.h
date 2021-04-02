#pragma once
#include <string>
#include "../../common/ByteArray.h"

typedef unsigned char byte;

using namespace std;

namespace algorithm {
	class VernamCipher {
	private:
		VernamCipher() { }

	public:
		static ByteArray generateKey(const int length);
		static const ByteArray encrypt(const string text, const ByteArray key);
		static string decrypt(const ByteArray text, const ByteArray key);
	};
}