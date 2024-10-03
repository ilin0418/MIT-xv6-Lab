#include "user.h"
#include "sysinfo.h"

int main(void) 
{
    struct sysinfo info;

    printf("sysinfotest: start\n");

    if (sysinfo(&info) < 0) 
    {
        printf("FAIL: sysinfo failed\n");
        exit(1);
    }

    printf("Free memory: %lu bytes\n", info.freemem);
    printf("Number of processes: %d\n", info.nproc);
    printf("sysinfotest: OK\n");

    exit(0); 
}