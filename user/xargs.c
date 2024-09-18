/**
 * @author: Ivan Lin
 * @date: 9/17/2024
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char *argv[]) 
{
    //insufficient arguents to ue xargs
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs <command>\n");
        exit(1);
    }

    char buf[512];
    char *cmd[MAXARG];
    int i;

    //copy command-line arguments into cmd array
    for (i = 0; i < argc - 1 && i < MAXARG - 1; i++) {
        cmd[i] = argv[i + 1];
    }

    //read from stdinput
    while (1) {
        int n = 0;
        while (n < sizeof(buf) - 1) {
            char c;
            if (read(0, &c, 1) != 1) 
            {
                break; //done reading
            }
            if (c == '\n') 
            {
                break; //reach the end
            }
            buf[n++] = c;
        }
        buf[n] = '\0';

        if (n == 0) 
        {
            break; //no more input
        }

        //add input line as additional argument
        cmd[i] = buf;
        cmd[i + 1] = 0;

        if (fork() == 0) 
        {
            exec(cmd[0], cmd);
            fprintf(2, "exec failed\n");
            exit(1);
        } 
        else 
        {
            wait(0);
        }
    }

    exit(0);
}
