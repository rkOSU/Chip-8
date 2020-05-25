#include "chip8.h"
#include <fstream>


//Initialize System
void chip8::initialize(){
    pc = 0x200; //The program counter starts at 0x200 in Chip-8 memory
    opcode = 0; //Reset current opcode
    index_reg = 0; //Reset index register
    sp = 0; //Reset stack pointer

    //TODO: Clear display
    //TODO: Clear stack
    //TODO: Clear registers V0-VF
    //TODO: Clear memory

    //Load fontset
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

    }

    //TODO Load ROM data in Chip8 memory

    
}
