#pragma once

class iBit
{
public:
    virtual ~iBit() = 0;
    virtual bool test() = 0;
    virtual void set() = 0;
    virtual void clear() = 0;
    virtual void update(bool condition) = 0;
};
