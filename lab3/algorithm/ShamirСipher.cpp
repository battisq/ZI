#include "ShamirСipher.h"
#include <ctime>
#include <random>
#include <chrono>
#include <stdexcept>
#include <clocale>
#include <iostream>
#include <cmath>
#include "AlphabetConfig.h"

namespace algorithms {
	long ShamirСipher::extended_euclid(long a, long b, long* x, long* y) {
        long q, r, x1, x2, y1, y2;

        if (b == 0) {
            *x = 1;
            *y = 0;

            return a;
        }

        x2 = 1, x1 = 0, y2 = 0, y1 = 1;

        while (b > 0) {
            q = a / b, r = a - q * b;

            *x = x2 - q * x1, * y = y2 - q * y1;

            a = b, b = r;

            x2 = x1, x1 = *x, y2 = y1, y1 = *y;
        }

        *x = x2;
        *y = y2;

        return a;
	}

    long ShamirСipher::inverse(long a, long n) {
        long d, x, y;

        d = extended_euclid(a, n, &x, &y);

        if (d == 1)
            return (x % n + n) % n;

        return 0;
    }

    void ShamirСipher::getInversePiar(long* first, long* second, int left, int right) {
        long localFirst = 0; 
        long localSecond = 0;

        do {
            localFirst = random(left, right);
            localSecond = inverse(localFirst, p - 1);

        } while (localSecond == 0);


        *first = localFirst;
        *second = localSecond;
    }

    bool ShamirСipher::isPrimeNumber(long long x) {
        if (x == 2)
            return true;
       
        srand(time(NULL));
        
        for (int i = 0; i < 100; i++) {
            long long a = (rand() % (x - 2)) + 2;
            
            if (gcd(a, x) != 1)
                return false;
            
            if (pows(a, x - 1, x) != 1)
                return false;
        }
        return true;
    }

    BigInt symbolToCode(char el) {
        //el = toUpperCase(el);
        el = toupper(el);

        if (el == 'я') {
            el = 'Я';
        }

        // RUS - 0, EN - 1, Other - 2
        int state = 0;

        if (el >= 65 && el <= 122)
            state = 1;
        else if (el == ' ' || el == ',' || el == '.')
            state = 2;

        if (state == 0 || state == 1) {
            int startIndex = 0;
            int endIndex = state == 0 ? lengthRusAlphabet : lengthEnAlphabet;

            while (true) {
                int middle = (startIndex + endIndex) / 2;

                char middleSymbol = ' ';

                if (state == 0)
                    middleSymbol = rusAlphabet[middle][1];
                else
                    middleSymbol = enAlphabet[middle][1];

                if (el == middleSymbol) {
                    if (state == 0)
                        return rusAlphabet[middle][0];
                    else
                        return enAlphabet[middle][0];
                }

                if (el > middleSymbol)
                    startIndex = middle + 1;
                else
                    endIndex = middle - 1;

                if (startIndex > endIndex)
                    break;
            }
        }
        else {
            for (int i = 0; i < lengthSymbol; i++) {
                if (symbol[i][1] == el)
                    return symbol[i][0];
            }
        }

        return -1;
    }

    char codeToSymbol(BigInt num) {
        int el = (int)num;

        // RUS - 0, EN - 1, Other - 2
        int state = 0;

        if (el >= 42 && el <= 67)
            state = 1;
        else if (el == 68 || el == 69 || el == 70)
            state = 2;

        if (state == 0 || state == 1) {
            int startIndex = 0;
            int endIndex = state == 0 ? lengthRusAlphabet : lengthEnAlphabet;
            endIndex--;

            while (true) {
                int middle = (startIndex + endIndex) / 2;

                int middleCode = -1;

                if (state == 0)
                    middleCode = rusAlphabet[middle][0];
                else
                    middleCode = enAlphabet[middle][0];

                if (el == middleCode) {
                    if (state == 0)
                        return rusAlphabet[middle][1];
                    else
                        return enAlphabet[middle][1];
                }

                if (el > middleCode)
                    startIndex = middle + 1;
                else
                    endIndex = middle - 1;

                if (startIndex > endIndex)
                    break;
            }
        }
        else {
            for (int i = 0; i < lengthSymbol; i++) {
                if (symbol[i][0] == el)
                    return symbol[i][1];
            }
        }

        return '?';
    }

