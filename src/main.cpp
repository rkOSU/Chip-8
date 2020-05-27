#include "chip8.cpp"
#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
    cout << "Hello Chip";
    chip8 chip;
    chip.initialize();
    cout << ((0xAF34 & 0x00F0) >> 4) ;
}