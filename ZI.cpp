// ZI.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <clocale>
#include <Windows.h>
#include "common/FileHelper.h"
#include "lab1/algorithm/VernamCipher.h"

using namespace std;
using namespace files;
using namespace algorithm;

void lab1();

int main() {
    setlocale(LC_ALL, "Rus");

    lab1();

    cin.get();
    return 0;
}

void lab1() {
    FileHelper helper = 1;

    string sampleText = helper.readSampleText();
    
    ByteArray key = VernamCipher::generateKey(sampleText.length());
    helper.writeEncryptionKey(key);

    key = helper.readEncryptionKey();
    ByteArray en = VernamCipher::encrypt(sampleText, key);
    helper.writeCipherText(en);
    
    en = helper.readCipherText();
    string de = VernamCipher::decrypt(en, key);

    cout << "Изначальный текст:\n" << sampleText << endl << endl;
    cout << "Ключ шифрования:\n" << key << endl << endl;
    cout << "Зашифрованный текст:\n" << en << endl << endl;
    cout << "Расшифрованный текст:\n" << de;
}