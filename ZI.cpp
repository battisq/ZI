#include <iostream>
#include <clocale>
#include <Windows.h>
#include "common/FileHelper/FileHelper.h"
#include "lab1/algorithm/VernamCipher.h"
#include "lab2/algorithm/AES.h"

using namespace std;
using namespace files;
using namespace algorithms;

void lab1();
void lab2();

static FileHelper *helper;

int main() {
    setlocale(LC_ALL, "Rus");

    int lab = 2;
    helper = new FileHelper(lab);

    switch (lab) {
        case 1: {
            lab1();
            break;
        }

        case 2: {
            lab2();
            break;
        }

        default:
            break;
    }

    cin.get();
    return 0;
}

void lab1() {
    string sampleText = helper->readSampleText();
     
    ByteArray key = VernamCipher::generateKey(sampleText.length());
    helper->writeEncryptionKey(key);

    key = helper->readByteEncryptionKey();
    ByteArray en = VernamCipher::encrypt(sampleText, key);
    helper->writeCipherText(en);
    
    en = helper->readCipherText();
    string de = VernamCipher::decrypt(en, key);

    cout << "Изначальный текст:\n" << sampleText << endl << endl;
    cout << "Ключ шифрования:\n" << key << endl << endl;
    cout << "Зашифрованный текст:\n" << en << endl << endl;
    cout << "Расшифрованный текст:\n" << de;
}

void lab2() {
    try {
        AES aes = AES();

        string key = helper->readStringEncryptionKey();
        string sampleText = helper->readSampleText();

        ByteArray en = aes.encrypt(sampleText, key);
        helper->writeCipherText(en);

        en = helper->readCipherText();
        string de = aes.decrypt(en, key);
        helper->writeDecryptedText(de);

        cout << "Изначальный текст:\n" << sampleText << endl << endl;
        cout << "Ключ шифрования:\n" << key << endl << endl;
        cout << "Зашифрованный текст:\n" << en << endl << endl;
        cout << "Расшифрованный текст:\n" << de;
    } 
    catch (exception ex) {
        cout << ex.what();
    }
}
