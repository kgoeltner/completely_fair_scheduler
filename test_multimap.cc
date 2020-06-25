//
// Karl Goeltner
// 917006087
// ECS 36C - 05/22/2020
//
// test_multimap.cc - Unit tester for multimap.h
//

#include <gtest/gtest.h>
#include <vector>

#include "multimap.h"

// 1) Check one key: insert, contains, get
TEST(Multimap, OneKey) {
  Multimap<int, int> multimap;
  std::vector<int> keys{2};

  for (auto i : keys) {
    multimap.Insert(i, i);
  }

  EXPECT_EQ(multimap.Contains(2), true);
  EXPECT_EQ(multimap.Get(2), keys[0]);
}

// 2) Check multiple keys: insert, contains, get
TEST(Multimap, MultipleKeys) {
  Multimap<int, int> multimap;
  std::vector<int> keys{2, 18, 42, 43};

  // Insert a bunch of keys
  for (auto i : keys) {
    multimap.Insert(i, i);
  }

  // Check that every key is contained
  std::random_shuffle(keys.begin(), keys.end());
  for (auto i : keys) {
    EXPECT_EQ(multimap.Contains(i), true);
    EXPECT_EQ(multimap.Get(i), i);
  }
}

// 3) Check maximum & minimum keys: insert, remove, max, min
TEST(Multimap, MaxMin) {
  Multimap<int, int> multimap;
  std::vector<int> keys{48, 29, 89, 194, 5};

  // Insert a bunch of keys
  for (auto i : keys) {
    multimap.Insert(i, i);
  }

  // Check max & min element
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Max(), 194);
  EXPECT_EQ(multimap.Min(), 5);

  // Remove max & min
  multimap.Remove(194);
  multimap.Remove(5);

  // Check max & min element
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_NE(multimap.Max(), 194);
  EXPECT_NE(multimap.Min(), 5);
  EXPECT_EQ(multimap.Max(), 89);
  EXPECT_EQ(multimap.Min(), 29);
}

// 4) Check maximum & minimum keys: insert, remove, max, min, print
TEST(Multimap, MultipleMaxMin) {
  Multimap<int, int> multimap;
  std::vector<int> keys{11, 33, 22, 88, 44};

  // Insert multiple duplicates
  for (auto i : keys) {
    multimap.Insert(i, i*3);
    multimap.Insert(i, i);
    multimap.Insert(i, i*2);
  }

  // Check max & min element
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Max(), 88);
  EXPECT_EQ(multimap.Min(), 11);

  // Get max & min value
  EXPECT_EQ(multimap.Get(multimap.Max()), 88*3);
  EXPECT_EQ(multimap.Get(multimap.Min()), 11*3);

  // Remove first duplicate
  for (auto i : keys) {
    multimap.Remove(i);
  }

  // Check max & min element
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Max(), 88);
  EXPECT_EQ(multimap.Min(), 11);

  // Get max & min value
  EXPECT_EQ(multimap.Get(multimap.Max()), 88);
  EXPECT_EQ(multimap.Get(multimap.Min()), 11);

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "<11,11> <11,22> <22,22>"
   " <22,44> <33,33> <33,66> <44,44> <44,88> <88,88> <88,176> \n");
}

// 5) Check size and get exception: insert, remove, size, get
TEST(Multimap, ExceptionCase) {
  Multimap<int, int> multimap;
  std::vector<int> keys{10, 20, 30, 40, 50};

  // Check size
  EXPECT_EQ(multimap.Size(), 0);

  // Insert a bunch of keys
  for (auto i : keys) {
    multimap.Insert(i, i);
  }

  // Check size
  EXPECT_EQ(multimap.Size(), 5);

  // Check get exception
  EXPECT_THROW(multimap.Get(42), std::exception);
  EXPECT_THROW(multimap.Get(0), std::exception);

  // Check get elements
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Get(10), 10);
  EXPECT_EQ(multimap.Get(40), 40);

  // Remove all elements
  for (auto i : keys) {
    multimap.Remove(i);
  }

  // Check size & get exception
  EXPECT_EQ(multimap.Size(), 0);
  EXPECT_THROW(multimap.Get(10), std::exception);
  EXPECT_THROW(multimap.Get(50), std::exception);
}

// 6) Check duplicate insertion & removal: insert, remove, size, get
TEST(Multimap, DuplicateKeys) {
  Multimap<int, int> multimap;
  std::vector<int> keys{90, 60, 15, 30, 75, 45};

  // Check size
  EXPECT_EQ(multimap.Size(), 0);

  // Insert duplicate x3 key-value pairs
  for (auto i : keys) {
    multimap.Insert(i, i);
    multimap.Insert(i, i);
    multimap.Insert(i, i);
  }

  // Check size
  EXPECT_EQ(multimap.Size(), 18);

  // Check get exception
  EXPECT_THROW(multimap.Get(92), std::exception);
  EXPECT_THROW(multimap.Get(31), std::exception);

  // Check get elements
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Get(90), 90);
  EXPECT_EQ(multimap.Get(30), 30);

  // Remove duplicate x2 key-value pairs
  for (auto i : keys) {
    multimap.Remove(i);
    multimap.Remove(i);
  }

  // Check size & get exception
  EXPECT_EQ(multimap.Size(), 6);
  EXPECT_EQ(multimap.Get(15), 15);
  EXPECT_EQ(multimap.Get(75), 75);
}

