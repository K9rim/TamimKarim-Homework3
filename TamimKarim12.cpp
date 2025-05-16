#ifndef MY_DEQUE_H
#define MY_DEQUE_H

#include <iostream>
#include <stdexcept>

template<typename T>
class MyDeque {
private:
    static const int BLOCK_SIZE = 64;
    T** blocks;
    int blockCount;
    int frontBlock, backBlock;
    int frontIndex, backIndex;
    size_t count;

    void allocateBlocks(int count) {
        blocks = new T*[count];
        for (int i = 0; i < count; ++i) {
            blocks[i] = nullptr;
        }
    }

    void grow() {
        int newBlockCount = blockCount * 2;
        T** newBlocks = new T*[newBlockCount];
        for (int i = 0; i < newBlockCount; ++i)
            newBlocks[i] = nullptr;

        int offset = (newBlockCount - blockCount) / 2;
        for (int i = 0; i < blockCount; ++i)
            newBlocks[i + offset] = blocks[i];

        delete[] blocks;
        blocks = newBlocks;
        frontBlock += offset;
        backBlock += offset;
        blockCount = newBlockCount;
    }

    void ensureFrontCapacity() {
        if (frontIndex == 0) {
            if (frontBlock == 0)
                grow();
            --frontBlock;
            if (!blocks[frontBlock])
                blocks[frontBlock] = new T[BLOCK_SIZE];
            frontIndex = BLOCK_SIZE;
        }
    }

    void ensureBackCapacity() {
        if (backIndex == BLOCK_SIZE) {
            if (backBlock == blockCount - 1)
                grow();
            ++backBlock;
            if (!blocks[backBlock])
                blocks[backBlock] = new T[BLOCK_SIZE];
            backIndex = 0;
        }
    }

    void cleanUp() {
        for (int i = 0; i < blockCount; ++i) {
            delete[] blocks[i];
        }
        delete[] blocks;
    }

public:
    MyDeque() {
        blockCount = 8;
        allocateBlocks(blockCount);
        frontBlock = backBlock = blockCount / 2;
        blocks[frontBlock] = new T[BLOCK_SIZE];
        frontIndex = backIndex = BLOCK_SIZE / 2;
        count = 0;
    }

    ~MyDeque() {
        cleanUp();
    }

    MyDeque(const MyDeque& other) {
        blockCount = other.blockCount;
        frontBlock = other.frontBlock;
        backBlock = other.backBlock;
        frontIndex = other.frontIndex;
        backIndex = other.backIndex;
        count = other.count;

        allocateBlocks(blockCount);
        for (int i = frontBlock; i <= backBlock; ++i) {
            if (other.blocks[i]) {
                blocks[i] = new T[BLOCK_SIZE];
                for (int j = 0; j < BLOCK_SIZE; ++j) {
                    blocks[i][j] = other.blocks[i][j];
                }
            }
        }
    }

    MyDeque& operator=(const MyDeque& other) {
        if (this == &other)
            return *this;

        cleanUp();

        blockCount = other.blockCount;
        frontBlock = other.frontBlock;
        backBlock = other.backBlock;
        frontIndex = other.frontIndex;
        backIndex = other.backIndex;
        count = other.count;

        allocateBlocks(blockCount);
        for (int i = frontBlock; i <= backBlock; ++i) {
            if (other.blocks[i]) {
                blocks[i] = new T[BLOCK_SIZE];
                for (int j = 0; j < BLOCK_SIZE; ++j) {
                    blocks[i][j] = other.blocks[i][j];
                }
            }
        }

        return *this;
    }

    void push_back(const T& value) {
        ensureBackCapacity();
        blocks[backBlock][backIndex++] = value;
        ++count;
    }

    void push_front(const T& value) {
        ensureFrontCapacity();
        blocks[frontBlock][--frontIndex] = value;
        ++count;
    }

    void pop_back() {
        if (count == 0)
            throw std::out_of_range("Deque is empty");
        if (backIndex == 0) {
            --backBlock;
            backIndex = BLOCK_SIZE;
        }
        --backIndex;
        --count;
    }

    void pop_front() {
        if (count == 0)
            throw std::out_of_range("Deque is empty");
        ++frontIndex;
        if (frontIndex == BLOCK_SIZE) {
            ++frontBlock;
            frontIndex = 0;
        }
        --count;
    }

    T& front() {
        if (count == 0)
            throw std::out_of_range("Deque is empty");
        return blocks[frontBlock][frontIndex];
    }

    T& back() {
        if (count == 0)
            throw std::out_of_range("Deque is empty");
        return blocks[backBlock][backIndex - 1];
    }

    size_t size() const {
        return count;
    }

    size_t capacity() const {
        return blockCount * BLOCK_SIZE;
    }
};

#endif
