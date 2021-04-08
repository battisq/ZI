#include "AES.h"
#include "AESConfig.h"
#include <string>


namespace algorithms {

    TMatrix<byte> AES::addRoundKey(TMatrix<byte> state, TMatrix<byte> w, int round) {

        TMatrix<byte> tmp = TMatrix<byte>(4);

        for (int c = 0; c < Nb; c++) {
            for (int l = 0; l < 4; l++) {
                tmp[l][c] = (state[l][c] ^ w[round * Nb + c][l]);
            }
        }

        return tmp;
    }

    TMatrix<byte> AES::invMixColumns(TMatrix<byte> s) {

        int sp[4];
        byte b02 = (byte)0x0e, b03 = (byte)0x0b, b04 = (byte)0x0d, b05 = (byte)0x09;

        for (int c = 0; c < 4; c++) {
            sp[0] = ffMul(b02, s[0][c]) ^ ffMul(b03, s[1][c]) ^ ffMul(b04, s[2][c]) ^ ffMul(b05, s[3][c]);
            sp[1] = ffMul(b05, s[0][c]) ^ ffMul(b02, s[1][c]) ^ ffMul(b03, s[2][c]) ^ ffMul(b04, s[3][c]);
            sp[2] = ffMul(b04, s[0][c]) ^ ffMul(b05, s[1][c]) ^ ffMul(b02, s[2][c]) ^ ffMul(b03, s[3][c]);
            sp[3] = ffMul(b03, s[0][c]) ^ ffMul(b04, s[1][c]) ^ ffMul(b05, s[2][c]) ^ ffMul(b02, s[3][c]);

            for (int i = 0; i < 4; i++) {
                s[i][c] = (byte)(sp[i]);
            }
        }

        return s;
    }

    TMatrix<byte> AES::invShiftRows(TMatrix<byte> state) {
        byte t[4];

        for (int r = 1; r < 4; r++) {
            for (int c = 0; c < Nb; c++) {
                t[(c + r) % Nb] = state[r][c];
            }

            for (int c = 0; c < Nb; c++) {
                state[r][c] = t[c];
            }
        }

        return state;
    }

