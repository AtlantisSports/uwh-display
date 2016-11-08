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

TEST(CanvasTest, ForEach) {
  UWHDCanvas *C = UWHDCanvas::create(3, 3);
  C->forEach([&](unsigned X, unsigned Y) {
    C->at(X, Y).r = X + Y * Y;
  });

  EXPECT_EQ(C->at(0, 0).r, 0);
  EXPECT_EQ(C->at(0, 1).r, 1);
  EXPECT_EQ(C->at(0, 2).r, 4);

  EXPECT_EQ(C->at(1, 0).r, 1);
  EXPECT_EQ(C->at(1, 1).r, 2);
  EXPECT_EQ(C->at(1, 2).r, 5);

  EXPECT_EQ(C->at(2, 0).r, 2);
  EXPECT_EQ(C->at(2, 1).r, 3);
  EXPECT_EQ(C->at(2, 2).r, 6);

  free(C);
}

TEST(CanvasTest, Fill) {
  UWHDCanvas *C = UWHDCanvas::create(2, 2);

  UWHDPixel V;
  V.r = 1;
  V.g = 2;
  V.b = 3;

  C->fill(V);

  EXPECT_EQ(C->at(0, 0).r, 1);
  EXPECT_EQ(C->at(0, 0).g, 2);
  EXPECT_EQ(C->at(0, 0).b, 3);

  EXPECT_EQ(C->at(0, 0), V);
  EXPECT_EQ(C->at(0, 1), V);

  EXPECT_EQ(C->at(1, 0), V);
  EXPECT_EQ(C->at(1, 1), V);

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
