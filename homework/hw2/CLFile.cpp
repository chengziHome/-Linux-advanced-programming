//
// Created by chengzi on 18-11-8.
//

#include "CLFile.h"
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <typeinfo>





CLFile::CLFile() {
    isOpen = false;
}
CLFile::~CLFile() {

}

CLStatus CLFile::openFile(const char *filename) {
    mFID = open(filename, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);
    if(mFID == -1)
        throw "In CLFile::CLFile(), open error";
    isOpen = true;
    loadCache();
    return CLStatus(0,0);
}

CLStatus CLFile::readStr(char *buf, size_t nbyte) {
    if(!isOpen)
        return CLStatus(-1,0);

    int currentPos = lseek(mFID,0,SEEK_CUR);

    if(nbyte>CACHE_SIZE){
        flush();
        isWrite = false;
        read(mFID,buf,nbyte);
    }else if(currentPos>=cache_pos &&  (currentPos+nbyte)<=(cache_pos+CACHE_SIZE)){
        memcpy(buf,cache+(currentPos-cache_pos),nbyte);
    }else{
        if(isWrite){
            flush();
            isWrite = false;
        }
        loadCache();
        cache_pos = currentPos;
        return readStr(buf,nbyte);
    }
    return CLStatus(0,0);
}

CLStatus CLFile::writeStr(const char *buf, size_t nbyte) {
    if(!isOpen)
        return CLStatus(-1,0);


    if(offset>=cache_pos &&  (offset+nbyte)<=(cache_pos+CACHE_SIZE)){
        std::cout << "1,int span " << std::endl;
        memcpy(cache+(offset-cache_pos),buf,nbyte);
        offset = offset+nbyte;
        isWrite = true;
    }else{
        std::cout << "2,out of span" << std::endl;
        //无论是不是大于缓存区间，都要写后重新load
        //这里策略是，先往磁盘写，然后再重新load，注意重新load之后的缓存区的区间已经变了，到写的结尾了
        if(isWrite){
            flush();
            isWrite= false;
        }
        lseek(mFID,offset,SEEK_SET);
        write(mFID,buf,nbyte);
        cache_pos = offset = lseek(mFID,0,SEEK_CUR);
        loadCache();

    }
    int currentPos = lseek(mFID,0,SEEK_CUR);//
    std::cout << "after write ,cache_offset:" << offset << ",cache_pos:" << cache_pos << std::endl;

    return CLStatus(0,0);
}

/**
 * 看一下lseek的含义：http://pubs.opengroup.org/onlinepubs/7908799/xsh/lseek.html
 * 简单说就是：
 *      文件描述符有一个offset属性，它会影响read和write操作，当APPEND设置的时候，每次write都是将offset设置到末尾
 *      lseek方法就是用来设置这个offset的。
 * @param currentPos
 * @param offset
 * @param whence
 * @return
 *
 */
CLStatus CLFile::lseekOff(int *currentPos,int offset, int whence) {
    if(!isOpen)
        return CLStatus(-1,0);


    *currentPos = lseek(mFID,offset,whence);
    if(*currentPos == -1){
        std::cout << "lseek error:" << strerror(errno) << std::endl;
        return CLStatus(-1,errno);
    }
    return CLStatus(0,0);
}


/**
 * 长度不够用空字符串补齐
 * @return
 */
CLStatus CLFile::loadCache() {
    int pos = lseek(mFID,0,SEEK_CUR);
    int end_pos = lseek(mFID,0,SEEK_END);
    if((end_pos-pos)<CACHE_SIZE){
        read(mFID,cache,(end_pos-pos));
        for(int i=(end_pos-pos);i<CACHE_SIZE;i++){
            cache[i] = ' ';
        }
    }else{
        read(mFID,cache,CACHE_SIZE);
    }

    lseek(mFID,pos,SEEK_SET);
}

CLStatus CLFile::flush() {
    int pos = lseek(mFID,0,SEEK_CUR);
    lseek(mFID,cache_pos,SEEK_SET);
    write(mFID,cache,CACHE_SIZE);
    lseek(mFID,pos,SEEK_SET);
}

CLStatus CLFile::closeFile() {
    if(mFID!=-1){
        flush();
        close(mFID);
    }
    return CLStatus(0,0);
}



