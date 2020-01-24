#include "machine.h"
#include <windows.h>

int main()
{
    // Create the machine
    Machine machine;

    // Run until the running flag is set to false
    while (machine.GetRunning())
        machine.GoNext();

    return 0;
}
