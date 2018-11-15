//
// Created by chengzi on 18-11-14.
//

#include <iostream>
#include "CLQueue.h"


template <class T>
Queue<T>::Queue(int c):
    capacity(c+1),
    front(0),
    rear(0),
    m_Queue(NULL)
{
    m_Queue = new T[c+1];
    std::cout << "capacity:" << c << std::endl;
}


template <class T>
Queue<T>::~Queue() {
    if(m_Queue){
        delete []m_Queue;
        m_Queue = NULL;
    }
}

template <class T>
bool Queue<T>::enqueue(T data) {
    if(isFull())
        return false;
    m_Queue[rear] = data;
    rear = (rear+1)%capacity;
    return true;
}

template <class T>
T Queue<T>::dequeue() {
    T data;
    if(isEmpty()){
        throw std::out_of_range("Queue:dequeue() - Empty queue");
    }
    data = m_Queue[front];
    front = (front+1)%capacity;
    return data;
}


template <class T>
bool Queue<T>::isEmpty() {
    return (front == rear);
}

template <class T>
bool Queue<T>::isFull() {
    return (((rear + 1)%capacity) == front);
}

template <class T>
int Queue<T>::size() {
    return ((rear - front +capacity ) % capacity);
}









