#include <iostream>

template<typename T, typename Deleter>
class SharedPtr {
private:
    T* ptr;
    int* refCount;
    Deleter deleter;

public:
    SharedPtr(T* rawPtr) : ptr(rawPtr), refCount(new int(1)), deleter(Deleter()) {}

    SharedPtr(const SharedPtr& other) {
        ptr = other.ptr;
        refCount = other.refCount;
        deleter = other.deleter;
        (*refCount)++;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            refCount = other.refCount;
            deleter = other.deleter;
            (*refCount)++;
        }
        return *this;
    }

    ~SharedPtr() {
        release();
    }

    T& operator*() {
        return *ptr;
    }

    T* operator->() {
        return ptr;
    }

    int use_count() const {
        return *refCount;
    }

private:
    void release() {
        if (--(*refCount) == 0) {
            deleter(ptr);
            delete refCount;
        }
    }
};

struct MyDeleter {
    void operator()(int* p) const {
        std::cout << "Deleting int: " << *p << std::endl;
        delete p;
    }
};

int main() {
    SharedPtr<int, MyDeleter> p1(new int(42));
    {
        SharedPtr<int, MyDeleter> p2 = p1;
        std::cout << "Use count: " << p1.use_count() << std::endl;
        std::cout << "Value: " << *p2 << std::endl;
    }
    std::cout << "After inner scope. Use count: " << p1.use_count() << std::endl;

    return 0;
}
