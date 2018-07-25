#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#define FILE_NAME   "demo.txt"
#define CONSOLE     "/dev/tty"
#define err_exit(m) {perror(m); exit(1);}
int fd; //设置成全局变量，方便下面的程序访问
int main(){
    /************************参数说明如下：***********************************
     *     *O_RDWR   对文件的操作权限，可读可写操作
     *         *O_CREAT  如果文件不存在就新建该文件，用于记录写入内容；后面的0644设定文件的进入权限
     *             *O_TRUNC  每次打开文件的时候都清空文件原先的内容
     *                 *O_APPEND 设定定位与写入操作的原子性，每次写入都追加到文件末尾
     *                     ***********************************************************************/
    fd = open(FILE_NAME, O_RDWR | O_CREAT | O_TRUNC | O_APPEND , 0644);
    pthread_t pth1, pth2, pth3, pth4;
    char    buf[4096];
    void    write_block(void *);
    memset(buf, 'a', 4093);  //设定一个很大的数组4096*1024，测试其他进程是否可以打断该操作。
    memset(buf+4094,'\n', 1);    //将该数组的内容设定为"aaaaaaa...\n",方便我们观测。
    memset(buf+4095,'\0', 1);    //当然最后以0结尾，因为下面的函数用到strlen方法。
    pthread_create(&pth2, NULL, write_block, buf);
    pthread_create(&pth1, NULL, write_block, "bb123bb\n");
    pthread_create(&pth4, NULL, write_block, "cc222222cc\n");
    pthread_create(&pth3, NULL, write_block, "d123ddd\n");
    pthread_join(pth3, NULL);
    pthread_join(pth2, NULL);
    pthread_join(pth4, NULL);
    pthread_join(pth1, NULL);
    return 0;
}
void    write_block(void* buf){
    int     i = 0;
    int len = strlen(buf);
    for( ; i < 10; i++ ){
            write(fd, buf,len);
        }
}
