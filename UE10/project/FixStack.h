//
// Created by localadmin on 18.06.18.
//

#ifndef PROJECT_FIX_STACK_H
#define PROJECT_FIX_STACK_H

template <typename T>
class FixStack
{
    T* _data;
    const int SIZE;
    int _index = -1;
public:
    FixStack() : SIZE(10) {
        _data = new T[SIZE];
    }
    bool push(T);
    bool pop(T&);
};

template <typename T>
bool FixStack<T>::push(T e)
{
    if (_index == SIZE - 1) return false;
    _data[++_index] = e;
    return true;
}

template <typename T>
bool FixStack<T>::pop(T& e)
{
    if (_index == -1) return false;
    e = _data[_index--];
    return true;
}

#endif //PROJECT_FIX_STACK_H
