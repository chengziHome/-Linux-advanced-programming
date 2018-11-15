//
// Created by chengzi on 18-11-8.
//


#include "CLFile.h"
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>


using namespace std;

void test(char *a){
    CLFile *file = new CLFile();
    file->openFile("chengzi.txt");
    int currentPos;
    file->lseekOff(&currentPos,0,SEEK_CUR);
    cout << "currentPos:" << currentPos << endl;
    char *str = "My name is chengzi";
    file->writeStr(str,strlen(str));
    file->lseekOff(&currentPos,0,SEEK_CUR);
    cout << "currentPos:" << currentPos << endl;
    char buf[10];
    cout << "buf. sizeof:" << sizeof(buf) << endl;
    file->readStr(buf,9);

    file->closeFile();
    delete file;
}

void test2(){
    CLFile *file = new CLFile();
    file->openFile("chengzi.txt");
    int currentPos;
    file->lseekOff(&currentPos,0,SEEK_SET);
    char buf[50] = "1111111111222222222233333333334444444444555555555";
    char c[1] = {'\n'};

    for(int i=0;i<5;i++){
        cout << "i=" << i << endl;
        file->writeStr(buf,50);
        file->writeStr(c,1);

        file->lseekOff(&currentPos,0,SEEK_SET);
        cout << "currentPos:" << currentPos << endl;
    }
    file->closeFile();
    delete file;
}

/**
 * 测试一下，从一个文件原生读，测试writeStr方法
 */
void test3(){
    int fID = open("test.txt",O_RDONLY);
    int end_pos = lseek(fID,0,SEEK_END);

    cout << "end_pos:" << end_pos << endl;
    char buf[end_pos];
    read(fID,buf,end_pos);

    string str = buf;
    cout << "str:\n" << str << endl;

    CLFile *file = new CLFile();
    file->openFile("chengzi.txt");
    for(int i=0;i<10 ;i++){
        file->writeStr(buf+(i*105),105);
    }


    file->closeFile();
    delete file;
}

int main(){

    test3();
    return 0;
}
