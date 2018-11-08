//
// Created by chengzi on 18-11-3.
//

#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>



using namespace std;

int main() {
    int fd = open("a.c", O_RDWR | O_APPEND);
    if (-1 == fd) {
        cout << strerror(errno) << endl;
        return 0;
    }

    char buf[] = "%%%";

    //先看看偏移量初始值是不是0
    off_t  currentPosition = lseek(fd,0,SEEK_CUR);
    if (currentPosition == -1) {
        cout << "lseek error:" << strerror(errno) << endl;
        close(fd);
        return 0;
    }
    std::cout << "pos1:" <<  currentPosition << std::endl;

    //读取3个字符并且检查
    ssize_t i = read(fd,buf,3);
    if (i == -1) {
        cout << "read error:" << strerror(errno) << endl;
        close(fd);
        return 0;
    }
    std::cout << "buf1:" << buf << std::endl;

    //偏移两个单位
    off_t  currentPosition2 = lseek(fd,2,SEEK_CUR);
    if (currentPosition2 == -1) {
        cout << "lseek error:" << strerror(errno) << endl;
        close(fd);
        return 0;
    }
    std::cout << "pos2:" <<  currentPosition2 << std::endl;

    //再读取三个字符，看是否越过"45"两个字符
    ssize_t j = read(fd,buf,3);
    if (j == -1) {
        cout << "read error:" << strerror(errno) << endl;
        close(fd);
        return 0;
    }
    std::cout << "buf2:" << buf << std::endl;

    //最后将3个字符写入，看是拼接在8后面，还是9后面
    char buf2[] = "XYZ";
    ssize_t len = write(fd,buf2,3);
    if (len == -1) {
        cout << "write error:" << strerror(errno) << endl;
        close(fd);
        return 0;
    }
    std::cout << "len:" << len << std::endl;
    close(fd);

    return 0;
}




