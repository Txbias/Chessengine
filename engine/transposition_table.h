#pragma once

#include <mutex>

typedef unsigned long U64;

struct Entry {
    Entry(U64 _hash, int _depth, int _score) :
            hash(_hash), depth(_depth), score(_score), countRequested(0) {}

    Entry() {
        hash = 0;
        depth = -1;
        score = -1;
        countRequested = 0;
    }
    U64 hash;
    int depth;
    int score;
    int countRequested;
};

class TranspositionTable {
public:
    TranspositionTable();

    void addEntry(Entry entry);
    void updateAfterSearch();

    Entry getEntry(U64 hash);
private:
    std::mutex tableMutex;

    static const int TABLE_SIZE = 100000;
    Entry entries[TABLE_SIZE];
};