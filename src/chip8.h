#include <cstdint>

class chip8
{
    public:
        void load_ROM(char const* filename);
        void initialize();
        void emulate();

    private:
        //Array for 16 8-bit registers
        uint8_t registers[16];
        //Array for 4096 bytes of memory
        uint8_t memory[4096];
        //16-bit Index Register
        uint16_t index_reg;
        //16-bit Program Counter
        uint16_t pc;

        //Stack to hold 16 different program counters
        uint16_t stack[16];
        //8-bit Stack Pointer
        uint8_t sp;

        //8-bit Delay Timer
        uint8_t delay_timer;
        //8-bit Sound Timer
        uint8_t sound_timer;

        //Key Inputs
        uint8_t key[16];

        //Display Memory for 64*32 number of pixels
        uint32_t display[64*32];

        //16-bit Opcode
        uint16_t opcode;

        //Font Set (16 characters at 5 bytes each)
        uint8_t font_set[16*5] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F  
        };

};    


    
