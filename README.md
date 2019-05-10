# CPPGB


a gb emulator written in cpp
# CPPGB


a gb emulator written in cpp

###todo list 
cpp.h :
 1. swap function

cpu.cpp: 
1. daa 0x27
2. halt 0x76
3. stop 1000
4. EI/DI 0xf4
5. rst
6. ret

###problem:

Rom.h:
1. line 19

    `
    Rom(char* c, Word romLength): bytes(c, c + romLength) { };
    `

    > too many initializers for 'std::array<unsigned char, 256ull>'
    > array must be initialized with a brace-enclosed initializer

2. line 45 
    `
    void setByte(Word address, Byte value) override {
        warning("write to rom");
    `
    > ISO C++ forbids converting a string constant to 'char*' 
      In instantiation of 'void Rom<offset, length>::setByte(Word, Byte) [with short unsigned int offset = 0u; short unsigned int length = 32768u; Word = short unsigned int; Byte = unsigned char]':
    >[maybe we should use string instead of char *]