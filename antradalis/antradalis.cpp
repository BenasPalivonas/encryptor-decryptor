#include <iostream>
#include <fstream>
#include <iomanip>
#include "rlutil.h";
using namespace std;
int Rx(unsigned char* prog_mem, int i) {
    return prog_mem[i + 1] & 0x0f;
}
int Ry(unsigned char* prog_mem, int i) {
    return prog_mem[i + 1] >> 4;
}
void i2() {
}
bool aryraNulis(unsigned char* regs) {
    int arraySize = sizeof(regs) / sizeof(regs[0]);
    for (int i = 0; i < arraySize; i++) {
        if ((int)regs[i] == 0) {
            return true;
        }
    }
    return false;
}
void i2(int& i) {
    i += 2;
}
void raudona() {
    rlutil::setColor(rlutil::RED);
}
void zalia() {
    rlutil::setColor(rlutil::GREEN);
}
void geltona() {
    rlutil::setColor(rlutil::YELLOW);
}
void melyna() {
    rlutil::setColor(rlutil::LIGHTBLUE);
}
void ScreenPosition(int x, int y)
{
    COORD cursorPosition;
    cursorPosition.X = x;
    cursorPosition.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}
void uzduotiesAtlikimas(string decryptor, string textas, string rezas) {
    unsigned char regs[16];
    unsigned char prog_mem[256];
    ifstream bin(decryptor, ios::binary);
    ifstream text(textas);
    ofstream rez(rezas);
    int dydis = -1;
    while (!bin.eof()) {
        dydis++;
        bin >> noskipws >> prog_mem[dydis];
        //cout << setbase(16) << (int)prog_mem[dydis];
        //cout << setbase(10)<<" "<<dydis << endl;
    }

    bin.close();
    int i = 0;
    bool eofFlag = false;
    //cout << setbase(10) << dydis << endl;
    bool end = false;
    while (!end) {
        switch (prog_mem[i]) {
        case 0x01: // INC
            regs[Rx(prog_mem, i)]++;
            i2(i);
            break;

        case 0x02: // DEC
            regs[Rx(prog_mem, i)]--;
            i2(i);
            break;

        case 0x03: // MOV   
            regs[Rx(prog_mem, i)] = regs[Ry(prog_mem, i)];
            i2(i);
            break;

        case 0x04: // MOVC
            regs[0] = (int)prog_mem[i + 1];
            i2(i);
            break;

        case 0x05: // LSL
            regs[Rx(prog_mem, i)] = regs[Rx(prog_mem, i)] << 1;
            i2(i);
            break;

        case 0x06: // LSR
            regs[Rx(prog_mem, i)] = regs[Rx(prog_mem, i)] >> 1;
            i2(i);
            break;

        case 0x07: // JMP
            i += (char)prog_mem[i + 1];
            while (i >= dydis) {
                i -= dydis;
            }
            break;

        case 0x08: // JZ
            if (aryraNulis(regs)) {
                i += (char)prog_mem[i + 1];
                while (i >= dydis) {
                    i -= dydis;
                }
            }
            else i2(i);
            break;
        case 0x09: // JNZ
            if (!aryraNulis(regs)) {
                i += (char)prog_mem[i + 1];
                while (i >= dydis) {
                    i -= dydis;
                }
            }
            else i2(i);
            break;

        case 0x0a: // JFE
            if (eofFlag == true) {
                i += (char)prog_mem[i + 1];
                while (i >= dydis) {
                    i -= dydis;
                }
            }
            else {
                i2(i);
            }

            break;

        case 0x0b: // RET
            end = true;
            break;

        case 0x0c: // ADD
            regs[Rx(prog_mem, i)] = regs[Rx(prog_mem, i)] + regs[Ry(prog_mem, i)];
            i2(i);
            break;

        case 0x0d: // SUB
            regs[Rx(prog_mem, i)] = regs[Rx(prog_mem, i)] - regs[Ry(prog_mem, i)];
            i2(i);
            break;

        case 0x0e: // XOR
            regs[Rx(prog_mem, i)] = regs[Rx(prog_mem, i)] ^ regs[Ry(prog_mem, i)];
            i2(i);
            break;

        case 0x0f: // OR
            regs[Rx(prog_mem, i)] = regs[Rx(prog_mem, i)] | regs[Ry(prog_mem, i)];
            i2(i);
            break;

        case 0x10: // IN
            unsigned char charas;
            text >> noskipws >> charas;
            if (text.eof()) {
                eofFlag = true;
            }
            regs[Rx(prog_mem, i)] = (int)charas;
            i2(i);
            break;

        case 0x11: // OUT

            rez << regs[Rx(prog_mem, i)];
            i2(i);
            break;
        default:
            cout << "error" << endl;
            getch();
        }
    }
    system("cls");
}
void gryzimoTekstas() {
    ScreenPosition(2, 8);
    melyna();
    cout << "Iveskite q, jei norite sugryzti";
    ScreenPosition(0, 0);
    geltona();
}
int main()
{
    int meniuPasirinkimas = 0;
    char mygtukas;
    string dec;
    string text;
    string rez;
    string meniuTekstas[4] = { "1. atkoduoti pirma uzduoti" , "2. uzkoduoti savo teksta" ,
    "3. atkoduoti savo teksta" ,"4. baigti darba" };
    while (1) {
        melyna();
        ScreenPosition(0, 0);
        cout << "instrukcija:" << endl;
        cout << "vaiksioti po meniu su s ir w" << endl;
        cout << "pasirinkus spausti e";
        for (int i = 0; i < 4; i++) {
            ScreenPosition(4, i + 5);
            if (meniuPasirinkimas == i)raudona();
            else zalia();
            cout << meniuTekstas[i];
        }
        mygtukas=getch();
        if (mygtukas == 'w' & meniuPasirinkimas != 0) {
            meniuPasirinkimas--;
        }
        else if (mygtukas == 's' & meniuPasirinkimas != 3) {
            meniuPasirinkimas++;
        }
        else if (mygtukas == 'e') {

            system("cls");
            geltona();
            switch (meniuPasirinkimas) {
            case 0:
                gryzimoTekstas();
                cout << "iveskite rezultato failo pavadinima: " << endl;;
                cin >> rez;
                if (rez == "q") {
                    system("cls");
                    break;
                }
                uzduotiesAtlikimas("decryptor.bin", "q1_encr.txt", rez);
                break;
            case 1:
                gryzimoTekstas();
                cout << "iveskite teksto failo pavadinima,kuri norite uzkoduoti: " << endl;
                cin >> text;
                if (text == "q") {
                    system("cls");
                    break;
                }
                cout << endl << "iveskite rezultato failo pavadinima: " << endl;
                cin >> rez;
                if (rez == "q") {
                    system("cls");
                    break;
                }
                uzduotiesAtlikimas("encryptorbbz.bin", text, rez);
                break;
            case 2:
                gryzimoTekstas();
                cout << "iveskite uzkoduoto teksto failo pavadinima, kuri norite atkoduoti: " << endl;
                cin >> text;
                if (text == "q") {
                    system("cls");
                    break;
                }
                cout << endl << "iveskite rezultato failo pavadinima: " << endl;
                cin >> rez;
                if (rez == "q") {
                    system("cls");
                    break;
                }
                uzduotiesAtlikimas("decryptorbbz.bin", text, rez);
                break;
            case 3:
                return 0;
                break;
            default:
                cout << "error" << endl;
                break;
            }
        }
    }
       
}

