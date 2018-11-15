//
// Created by chengzi on 18-11-8.
//

#ifndef CODE_CLFILE_H
#define CODE_CLFILE_H
#endif //CODE_CLFILE_H


#include "CLStatus.h"
#include <unistd.h>
#include <sys/types.h>

#define CACHE_SIZE 1024


/**
 *
 * 要注意的一些问题：
 * 1,我这个File类没有单独设置offset属性，所以在load和store操作的时候都要记得把原生offset复原
 *
 *
 */

class CLFile{
public:
    CLFile();
    ~CLFile();

    CLStatus openFile(const char* filename);//不做过多设置，默认是RDWR方法打开
    CLStatus readStr(char *buf,size_t nbyte);//简单点，就是字节读取类
    CLStatus writeStr(const char* buf,size_t nbyte);//简单点
    CLStatus lseekOff(int *currentPos,int offset,int whence);
    CLStatus flush();
    CLStatus closeFile();

private:
    CLStatus loadCache();

private:
    int mFID;
    bool isOpen;

    char cache[CACHE_SIZE];
    int cache_pos;//cache的起始地址偏移
    int offset;//offset的虚拟变量，不一定和FID中offset同步，
    bool isWrite;



};


