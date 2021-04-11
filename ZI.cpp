#include <iostream>
#include <clocale>
#include <Windows.h>
#include <stdio.h>
#include "common/FileHelper/FileHelper.h"
#include "lab1/algorithm/VernamCipher.h"
#include "lab2/algorithm/AES.h"
#include "lab3/algorithm/ShamirСipher.h"

using namespace std;
using namespace files;
using namespace algorithms;

void lab1();
void lab2();
void lab3();

static FileHelper *helper;

int main() {
    setlocale(LC_ALL, "Rus");

    //long a = 27, n = 796;


    //printf("the inverse of %ld modulo %2ld is %ld\n", a, n, inverse(a, n));

    //a = 151, n = 796;

    //printf("the inverse of %ld modulo %2ld is %ld\n", a, n, inverse(a, n));


    int lab = 3;
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

        case 3: {
            lab3();
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

void lab3() {
    try {
        int p = 739;

        ShamirСipher chipher = p;

        string sampleText = helper->readSampleText();

        chipher.run(sampleText, *helper);
    }
    catch (exception ex) {
        cout << ex.what();
    }
}
