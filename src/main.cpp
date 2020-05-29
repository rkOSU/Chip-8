#include "chip8.cpp"
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

int main(int argc, char **argv) {
    cout << "Hello Chip";
    chip8 chip;
    chip.initialize();
     
    int k = (int)(log2(364));
    cout << k;

}