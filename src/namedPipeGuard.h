#ifndef NAMEDPIPEGUARD
#define NAMEDPIPEGUARD

#include <windows.h>
#include <iostream>

class NamedPipeGuard
{
    private:
        HANDLE pipe;
    public:
        NamedPipeGuard(HANDLE h);
        ~NamedPipeGuard();
        HANDLE get() const;
};

#endif