#include <iostream>
#include <clocale>
#include <Windows.h>
#include <stdio.h>
#include "common/FileHelper/FileHelper.h"
#include "lab3/algorithm/ShamirСipher.h"

using namespace std;
using namespace files;
using namespace algorithms;

void lab3();

static FileHelper *helper;

int main() {
    setlocale(LC_ALL, "Rus");

    int lab = 3;
    helper = new FileHelper(lab);

    lab3();
    
    cin.get();
    return 0;
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
