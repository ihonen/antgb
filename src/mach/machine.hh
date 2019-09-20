#ifndef GAMEBOY_HH
#define GAMEBOY_HH

class GBMachine
{
public:
    GBMachine();
    class CPU;
    class MMU;
    class Peripheral;
    class LCDControl;
private:
    CPU* cpu;
    MMU* mmu;
    LCDControl* lcd;
};

#endif // GAMEBOY_HH
