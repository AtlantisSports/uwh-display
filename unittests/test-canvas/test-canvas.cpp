#include "uwhd/canvas/Canvas.h"

#include "gtest/gtest.h"

TEST(CanvasTest, Allocation) {
  unsigned h = 5;
  unsigned w = 7;
  UWHDCanvas *C = UWHDCanvas::create(w, h);
  EXPECT_EQ(C->w, w);
  EXPECT_EQ(C->h, h);
  free(C);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
