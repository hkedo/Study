#include "namedPipe.h"
#include <string.h>

int main(void){

    int fdpub, n;
    char line[LINESIZE];

    if(mknod(PUBLIC, S_IFIFO|0666, 0) == -1 ){
        perror("mknod(): ");
        exit(1);
    }

    while(1){

        if((fdpub = open(PUBLIC, O_RDONLY))== -1 ){
            perror(PUBLIC);
            exit(1);
        }

        memset(line, 0x00, LINESIZE);

        while( ( n = read(fdpub, line, LINESIZE)) > 0 )
            write(1, line, n);

        close(fdpub);
    }

    unlink(PUBLIC);

    return 0;
}