// 7) Check duplicate keys with diff values: insert, remove, size, print
TEST(Multimap, DuplicateKeysDiffValues) {
  Multimap<int, int> multimap;
  std::vector<int> keys{5, 10, 15};

  // Check size
  EXPECT_EQ(multimap.Size(), 0);

  // Insert duplicate x3 key & diff value pairs
  for (auto i : keys) {
    multimap.Insert(i, i);
    multimap.Insert(i, i*2);
    multimap.Insert(i, i*4);
  }

  // Check size
  EXPECT_EQ(multimap.Size(), 9);

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "<5,5> <5,10> <5,20>"
   " <10,10> <10,20> <10,40> <15,15> <15,30> <15,60> \n");

  // Remove first key-value duplicate pairs
  for (auto i : keys) {
    multimap.Remove(i);
  }

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "<5,10> <5,20>"
   " <10,20> <10,40> <15,30> <15,60> \n");
}

// 8) Check first obtained duplicate keys with diff values: insert, remove, get
TEST(Multimap, GetDuplicateFirst) {
  Multimap<int, int> multimap;
  std::vector<int> keys{20, 10, 30, 40};

  // Insert duplicate x3 key & diff value pairs
  for (auto i : keys) {
    multimap.Insert(i, i*2);
    multimap.Insert(i, i);
    multimap.Insert(i, i*4);
  }

  // Check that first obtained is i*2 value
  for (auto i : keys) {
    EXPECT_EQ(multimap.Get(i), i*2);
  }

  // Remove first key-value duplicate pairs
  for (auto i : keys) {
    multimap.Remove(i);
  }

  // Check that first obtained is i value
  for (auto i : keys) {
    EXPECT_EQ(multimap.Get(i), i);
  }
}

// 9) Check printing: print, insert, remove, size
TEST(Multimap, PrintInsertRemoval) {
  Multimap<int, int> multimap;
  std::vector<int> keys{109, 23, 5, 41, 88, 209, 13};

  // Insert key-value pairs
  for (auto i : keys) {
    multimap.Insert(i, i);
  }

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "<5,5> <13,13>"
    " <23,23> <41,41> <88,88> <109,109> <209,209> \n");

  // Remove some key-value pairs
  multimap.Remove(109);
  multimap.Remove(23);
  multimap.Remove(5);

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "<13,13> <41,41>"
    " <88,88> <209,209> \n");

  // Remove all but 1 of key-value pairs
  multimap.Remove(13);
  multimap.Remove(41);
  multimap.Remove(209);

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "<88,88> \n");

  // Check size is 1
  EXPECT_EQ(multimap.Size(), 1);

  // Remove final key-value pair
  multimap.Remove(88);

  // Check printing of key-value pairs
  testing::internal::CaptureStdout();
  multimap.Print();
  EXPECT_EQ(testing::internal::GetCapturedStdout(), "\n");
}

// 10) Check repetitive insertion & removal: insert, remove, size, contains
TEST(Multimap, RepInsertRemoval) {
  Multimap<int, int> multimap;
  std::vector<int> keys{1, 3, 2, 4, 5};

  // Check size
  EXPECT_EQ(multimap.Size(), 0);

  // Insert & Remove key-value pairs x2
  for (auto i : keys) {
    multimap.Insert(i, i);
    multimap.Remove(i);
    multimap.Insert(i, i);
    multimap.Remove(i);
  }

  // Check size still 0
  EXPECT_EQ(multimap.Size(), 0);

  // Check get exception
  EXPECT_THROW(multimap.Get(1), std::exception);
  EXPECT_THROW(multimap.Get(5), std::exception);

  // Check contains elements
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Contains(1), false);
  EXPECT_EQ(multimap.Contains(5), false);

  // Insert key-value pairs x2
  for (auto i : keys) {
    multimap.Insert(i, i);
    multimap.Insert(i, i);
  }

  // Check contains elements
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Contains(1), true);
  EXPECT_EQ(multimap.Contains(5), true);
  EXPECT_EQ(multimap.Contains(12), false);

  // Check size
  EXPECT_EQ(multimap.Size(), 10);

  // Remove key-value pairs x2
  for (auto i : keys) {
    multimap.Remove(i);
    multimap.Remove(i);
  }

  // Check size
  EXPECT_EQ(multimap.Size(), 0);

  // Check contains elements
  std::random_shuffle(keys.begin(), keys.end());
  EXPECT_EQ(multimap.Contains(1), false);
  EXPECT_EQ(multimap.Contains(5), false);
  EXPECT_EQ(multimap.Contains(12), false);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
