#include "uwhd/canvas/Canvas.h"
#include "uwhd/canvas/CanvasViewer.h"
#include "uwhd/canvas/PPMCanvasViewer.h"

#include "gtest/gtest.h"

#include <fstream>

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

TEST(CanvasTest, PPMOutput) {
  UWHDCanvas *C = UWHDCanvas::create(2, 3);

  C->forEach([&](unsigned X, unsigned Y) {
    C->at(X, Y).r = X;
    C->at(X, Y).g = Y;
    C->at(X, Y).b = 0;
  });

  const std::string Reference =
    "P6\n"
    "2 3\n"
    "2\n"
    "0 0 0  1 0 0\n"
    "0 1 0  1 1 0\n"
    "0 2 0  1 2 0\n";

  EXPECT_EQ(asPPMString(C), Reference);

  // Now try Round-tripping through a temp file
  char FileName[] = "/tmp/uwhdtemp.XXXXXX";
  int fd = mkstemp(FileName);
  ASSERT_NE(fd, -1)
    << "Failed to create temp file";

  UWHDCanvasViewer *PPMV = createPPMCanvasViewer(FileName);
  PPMV->show(C);
  free(PPMV);
  close(fd);

  std::ifstream OF(FileName);
  std::string OS((std::istreambuf_iterator<char>(OF)),
                  std::istreambuf_iterator<char>());
  EXPECT_EQ(OS, Reference)
    << FileName;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
