#include "gtest/gtest.h"
#include "transposition_table.h"

TEST(TranspositionTable, addEntry) {
    TranspositionTable transpositionTable;

    U64 hash = 0x23423414;
    int depth = 3;
    int score = 10;

    Entry entry(hash, depth, score);
    transpositionTable.addEntry(entry);

    Entry res = transpositionTable.getEntry(hash);

    ASSERT_EQ(res.hash, hash);
    ASSERT_EQ(res.depth, depth);
    ASSERT_EQ(res.score, score);
}

TEST(TranspositionTable, countRequested) {
    TranspositionTable transpositionTable;

    U64 hash = 0x234234;
    int depth = 2;
    int score = 1234;

    Entry entry(hash, depth, score);
    transpositionTable.addEntry(entry);

    Entry res = transpositionTable.getEntry(hash);
    ASSERT_EQ(res.countRequested, 1);

    transpositionTable.updateAfterSearch();

    res = transpositionTable.getEntry(hash);
    // Is one again, because it was just requested the line above
    ASSERT_EQ(res.countRequested, 1);
}