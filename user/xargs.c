// xargs.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char* readline() {
	char* buff = malloc(100);
	char* p = buff;
	while (read(0, p, 1) != 0){
		if(*p == '\n' || *p == '\0'){
			*p = '\0';
			return buff;
		}
		p++;
	}
	if (p != buff){
		return buff;
	}
	free(buff);
	return 0;	
}

int main(int argc, char *argv[]){
	if(argc < 2) {
        printf("Usage: xargs [command]\n");
        exit(-1);
    }
	argv++;
	char* nargv[100];
	char** pnargv = nargv;
	char** pargv = argv;

	while (*pargv != 0){
		*pnargv = *pargv;
		pargv++;
		pnargv++;
	}
	// printf("40%s", argv[0]);
	char* line;
	while((line = readline()) != 0){
		char* p = line;
		// printf("42%s", line);
        char* buff = malloc(36);
        char* pbuff = buff;
        int nargc = argc - 1;		
		while(*p != 0){
			if(*p == ' ' && buff != pbuff){
				*pbuff = 0;
				nargv[nargc] = buff;
				buff = malloc(36);
                pbuff = buff;
                nargc++;
			} else {
				*pbuff = *p;
				pbuff++;
			}
			p++;
		}
        if(buff != pbuff){
            nargv[nargc] = buff;
            nargc++;
        }	
		nargv[nargc] = 0;	
		free(line);
        int pid = fork();
		// printf("66%s %s", nargv[0], nargv[1]);
        if(pid == 0){
            exec(nargv[0], nargv);
        }else{
            wait(0);
        }		
	}
	exit(0);
}