#include "transposition_table.h"

TranspositionTable::TranspositionTable() {
    for(auto & entry : entries) {
        entry = Entry();
    }
}

TranspositionTable & TranspositionTable::getInstance() {
    static auto *transpositionTable = new TranspositionTable;
    return *transpositionTable;
}

void TranspositionTable::addEntry(Entry entry) {
    unsigned long index = entry.hash % TABLE_SIZE;

    std::lock_guard<std::mutex> lock(tableMutex);
    if(entries[index].countRequested == 0) {
        entries[index] = entry;
    }
}

void TranspositionTable::updateAfterSearch() {
    std::lock_guard<std::mutex> lock(tableMutex);
    for(auto &entry : entries) {
        entry.countRequested = 0;
    }
}

Entry TranspositionTable::getEntry(U64 hash) {
    U64 index = hash % TABLE_SIZE;
    entries[index].countRequested++;
    return entries[hash % TABLE_SIZE];
}