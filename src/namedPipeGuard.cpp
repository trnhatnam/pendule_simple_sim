#include "namedPipeGuard.h"



NamedPipeGuard::NamedPipeGuard(HANDLE h):pipe(h){}

NamedPipeGuard::~NamedPipeGuard()
{
    if (pipe != INVALID_HANDLE_VALUE) 
    {
        DisconnectNamedPipe(pipe);
        CloseHandle(pipe);
    }
}
HANDLE NamedPipeGuard::get() const 
{
    return pipe;
}