/**
 * @author: Ivan Lin
 * @date: 9/15/2024
 */



#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    //not enough args
    if(argc != 2){
        fprintf(2, "Usage: sleep <ticks>\n");
        exit(1);
    }
    int ticks = atoi(argv[1]);
    if (ticks < 0) //set tick to 0 if they input a negative
    {
        ticks = 0;
    }
    sleep(ticks);
    exit(0);
}
