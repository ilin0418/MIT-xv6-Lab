/**
 * @author: Ivan Lin
 * @date: 9/17/2024
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


void fmtname(char *path, char *name) 
{
    char *p;
    //find first character after the last slash
    for(p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
    strcpy(name, p);
}


void find(char *path, char *target) 
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0)//attempt to open the path
    { 
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0)//attempt to look at the status info for the directories
    { 
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type)
    {
        case T_FILE: //case for regular files
            {
                char name[DIRSIZ+1];
                fmtname(path, name);
                if(strcmp(name, target) == 0) //if the file match our target, we print the path
                {
                    printf("%s\n", path);
                }
            }
            break;

        case T_DIR: //case for directories
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof(de)) == sizeof(de))
            { 
                if(de.inum == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) 
                {
                    find(buf, target);
                }
            }
            break;
    }
    close(fd);
}


int main(int argc, char *argv[]) 
{
    //user inputs the wrong params
    if(argc < 3){
        fprintf(2, "Usage: find <directory> <name>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
