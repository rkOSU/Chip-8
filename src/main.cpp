#include "chip8.cpp"
#include <fstream>
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
    cout << "Hello Chip";
    chip8 chip;
    chip.initialize();
}