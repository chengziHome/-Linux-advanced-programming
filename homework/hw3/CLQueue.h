//
// Created by chengzi on 18-11-14.
//

#ifndef CODE_CLQUEUE_H
#define CODE_CLQUEUE_H

#endif //CODE_CLQUEUE_H


template <class T>
class Queue {
public:
    Queue(int c = 32);

    ~Queue();

    bool enqueue(T data);

    T dequeue();

    bool isEmpty();

    bool isFull();

    int size();

private:
    T *m_Queue;
    int front;
    int rear;
    int capacity;
};





