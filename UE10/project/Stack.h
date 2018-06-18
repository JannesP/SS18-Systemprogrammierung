//
// Created by localadmin on 18.06.18.
//

#ifndef PROJECT_STACK_H
#define PROJECT_STACK_H

template <typename T, int SIZE>
class Stack
{
    T* _data;
    int _index = -1;
public:
    Stack() {
        _data = new T[SIZE];
    }
    bool push(T);
    bool pop(T&);
};

template <typename T, int SIZE>
bool Stack<T,SIZE>::push(T e)
{
    if (_index == SIZE - 1) return false;
    _data[++_index] = e;
    return true;
}

template <typename T, int SIZE>
bool Stack<T,SIZE>::pop(T& e)
{
    if (_index == -1) return false;
    e = _data[_index--];
    return true;
}

#endif //PROJECT_STACK_H
