#ifndef GAMEBOY_HH
#define GAMEBOY_HH

class GBMachine
{
public:
    GBMachine();
    class CPU;
private:
    CPU* cpu;
};

#endif // GAMEBOY_HH
