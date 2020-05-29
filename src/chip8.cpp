#include "chip8.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <istream>
#include <cmath>

using namespace std;


int chip8::extract_x(uint16_t opcode){
    return ((opcode & 0x0F00) >> 8);
}
int chip8::extract_y(uint16_t opcode){
    return ((opcode & 0x00F0) >> 4);
}
int chip8::extract_kk(uint16_t opcode){
    return (opcode & 0x00FF);
}

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
    int sum = 0;
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
                    pc +=2;
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
            x = extract_x(opcode);
            //Extract kk
            kk = extract_kk(opcode);
            //Compare and increment PC by 2 if equal
            if(registers[x] == kk){
                pc +=2;
            }
            pc +=2;
            break;
        //0x4XKK: Skip next instruction if Vx != kk
        case 0x4000:
            //Extract x
            x = extract_x(opcode);
            //Extract kk
            kk = extract_kk(opcode);
            //Compare and increment PC by 2 if not equal
            if(registers[x] != kk){
                pc +=4;
            }else{
                pc +=2;
            }
            break;
        //0x5XY0: Skip next instruction if VX = VY
        case 0x5000:
            //Extract x
            x = extract_x(opcode);
            //Extract y
            y = extract_y(opcode);
            if(registers[x] == registers[y]){
                pc +=4;
            }else{
                pc +=2;
            }
            break;
        //0x6XKK: Set VX = KK
        case 0x6000:
            //Extract x
            x = extract_x(opcode);
            //Extract kk
            kk = extract_kk(opcode);

            registers[x] = kk;
            pc +=2;
            break;
        //0x7XKK: Set VX = VX + KK
        case 0x7000:
            //Extract x
            x = extract_x(opcode);
            //Extract kk
            kk = extract_kk(opcode);

            //Add KK to VX
            sum = registers[x] + kk;
            //Set sum as VX
            registers[x] = sum;
            pc +=2;
            break;

        //0x8xyN instructions
        case 0x8000:
            switch(opcode & 0x000F){
                //0x8XY0: Set VX = VY
                case 0:
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    registers[x] = registers[y];
                    pc +=2;
                    break;
                //0x8XY1: Set VX = VX OR VY
                case 1:
                    //Perform a bitwise OR on VX and VY and then store in VX
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    registers[x] = (registers[x] | registers[y]);
                    pc +=2;
                    break;
                //0x8XY2: Set VX = VX AND VY
                case 2:
                    //Perform a bitwise AND on VX and VY and then store in VX
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    registers[x] = (registers[x] & registers[y]);
                    pc +=2;
                    break;
                //0x8XY3: Set VX = VX XOR VY
                case 3:
                    //Perform a bitwise XOR on VX and VY and then store in VX
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    registers[x] = (registers[x] ^ registers[y]);
                    pc +=2;
                    break;    
                //0x8XY4: Set VX = VX + VY, set VF = carry
                case 4:
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    //Find sum
                    sum = registers[x] + registers[y];
                    //Set VF = 1 if sum > 255
                    if(sum > 255){
                        registers[15] = 1;
                    }else{
                        registers[15] = 0;
                    }

                    //Store lowest 8 bits of sum in VX
                    registers[x] = (sum & 0xFF);
                    pc +=2;
                    break;
                //0x8XY5: Set Vx = Vx - Vy, set VF = NOT borrow
                case 5:
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    if(registers[x] > registers[y]){
                        registers[15] = 1;
                    }else{
                        registers[15] = 0;
                    }

                    registers[x] = (registers[x] - registers[y]);
                    pc +=2;
                    break;
                //0x8XY6: Set Vx = Vx SHR 1.
                case 6:
                    x = extract_x(opcode);
                    y = extract_y(opcode);
                    
                    //Check if LSB is 1
                    if(registers[x] & 1){
                        registers[15] = 1;
                    }else{
                        registers[15] = 0;
                    }

                    //Divide Vx by 2
                    registers[x] /= 2;
                    pc +=2;
                    break;
                //0x8XY7: Set Vx = Vy - Vx, set VF = NOT borrow.
                case 7:
                    x = extract_x(opcode);
                    y = extract_y(opcode);

                    if(registers[y] > registers[x]){
                        registers[15] = 1;
                    }else{
                        registers[15] = 0;
                    }
                    //Subtrack Vx from Vy and store in Vx
                    registers[x] = (registers[y] - registers[x]);
                    pc +=2;
                    break;
                //0x8XYE: Set Vx = Vx SHL 1.
                case 0xE:
                    x = extract_x(opcode);
                    //Find MSB of Vx
                    int MSB = (int)(log2(registers[x]));
                    //Compare
                    if(MSB == 1){
                        registers[15] = 1;
                    }else{
                        registers[15] = 0;
                    }
                    //Multiply Vx by 2
                    registers[x] *= 2;
                    pc +=2;
                    break;
            }
            break;
        //0x9xy0: Skip next instruction if Vx != Vy.
        case 0x9000:
            x = extract_x(opcode);
            y = extract_y(opcode);

            if(registers[x] != registers[y]){
                pc +=4;
            }
            pc +=2;
            break;

        //0xANNN: Set index register to NNN
        case 0xA000:
            index_reg = opcode & 0x0FFF;
            pc +=2;
            break;
        //0xBNNN: Jump to location NNN + V0    
        case 0xB000:
            pc = (opcode & 0x0FFF) + registers[0];
            break;
        //0xCxkk: Set Vx = random byte AND kk.
        case 0xC000:
            x = extract_x(opcode);
            kk = extract_kk(opcode);

            int rand_num = (rand() % 0xFF);
            //AND with kk and store in Vx
            registers[x] = (rand_num & kk);
            pc+=2;
            break;
        //0xDxyn:
        case 0xD000:
            unsigned short x = extract_x(opcode);
            unsigned short y = extract_y(opcode);
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;
            
            registers[0xF] = 0;
            for (int yline = 0; yline < height; yline++)
            {
                pixel = memory[index_reg + yline];
                for(int xline = 0; xline < 8; xline++)
                {
                if((pixel & (0x80 >> xline)) != 0)
                {
                    if(display[(x + xline + ((y + yline) * 64))] == 1)
                    registers[0xF] = 1;                                 
                    display[x + xline + ((y + yline) * 64)] ^= 1;
                }
                }
            }
            
            draw_flag = true;
            pc += 2;
            break;

        case 0xE000:
            switch(opcode & 0x00FF){
                //0xEx9e: Skip next instruction if key with the value of Vx is pressed
                case 0x009E:
                    x = extract_x(opcode);
                    if(key[registers[x] != 0]){
                        pc +=4;
                    }else{
                        pc +=2;
                    }
                    break;
                //0xExA1: Skip next instruction if key with the value of Vx is not pressed.
                    x = extract_x(opcode);
                    if(key[registers[x] == 0]){
                        pc +=4;
                    }else{
                        pc +=2;
                    }
                    break;
            }
            break;
            case 0xF000:
                switch(opcode & 0x00FF){
                    //0xFx07: Set Vx = delay timer value.
                    case 0x0007:
                        x = extract_x(opcode);
                        registers[x] = delay_timer;
                        pc +=2;
                        break;

                }


        
        
        default:
            printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
    }

    //TODO:Execute opcode
    //TODO: Update timers
}


    




