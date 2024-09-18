/**
 * @author: Ivan Lin
 * @date: 9/15/2024
 */


#include "kernel/types.h"
#include "user/user.h"

int main() {
    int p1[2];
    int p2[2];
    char buf[1];
    pipe(p1); //pipe for parent to child message
    pipe(p2); //pipe for child to parent message

    if (fork() == 0) { //if it's the child process
        read(p1[0], buf, 1);  //read message from the parent
        printf("%d: received ping\n", getpid());
        write(p2[1], buf, 1); //send message back to the parent
        exit(0);
    } 
    else 
    {
        // Parent process
        write(p1[1], "x", 1);  //send message to child
        read(p2[0], buf, 1);   //read the message sent from the child
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}
