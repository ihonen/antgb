#ifndef GAMEBOY_HH
#define GAMEBOY_HH

class GBMachine
{
public:
    GBMachine();
    class CPU;
    class MMU;
    class Peripheral;
private:
    CPU* cpu;
    MMU* mmu;
};

#endif // GAMEBOY_HH
