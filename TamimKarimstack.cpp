#ifndef MY_STACK_H
#define MY_STACK_H

#include "TamimKarim12.h"

template<typename T>
class MyStack {
private:
    MyDeque<T> dq;

public:
    MyStack() {}

    void push(const T& value) {
        dq.push_back(value);
    }

    void pop() {
        dq.pop_back();
    }

    T& top() {
        return dq.back();
    }

    const T& top() const {
        return dq.back();
    }

    bool empty() const {
        return dq.size() == 0;
    }

    size_t size() const {
        return dq.size();
    }
};

#endif