    TMatrix<byte> AES::invSubBytes(TMatrix<byte> state) {
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < Nb; col++) {
                state[row][col] = (byte)(invSbox[(state[row][col] & 0x000000ff)] & 0xff);
            }
        }

        return state;
    }

    TMatrix<byte> AES::mixColumns(TMatrix<byte> s) {
        int sp[4];

        byte b02 = (byte)0x02, b03 = (byte)0x03;

        for (int c = 0; c < 4; c++) {
            sp[0] = ffMul(b02, s[0][c]) ^ ffMul(b03, s[1][c]) ^ s[2][c] ^ s[3][c];
            sp[1] = s[0][c] ^ ffMul(b02, s[1][c]) ^ ffMul(b03, s[2][c]) ^ s[3][c];
            sp[2] = s[0][c] ^ s[1][c] ^ ffMul(b02, s[2][c]) ^ ffMul(b03, s[3][c]);
            sp[3] = ffMul(b03, s[0][c]) ^ s[1][c] ^ s[2][c] ^ ffMul(b02, s[3][c]);

            for (int i = 0; i < 4; i++) {
                s[i][c] = (byte)(sp[i]);
            }
        }

        return s;
    }

    ByteArray AES::rotWord(ByteArray input) {
        ByteArray tmp = ByteArray(input.getLength());
        tmp[0] = input[1];
        tmp[1] = input[2];
        tmp[2] = input[3];
        tmp[3] = input[0];

        return tmp;
    }
    
    TMatrix<byte> AES::shiftRows(TMatrix<byte> state) {
        byte t[4];
        
        for (int r = 1; r < 4; r++) {
            for (int c = 0; c < Nb; c++) {
                t[c] = state[r][(c + r) % Nb];
            }
            
            for (int c = 0; c < Nb; c++) {
                state[r][c] = t[c];
            }
        }

        return state;
    }
    
    TMatrix<byte> AES::subBytes(TMatrix<byte> state) {
        TMatrix<byte> tmp = TMatrix<byte>(4);
        
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < Nb; col++) {
                tmp[row][col] = (byte)(sbox[(state[row][col] & 0x000000ff)] & 0xff);
            }
        }

        return tmp;
    }
    
    ByteArray AES::subWord(ByteArray inp) {
        ByteArray tmp = inp.getLength();

        for (int i = 0; i < tmp.getLength(); i++) {
            tmp[i] = (byte)(sbox[inp[i] & 0x000000ff] & 0xff);
        }

        return tmp;
    }

    // TODO
    byte AES::ffMul(byte a, byte b) {
        byte aa = a, bb = b, r = 0, t;

        while (aa != 0) {
            if ((aa & 1) != 0) {
                r = (byte)(r ^ bb);
            }

            t = (byte)(bb & 0x80);
            bb = (byte)(bb << 1);

            if (t != 0) {
                bb = (byte)(bb ^ 0x1b);
            }

            aa = (byte)((aa & 0xff) >> 1);
        }

        return r;
    }

    // TODO: Переименовать
    TMatrix<byte> AES::generateSubkeys(ByteArray key) {
        TMatrix<byte> tmp = TMatrix<byte>(Nb * (Nr + 1), 4);

        for (int i = 0; i < Nk; i++) {
            tmp[i][0] = key[i * 4];
            tmp[i][1] = key[i * 4 + 1];
            tmp[i][2] = key[i * 4 + 2];
            tmp[i][3] = key[i * 4 + 3];
        }
       
        for (int i = Nk; i < Nb * (Nr + 1); i++) {
            ByteArray temp = 4;
            
            for (int k = 0; k < 4; k++) {
                temp[k] = tmp[i - 1][k];
            }
            
            if (i % Nk == 0) {
                temp = subWord(rotWord(temp));
                // TODO: Индексирование
                temp[0] = (byte)(temp[0] ^ (rcon[i / Nk] & 0xff));
            } else if (Nk > 6 && i % Nk == 4) {
                temp = subWord(temp);
            }
            
            ByteArray tmp2 = ByteArray(
                4,
                tmp[i - Nk][0], 
                tmp[i - Nk][1], 
                tmp[i - Nk][2], 
                tmp[i - Nk][3]
            );

            ByteArray result = 4;
            result = xorFunction(tmp2, temp);
            // tmp[i] = xorFunction(tmp2, temp);
            tmp[i][0] = result[0];
            tmp[i][1] = result[1];
            tmp[i][2] = result[2];
            tmp[i][3] = result[3];
            i++;
        }

        return tmp;
    }

    ByteArray AES::xorFunction(ByteArray a, ByteArray b) {
        ByteArray outp = a.getLength();
        
        for (int i = 0; i < a.getLength(); i++) {
            outp[i] = (byte)(a[i] ^ b[i]);
        }
        
        return outp;
    }

    ByteArray AES::encryptBloc(ByteArray inp) {
        ByteArray tmp = inp.getLength();

        TMatrix<byte> state = TMatrix<byte>(4, Nb);

        for (int i = 0; i < inp.getLength(); i++) {
            state[i / 4][i % 4] = inp[i % 4 * 4 + i / 4];
        }

        state = addRoundKey(state, w, 0);
        
        for (int round = 1; round < Nr; round++) {
            state = subBytes(state);
            state = shiftRows(state);
            state = mixColumns(state);
            state = addRoundKey(state, w, round);
        }
        
        state = subBytes(state);
        state = shiftRows(state);
        state = addRoundKey(state, w, Nr);

        for (int i = 0; i < tmp.getLength(); i++) {
            tmp[i % 4 * 4 + i / 4] = state[i / 4][i % 4];
        }

        return tmp;
    }

    ByteArray AES::decryptBloc(ByteArray inp) {
        ByteArray tmp = inp.getLength();

        TMatrix<byte> state = TMatrix<byte>(4, Nb);

        for (int i = 0; i < inp.getLength(); i++) {
            state[i / 4][i % 4] = inp[i % 4 * 4 + i / 4];
        }

        state = addRoundKey(state, w, Nr);
        
        for (int round = Nr - 1; round >= 1; round--) {
            state = invSubBytes(state);
            state = invShiftRows(state);
            state = addRoundKey(state, w, round);
            state = invMixColumns(state);

        }
        
        state = invSubBytes(state);
        state = invShiftRows(state);
        state = addRoundKey(state, w, 0);

        for (int i = 0; i < tmp.getLength(); i++) {
            tmp[i % 4 * 4 + i / 4] = state[i / 4][i % 4];
        }

        return tmp;
    }

    AES::AES() {}

    byte fo(int i) {
        if (i == 0)
            return (byte)0x80;

        return 0;
    }

    ByteArray AES::encrypt(string text, string inputKey) {
        ByteArray inp = ByteArray(text);
        ByteArray key = ByteArray(inputKey);

        Nb = 4;
        Nk = key.getLength() / 4;
        Nr = Nk + 6;

        int length = 16 - inp.getLength() % 16;
        ByteArray padding = ByteArray(length, &fo);

        ByteArray tmp = inp.getLength() + length;
        ByteArray bloc = 16;

        w = generateSubkeys(key);

        int count = 0;
        int i;

        for (i = 0; i < inp.getLength() + length; i++) {
            if (i > 0 && i % 16 == 0) {
                bloc = encryptBloc(bloc);
                bloc.copyTo(0, &tmp, i - 16, bloc.getLength());
            }

            if (i < inp.getLength()) {
                bloc[i % 16] = inp[i];
            }
            else {
                bloc[i % 16] = padding[count % 16];
                count++;
            }
        }

        if (bloc.getLength() == 16) {
            bloc = encryptBloc(bloc);
            bloc.copyTo(0, &tmp, i - 16, bloc.getLength());
        }

        return tmp;
    }
    
    string AES::decrypt(ByteArray inp, string inputKey) {
        int i;
        ByteArray tmp = inp.getLength();
        ByteArray bloc = 16;
        ByteArray key = ByteArray(inputKey);

        Nb = 4;
        Nk = key.getLength() / 4;
        Nr = Nk + 6;
        w = generateSubkeys(key);


        for (i = 0; i < inp.getLength(); i++) {
            if (i > 0 && i % 16 == 0) {
                bloc = decryptBloc(bloc);
                bloc.copyTo(0, &tmp, i - 16, bloc.getLength());
            }
            
            if (i < inp.getLength()) {
                bloc[i % 16] = inp[i];
            }
        }
        
        bloc = decryptBloc(bloc);
        bloc.copyTo(0, &tmp, i - 16, bloc.getLength());

        return tmp.trim().toString();
    }
}