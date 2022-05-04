#include "../PriorityQueue.h"

TEST(PriorityQueue, idk) {
    PriorityQueue<int> q;
    ASSERT_EQ(q.Size(), 0);
    q.Push(5);
    ASSERT_EQ(q.Size(), 1);
    ASSERT_EQ(q.Top(), 5);
    q.Push(3);
    ASSERT_EQ(q.Size(), 2);
    ASSERT_EQ(q.Top(), 3);
    q.Push(3);
    ASSERT_EQ(q.Size(), 3);
    ASSERT_EQ(q.Top(), 3);
    q.Pop();
    ASSERT_EQ(q.Size(), 2);
    ASSERT_EQ(q.Top(), 5);
}