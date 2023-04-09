#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void process(int from_left[2]){
    int p;
    if(read(from_left[0], &p, sizeof(p)) > 0){
       printf("prime: %d\n", p); 
    } else {
        exit(0);
    }
    
    int to_right[2];
    pipe(to_right);
    if (fork() == 0){
        close(to_right[1]);
        close(from_left[0]);
        process(to_right);
    } else {
        close(to_right[0]);
        int num;
        while (read(from_left[0], &num, sizeof(num)) > 0){
            if (num % p != 0){
                write(to_right[1], &num, sizeof(num));
            }
        }
        close(to_right[1]);
        wait(0);
        exit(0);
    }
    
}

int main(int argc, char *argv[]){
    int input[2];
    pipe(input);
    if(fork() == 0){
        close(input[1]);
        process(input);
    } else {
        close(input[0]);
        int i;
        for (i = 2; i <= 35; i++) {
            write(input[1], &i, sizeof(i));
        }
        close(input[1]);
    }

    wait(0);
    exit(0);
}