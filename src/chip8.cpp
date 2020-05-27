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
    std::ifstream file(filename, std::ios_base::binary | std::ios_base::ate);
    
    if(file.is_open()){
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
    int x = 0;
    int y = 0;
    int kk = 0;
    //Fetch opcode
    /*  An opcode is 16 bits or 2 bytes so shift
        8 bits to the left and combine with next byte to make
        full 2 byte opcode.
    */
    opcode = ((memory[pc] << 8) | memory[pc+1]);

    //TODO: Decode opcode TODO: MAKE FUNCTIONS
    switch(opcode & 0xF000){
        
        //0x0NNN: 
        case 0x0000: 
            switch(opcode & 0x000F){
                //0x00E0 Clear the display
                case 0x0000:
                    memset(display, 0, sizeof(display));
                    break;
                //0x00EE: Return from a subroutine
                case 0x000E:
                    pc =  stack[sp];
                    sp--;
                    break;
                default:
                    printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
            }
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
        //0x3XKK: Skip next instructin if Vx == kk    
        case 0x3000:
            //Extract x
            x = ((opcode & 0x0F00) >> 8);
            //Extract kk
            kk = (opcode & 0x00FF);
            //Compare and increment PC by 2 if equal
            if(registers[x] == kk){
                pc +=2;
            }
            break;
        //0x4XKK: Skip next instruction if Vx != kk
        case 0x4000:
            //Extract x
            x = ((opcode & 0x0F00) >> 8);
            //Extract kk
            kk = (opcode & 0x00FF);
            //Compare and increment PC by 2 if not equal
            if(registers[x] != kk){
                pc +=2;
            }
            break;
        //0x5XY0: Skip next instruction if VX = VY
        case 0x5000:
            //Extract x
            x = ((opcode & 0x0F00) >> 8);
            //Extract y
            y = ((opcode & 0x00F0) >> 4);
            if(registers[x] == registers[y]){
                pc +=2;
            }
            break;

        //0xANNN: Set index register to NNN
        case 0xA000:
            index_reg = opcode & 0x0FFF;
            break;
        //0xBNNN: Jump to location NNN + V0    
        case 0xB000:
            pc = (opcode & 0x0FFF) + registers[0];
            break;
        
    
        

        
        
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
    }

    //TODO:Execute opcode
    //TODO: Update timers
}


    




