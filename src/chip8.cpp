#include "chip8.h"
#include <fstream>
#include <string.h>
#include <algorithm>
#include <istream>


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
    }

    //TODO Load ROM data in Chip8 memory
   
    
}
