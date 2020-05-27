#include "chip8.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <istream>
using namespace std;


//Initialize System
void chip8::initialize(){
    pc = 0x200; //The program counter starts at 0x200 in Chip-8 memory
    opcode = 0; //Reset current opcode
    index_reg = 0; //Reset index register
    sp = 0; //Reset stack pointer

    //Clear display
    memset(display, 0, sizeof(display));
    //Clear stack
    memset(stack, 0, sizeof(stack));
    //Clear registers V0-VF
    memset(registers, 0, sizeof(registers));
    //Clear memory
    memset(memory, 0, sizeof(memory));

    //Load fontset (0x50 = 80)
    for(int i = 0; i < 80; ++i){
        memory[i] = font_set[i];
    }
}

//Load ROM into Memory
void chip8::load_ROM(char const* filename){
    /*  Open a file as a stream of binary.
        Position file pointer at the end of the stream.

        More information on the two refrences being used:
        http://www.cplusplus.com/reference/ios/ios_base/openmode/
    */
    std::ifstream file(filename, std::ios_base::binary | std::ios_base::ate){
        //File pointer is at end of steam
        //so tellg will return the size of the file
        std::streampos size = file.tellg();
        
        //Create a buffer
        char* buffer = new char[size];
        //Fill buffer
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        
        //Close file
        file.close();
    
        //Load ROM data into memory
        unsigned int start_pos = 0x200;
        for(int i = 0; i < size; i++){
            memory[start_pos] = buffer[i]; 
        } 
    }
}


void chip8::emulate(){
    
    //Fetch opcode
    /*  An opcode is 16 bits or 2 bytes so shift
        8 bits to the left and combine with next byte to make
        full 2 byte opcode.
    */
    opcode = memory[pc] << 8 | memory[pc+1];

    //TODO: Decode opcode
    switch(opcode & 0xF000){
        //0x0NNN: Jump to a machine code routine at NNN
        case 0x0000: 
            //NONE
            break;
        //0x1NNN: Jump to address NNN
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        //0x2NNN: Call subroutine at NNN
        case 0x2000:
            ++sp;
            stack[sp] = pc;
            pc = opcode & 0x0FFF;
            break;
        //0xANNN: Set index register to NNN
        case 0xA000:
            index_reg = opcode & 0x0FFF;
            break;
        //0xBNNN: Jump to location NNN + V0    
        case 0xB000:
            pc = (opcode & 0x0FFF) + registers[0];
            break;;
        
        default:
    }

    //TODO:Execute opcode
    //TODO: Update timers
}


    




