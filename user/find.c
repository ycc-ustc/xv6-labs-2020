#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path, char* target){
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512], *p;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // 排除掉仅后缀相同的情况:ttest && test × ==> path/test && test √
		if(strcmp(path + strlen(path) - strlen(target), target) == 0 
        && *(path + strlen(path) - strlen(target) - 1) == '/') {
			printf("%s\n", path);
		}
		break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    // 确保路径结尾为'/'
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      // printf("44%s\n", de.name);
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      //跳过.和..
			if(strcmp(buf + strlen(buf) - 2, "/.") != 0 && strcmp(buf + strlen(buf) - 3, "/..") != 0) {
        // printf("51%s\n", buf);
				find(buf, target); 
			}
    }
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("usage error");
    } else {
        find(argv[1], argv[2]);
    }
    return 0;
}