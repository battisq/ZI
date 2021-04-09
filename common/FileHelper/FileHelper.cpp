#include "FileHelper.h"

#include <stdexcept>
#include <fstream>
#include <sstream>

namespace files {
	
	FileHelper::FileHelper(int labNum) {
		if (labNum < 0 || labNum > 4)
			throw new invalid_argument("This lab is not listed");

		path = PROJECT_PATH + "\\lab" + to_string(labNum) +"\\files\\";
	}

	FileHelper::~FileHelper() { }

	string FileHelper::readSampleText() {
		return readText(path + SAMPLE_TEXT);
	}

	void FileHelper::writeEncryptionKey(ByteArray key) {
		writeBytes(path + ENCRYPTION_KEY, key);
	}

	ByteArray FileHelper::readByteEncryptionKey() {
		return readBytes(path + ENCRYPTION_KEY);
	}

	string FileHelper::readStringEncryptionKey() {
		return readText(path + ENCRYPTION_KEY);
	}

	void FileHelper::writeCipherText(ByteArray text) {
		writeBytes(path + CIPHER_TEXT, text);
	}

	ByteArray FileHelper::readCipherText() {
		return readBytes(path + CIPHER_TEXT);
	}

	void FileHelper::writeDecryptedText(string text) {
		writeText(path + DECRYPTED_TEXT, text);
	}

	string FileHelper::readDecryptedText() {
		return readText(path + DECRYPTED_TEXT);
	}

	FileHelper FileHelper::operator=(const FileHelper& helper) {
		path = helper.path;
		
		return *this;
	}

	string FileHelper::readText(const string path) {
		string tempText;
		stringstream fullText;
		ifstream  in(path);
		
		if (in.is_open()) {
			while (getline(in, tempText)) {
				fullText << tempText;
				
				if (!in.eof())
					fullText << endl;
			}
		}

		in.close();

		return fullText.str();
	}

	void FileHelper::writeText(const string path, const string text) {
		ofstream out(path, ios::out);

		if (out.is_open()) {
			out << text;
		}

		out.close();
	}

	const ByteArray FileHelper::readBytes(const string path) {
		string tempText;
		stringstream fullText;
		ifstream in(path, ios_base::binary);

		if (in.is_open()) {
			while (getline(in, tempText)) {
				fullText << tempText;

				if (!in.eof())
					fullText << endl;
			}
		}
		
		in.close();

		return ByteArray(fullText.str());
	}

	void FileHelper::writeBytes(string path, const ByteArray text) {
		ofstream out(path, ios_base::binary);

		if (out.is_open()) {
			for (int i = 0; i < text.getLength(); i++) {
				out << text[i];
			}
		}

		out.close();
	}
}