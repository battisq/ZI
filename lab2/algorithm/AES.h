#pragma once

#include <string>
#include "../../common/ByteArray/ByteArray.h"
#include "../../common/Matrix/TMatrix.h"

using namespace std;
using namespace Mathematics;

namespace algorithms {

	class AES {

	private:

		int Nb, Nk, Nr;
		TMatrix<byte> w;

		TMatrix<byte> addRoundKey(TMatrix<byte> state, TMatrix<byte> w, int round);
		TMatrix<byte> invMixColumns(TMatrix<byte> s);
		TMatrix<byte> invShiftRows(TMatrix<byte> state);
		TMatrix<byte> invSubBytes(TMatrix<byte> state);
		TMatrix<byte> mixColumns(TMatrix<byte> s);
		ByteArray rotWord(ByteArray input);
		TMatrix<byte> shiftRows(TMatrix<byte> state);
		TMatrix<byte> subBytes(TMatrix<byte> state);
		ByteArray subWord(ByteArray inp);

		byte ffMul(byte a, byte b);
		TMatrix<byte> generateSubkeys(ByteArray key);
		ByteArray xorFunction(ByteArray a, ByteArray b);
		ByteArray encryptBloc(ByteArray inp);
		ByteArray decryptBloc(ByteArray inp);

	public:
		AES();

		ByteArray encrypt(string text, string key);
		string decrypt(ByteArray encryptText, string key);
	};

};