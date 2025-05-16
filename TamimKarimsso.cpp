#include <iostream>
#include <cstring>

class SmallString {
private:
    static const size_t SSO_MAX = 15;

    union {
        char small[SSO_MAX + 1];
        struct {
            char* heap;
            size_t cap;
        };
    };

    size_t len;
    bool usingHeap;

    void switchToHeap(size_t newCap) {
        char* newMem = new char[newCap + 1];
        std::memcpy(newMem, small, len);
        newMem[len] = '\0';
        heap = newMem;
        cap = newCap;
        usingHeap = true;
    }

public:
    SmallString() : len(0), usingHeap(false) {
        small[0] = '\0';
    }

    SmallString(const char* str) {
        len = std::strlen(str);
        if (len <= SSO_MAX) {
            usingHeap = false;
            std::memcpy(small, str, len + 1);
        } else {
            usingHeap = true;
            cap = len;
            heap = new char[cap + 1];
            std::memcpy(heap, str, len + 1);
        }
    }

    SmallString(const SmallString& other) {
        len = other.len;
        usingHeap = other.usingHeap;
        if (usingHeap) {
            cap = other.cap;
            heap = new char[cap + 1];
            std::memcpy(heap, other.heap, len + 1);
        } else {
            std::memcpy(small, other.small, len + 1);
        }
    }

    SmallString& operator=(const SmallString& other) {
        if (this != &other) {
            if (usingHeap) delete[] heap;

            len = other.len;
            usingHeap = other.usingHeap;
            if (usingHeap) {
                cap = other.cap;
                heap = new char[cap + 1];
                std::memcpy(heap, other.heap, len + 1);
            } else {
                std::memcpy(small, other.small, len + 1);
            }
        }
        return *this;
    }

    ~SmallString() {
        if (usingHeap) delete[] heap;
    }

    size_t size() const {
        return len;
    }

    size_t capacity() const {
        return usingHeap ? cap : SSO_MAX;
    }

    void reserve(size_t newCap) {
        if (!usingHeap && newCap > SSO_MAX) {
            switchToHeap(newCap);
        } else if (usingHeap && newCap > cap) {
            char* newMem = new char[newCap + 1];
            std::memcpy(newMem, heap, len + 1);
            delete[] heap;
            heap = newMem;
            cap = newCap;
        }
    }

    void push_back(char c) {
        if (len + 1 <= capacity()) {
            if (usingHeap) {
                heap[len++] = c;
                heap[len] = '\0';
            } else {
                small[len++] = c;
                small[len] = '\0';
            }
        } else {
            size_t newCap = capacity() * 2;
            reserve(newCap);
            push_back(c);
        }
    }

    char operator[](size_t index) const {
        if (usingHeap) return heap[index];
        return small[index];
    }

    const char* c_str() const {
        return usingHeap ? heap : small;
    }
};

int main() {
    SmallString s("Tamim");
    s.push_back('K');
    s.push_back('a');
    s.push_back('r');
    s.push_back('i');
    s.push_back('m');

    std::cout << "String: " << s.c_str() << std::endl;
    std::cout << "Size: " << s.size() << std::endl;
    std::cout << "Capacity: " << s.capacity() << std::endl;

    SmallString big("this is a very long string");
    std::cout << "Big string: " << big.c_str() << std::endl;
    std::cout << "Big string size: " << big.size() << std::endl;
    std::cout << "Big string cap: " << big.capacity() << std::endl;

    return 0;
}