    void ShamirСipher::run(string text, FileHelper helper) {
        List<BigInt> elements = List<BigInt>(text, &symbolToCode);

        long x1 = 0;
        long x2;

        getInversePiar(&x1, &x2, 0, p / 2);
        
        string startData = "x1 = " + to_string(x1) + "; x2 = " + to_string(x2) + "\n";
        
        helper.addToProcessLog(startData);
        cout << startData;

        long y1 = x1;
        long y2;

        do {
            getInversePiar(&y1, &y2, p / 6, p / 4);
        } while (y1 == x1);

        startData = "y1 = " + to_string(y1) + "; y2 = " + to_string(y2) + "\np = " + to_string(p) + "\n\n";

        helper.addToProcessLog(startData);
        cout << startData;

        List<BigInt> first = List<BigInt>(transformation(elements, x1));
        List<BigInt> second = List<BigInt>(transformation(first, y1));
        List<BigInt> thrird = List<BigInt>(transformation(second, x2));
        List<BigInt> result = List<BigInt>(transformation(thrird, y2));

        string startMessage = "Изначальное сообщение:\n" + text + "\n\n";
        string startNumMessage = "Изначальное сообщение в числовой форме:\n" + thrird.toString() + "\n\n";
        string firstTransform = "Первая отправка сообщения:\n" + first.toString() + "\n\n";
        string secondTransform = "Вторая отправка сообщения:\n" + second.toString() + "\n\n";
        string thirdTransform = "Третья отправка сообщения:\n" + thrird.toString() + "\n\n";
        string resutlTransform = "Расшифрованное сообщениe:\n" + result.toSentence(&codeToSymbol) + "\n\n";

        helper.addToProcessLog(startMessage);
        cout << startMessage;

        helper.addToProcessLog(startNumMessage);
        cout << startNumMessage;

        helper.addToProcessLog(firstTransform);
        cout << firstTransform;

        helper.addToProcessLog(secondTransform);
        cout << secondTransform;

        helper.addToProcessLog(thirdTransform);
        cout << thirdTransform;

        helper.addToProcessLog(resutlTransform);
        cout << resutlTransform;
    }

    List<BigInt> ShamirСipher::transformation(const List<BigInt> previous, long power) {
        List<BigInt> transform = previous.getLength();

        for (int i = 0; i < transform.getLength(); i++) {
            transform[i] = pows(previous[i], power, p) % p;
        }

        return transform;
    }

    BigInt ShamirСipher::gcd(BigInt a, BigInt b) {
        if (b == 0)
            return a;
        
        return gcd(b, a % b);
    }

    BigInt ShamirСipher::mul(BigInt a, BigInt b, BigInt m) {
        if (b == 1)
            return a;

        if (b % 2 == 0) {
            BigInt t = mul(a, b / 2, m);
            return (2 * t) % m;
        }
        
        return (mul(a, b - 1, m) + a) % m;
    }

    BigInt ShamirСipher::pows(BigInt a, BigInt b, BigInt m) {
        if (b == 0)
            return 1;

        if (b % 2 == 0) {
            BigInt t = pows(a, b / 2, m);
        
            return mul(t, t, m) % m;
        }
        
        return (mul(pows(a, b - 1, m), a, m)) % m;
    }

    int ShamirСipher::random(int left, int right) {
        auto end = chrono::system_clock::now();
        time_t end_time = chrono::system_clock::to_time_t(end);

        srand(time(&end_time));

        return left + rand() % right;
    }

    ShamirСipher::ShamirСipher(int p) {
        if (!isPrimeNumber(p))
            throw invalid_argument("p должно быть простым числом");

        this->p = p;
    }
}