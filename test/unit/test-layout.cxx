#include "gtest/gtest.h"
#include "RAJA/RAJA.hxx"

TEST(LayoutTest, 1D)
{
  RAJA_INDEX_VALUE(TestIndex, "TestIndex");
  using layout = RAJA::OffsetLayout<int, RAJA::PERM_I, TestIndex>;

  /*
   * Construct a 1D view with  with the following indices:
   *
   * 10, 11, 12, 13, 14
   */
  const layout l({10}, {14});

  /*
   * First element, 10, should have index 0.
   */
  ASSERT_EQ(0, l(TestIndex(10)));

  ASSERT_EQ(2, l(TestIndex(12)));

  /*
   * Last element, 14, should have index 5.
   */
  ASSERT_EQ(4, l(TestIndex(14)));
}

TEST(LayoutTest, OffsetVsRegular)
{
  typedef RAJA::OffsetLayout<int, RAJA::PERM_JI, int, int> layout_off;
  typedef RAJA::Layout<int, RAJA::PERM_JI, int, int> layout_reg;

  const layout_off offset({0,0}, {5,5});
  const layout_reg layout(6,6);

  /*
   * OffsetLayout with 0 offset should function like the regular Layout.
   */
  for (int j = 0; j < 6; ++j) {
    for (int i = 0; i < 6; ++i) {
      ASSERT_EQ(offset(i,j), layout(i,j));
    }
  }
}

TEST(LayoutTest, 2D_IJ)
{
  typedef RAJA::OffsetLayout<int, RAJA::PERM_IJ, int, int> my_layout;

  /*
   * Construct a 2D layout:
   *
   * (-1, 0), (0, 0), (1, 0)
   * (-1, -1), (0, -1), (1, -1)
   * (-1, -2), (0, -2), (1, -2)
   */
  const my_layout layout({-1,-2}, {1,0});

  /*
   * First element, (-1, -2), should have index 0.
   */
  ASSERT_EQ(0, layout(-1, -2));

  /*
   * (0, -2) should be the second element??
   */
  ASSERT_EQ(1, layout(0, -2));

  /*
   * Last element, (1, 0), should have index 8.
   */
  ASSERT_EQ(8, layout(1,0));
}

TEST(LayoutTest, 2D_JI)
{
  typedef RAJA::OffsetLayout<int, RAJA::PERM_JI, int, int> my_layout;

  /*
   * Construct a 2D layout:
   *
   * (-1, 0), (0, 0), (1, 0)
   * (-1, -1), (0, -1), (1, -1)
   * (-1, -2), (0, -2), (1, -2)
   */
  const my_layout layout({-1,-2}, {1,0});

  /*
   * First element, (-1, -2), should have index 0.
   * things.
   */
  ASSERT_EQ(0, layout(-1, -2));

  ASSERT_EQ(3, layout(-1, -1));

  /*
   * Last element, (1, 0), should have index 8.
   */
  ASSERT_EQ(8, layout(1,0));
}

TEST(LayoutTest, View)
{
  int* data = new int[10];

  using layout = RAJA::OffsetLayout<int, RAJA::PERM_I, int>;

  RAJA::View<int, layout> view(data, layout({1}, {10}));

  for (int i = 0; i < 10; i++) {
    data[i] = i;
  }

  ASSERT_EQ(data[0], view(1));
  ASSERT_EQ(data[9], view(10));
}
