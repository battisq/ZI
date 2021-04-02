#pragma once

#include <string>
#include "../common/ByteArray.h"

using namespace std;

namespace files {

	class FileHelper {
	
	private:
		const string PROJECT_PATH 
			= "C:\\MY DATA\\Учёба\\4 курс\\8 семестр\\Защита информации\\Лабы\\ZI";
		
		const string LAB_PATH = PROJECT_PATH + "\\lab%d\\files\\";

		const string SAMPLE_TEXT = "sample_text.txt";
		const string ENCRYPTION_KEY = "encryption_key.txt";
		const string CIPHER_TEXT = "cipher_text.txt";
		const string DECRYPTED_TEXT = "decrypted_text.txt";

		string path;

		string readText(const string path);
		void writeText(const string path, const string text);

		const ByteArray readBytes(const string path);
		void writeBytes(string path, const ByteArray text);

	public:
		FileHelper(int labNum);
		~FileHelper();
	
		string readSampleText();

		void writeEncryptionKey(ByteArray key);
		ByteArray readEncryptionKey();

		void writeCipherText(ByteArray text);
		ByteArray readCipherText();

		void writeDecryptedText(string text);
		string readDecryptedText();
	};
}