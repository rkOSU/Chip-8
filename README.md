"Technically the CHIP-8 was never a real machine at all but instead a virtual machine. People would write programs for this virtual machine, and then an interpreter would decode the instructions. 
Because it was virtual, the same programs could run on different machines as long as they had an interpreter." - Austin Morlan

I want to create one of interpreters/emulators. This should help me develop my C++ and low level computing knowledge.
I am not too knowledgeable about SHELL and SDL, so I took the build commands and code for setting up graphics from https://github.com/JamesGriffin/CHIP-8-Emulator

Install SDL
```
$ sudo apt-get install cmake libsdl2-dev
```
Compile
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```
Run
```
./chip8 <ROM file>
```
