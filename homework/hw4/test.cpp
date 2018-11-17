#include <iostream>
#include "CLThread.h"
#include "CLExecutiveFunctionProvider.h"
#include "CLMessageQueueBySTLqueue.h"
//#include "CLMessage.h"
#include "CLMsgLoopManagerForSTLqueue.h"
#include "CLMsgControllerForSTLqueue.h"
//#include "CLMessageAdd.cpp"
#include "CLAdder.h"

using namespace std;


#define ADD_MSG 0
#define QUIT_MSG 1


int main() {
    CLMessageQueueBySTLqueue *pQ = new CLMessageQueueBySTLqueue();
//    CLMessageLoopManager *pM = new CLMyMsgProcessor(pQ);
    CLMsgControllerForSTLqueue *pC = new CLMsgControllerForSTLqueue(pQ,10);

    CLThread *t = new CLThread(new CLAdder(pC), true);
    t->Run(0);

    for(int i=1;i<1000;i++){
        CLMessageAdd *paddmsg = new CLMessageAdd(i,2, 4);
        pQ->PushMessage(paddmsg);
        cout << "put message no." << i << endl;
    }
    t->WaitForDeath();


    return 0;
}
