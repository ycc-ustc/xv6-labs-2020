#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    //创建两个管道，一个子进程向父进程，一个父进程到子进程
    int parent2child[2], child2parent[2];
    pipe(parent2child);
    pipe(child2parent);
    if (fork() != 0) //父进程
    {
        write(parent2child[1], "1", 1); //父进程发送一个字节
        char buf;
        read(child2parent[0], &buf, 1); //等待子进程发送数据
        printf("%d: received pong\n", getpid());
        wait(0);
    } else { //子进程
        char buf;
        read(parent2child[0], &buf, 1); //等待子进程发送数据
        printf("%d: received ping\n", getpid());
        write(child2parent[1], &buf, 1); //父进程发送一个字节
    }
    exit(0);
}