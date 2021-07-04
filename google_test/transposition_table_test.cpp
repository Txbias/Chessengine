#include "gtest/gtest.h"
#include "transposition_table.h"

TEST(TranspositionTable, addEntry) {

    U64 hash = 0x23423414;
    int depth = 3;
    int score = 10;

    Entry entry(hash, depth, score);
    TranspositionTable::getInstance().addEntry(entry);

    Entry res = TranspositionTable::getInstance().getEntry(hash);

    ASSERT_EQ(res.hash, hash);
    ASSERT_EQ(res.depth, depth);
    ASSERT_EQ(res.score, score);
}

TEST(TranspositionTable, countRequested) {
    U64 hash = 0x234234;
    int depth = 2;
    int score = 1234;

    Entry entry(hash, depth, score);
    TranspositionTable::getInstance().addEntry(entry);

    Entry res = TranspositionTable::getInstance().getEntry(hash);
    ASSERT_EQ(res.countRequested, 1);

    TranspositionTable::getInstance().updateAfterSearch();

    res = TranspositionTable::getInstance().getEntry(hash);
    // Is one again, because it was just requested the line above
    ASSERT_EQ(res.countRequested, 1);
}