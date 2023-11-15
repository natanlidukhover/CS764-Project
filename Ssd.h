#ifndef SSD_H
#define SSD_H

#include <cstdio>
#include <cstdlib>

constexpr size_t PAGE_SIZE = 8192; //8KB of data

class Ssd {
public:
    Ssd(const char* filename, size_t size);
    ~Ssd();
    void* writeData(const void* data);
    void* readData(void* buffer, size_t offset);

    size_t getReadCount() const;
    size_t getWriteCount() const;

private:
    FILE* filePtr;
    size_t _size;
    size_t _sizeOccupied;
    size_t _readCount;  // read call counter
    size_t _writeCount; // write call counter
};

#endif // SSD_H
