#include "../Includes/controllers/s21_ControllerCredit.h"
#include "../Includes/controllers/s21_ControllerDeposit.h"
#include "../Includes/controllers/s21_ControllerGraph.h"
#include "../Includes/controllers/s21_ControllerPolish.h"
#include "gtest/gtest.h"

TEST(test_class, polish_1) {
  s21::ControllerPolish model;
  std::string str = "21+9-10";
  std::string str_x = "0.0";
  double ref = 21 + 9 - 10;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_2) {
  s21::ControllerPolish model;
  std::string str = "2^3^2";
  std::string str_x = "0.0";
  double ref = powl(2, powl(3, 2));
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_3) {
  s21::ControllerPolish model;
  std::string str = "2^3^sin(1+1)+2";
  std::string str_x = "0.0";
  double ref = powl(2, powl(3, sinl(1 + 1))) + 2;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_4) {
  s21::ControllerPolish model;
  std::string str = "(2+(sin(1+1)^3^sin(1+1))+2)";
  std::string str_x = "0.0";
  double ref = (2 + (powl(sinl(1 + 1), powl(3, sinl(1 + 1)))) + 2);
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_5) {
  s21::ControllerPolish model;
  std::string str = "-(1+2)";
  std::string str_x = "0.0";
  double ref = -(1 + 2);
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_6) {
  s21::ControllerPolish model;
  std::string str = "+10";
  std::string str_x = "0.0";
  double ref = +10;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_7) {
  s21::ControllerPolish model;
  std::string str = "2*+3";
  std::string str_x = "0.0";
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_EQ(result.first, "Error");
  EXPECT_EQ(result.second.second, false);
}

TEST(test_class, polish_8) {
  s21::ControllerPolish model;
  std::string str = "2.3.+5";
  std::string str_x = "0.0";
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_EQ(result.first, "Error");
  EXPECT_EQ(result.second.second, false);
}

TEST(test_class, polish_9) {
  s21::ControllerPolish model;
  std::string str =
      "-cos(13*2-61.13/2)+acos(0.5)*sin(1)-ln(23+12)/(13-11)+log(2^2^2)";
  std::string str_x = "0.0";
  double ref = -cosl(13 * 2 - 61.13 / 2) + acosl(0.5) * sinl(1) -
               logl(23 + 12) / (13 - 11) + log10l(powl(2, powl(2, 2)));
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_10) {
  s21::ControllerPolish model;
  std::string str =
      "sin(cos(111))+tan(sqrt(222))-acos(asin(333))*atan(444)/ln(log(555))";
  std::string str_x = "0.0";
  double ref = sinl(cosl(111)) + tanl(sqrtl(222)) -
               acosl(asinl(333)) * atanl(444) / logl(log10l(555));
  auto result = model.СalculateExpression(str, str_x);
  ASSERT_TRUE(std::isnan(result.second.first));
  ASSERT_TRUE(std::isnan(ref));
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_11) {
  s21::ControllerPolish model;
  std::string str = "2/1+2+3+4*5*6^7-727mod728+((1000+500*2)*201)+sin(1)";
  std::string str_x = "0.0";
  double ref = 2 / 1 + 2 + 3 + 4 * 5 * powl(6, 7) - 727 % 728 +
               ((1000 + 500 * 2) * 201) + sinl(1);
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_12) {
  s21::ControllerPolish model;
  std::string str = "x+2";
  std::string str_x = "25";
  double ref = 27;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_13) {
  s21::ControllerPolish model;
  std::string str = "x+2/1+2+3+4*5*6^7-727";
  std::string str_x = "2";
  double ref = 2 + 2 / 1 + 2 + 3 + 4 * 5 * powl(6, 7) - 727;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_14) {
  s21::ControllerPolish model;
  std::string str = "x+2/1+2+3+x*x";
  std::string str_x = "5.0";
  double ref = 5 + 2 / 1 + 2 + 3 + 5 * 5;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_15) {
  s21::ControllerPolish model;
  std::string str = "x";
  std::string str_x = "-2";
  double ref = -2;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_16) {
  s21::ControllerPolish model;
  std::string str = "x+2/1+2+3+x*x";
  std::string str_x = "5.0";
  double ref = 5 + 2 / 1 + 2 + 3 + 5 * 5;
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
  str = "x";
  str_x = "-2";
  ref = -2;
  result = model.СalculateExpression(str, str_x);
  EXPECT_DOUBLE_EQ(result.second.first, ref);
  EXPECT_EQ(result.second.second, true);
}

TEST(test_class, polish_x) {
  s21::ControllerPolish model;
  std::string str = "x+x/2";
  std::string str_x = "a";
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_EQ(result.first, "Error");
  EXPECT_EQ(result.second.second, false);
}

TEST(test_class, polish_x_2) {
  s21::ControllerPolish model;
  std::string str = "";
  std::string str_x = "0.0";
  auto result = model.СalculateExpression(str, str_x);
  EXPECT_EQ(result.first, "Error");
  EXPECT_EQ(result.second.second, false);
}

TEST(test_class, credit_01) {
  s21::ControllerCredit model;
  auto result = model.CalculateCredit(1000000, 18, 8, 1);
  EXPECT_EQ(result.second, true);
  auto it = --result.first.end();
  ASSERT_NEAR(it->second.month_payment_, 1064525.40, 0.5);
  ASSERT_NEAR(it->second.interest_payment_, 64525.40, 0.5);
  ASSERT_NEAR(std::prev(it, 1)->second.month_payment_, 59140.30, 0.5);
}

TEST(test_class, credit_02) {
  s21::ControllerCredit model;
  auto result = model.CalculateCredit(1000000, 18, 8, 0);
  EXPECT_EQ(result.second, true);
  auto it = --result.first.end();
  ASSERT_NEAR(it->second.month_payment_, 1063458.31, 0.5);
  ASSERT_NEAR(it->second.interest_payment_, 63458.31, 0.5);
  ASSERT_NEAR(std::prev(it, 1)->second.month_payment_, 55919.85, 0.5);
}

TEST(test_class, deposit_01) {
  s21::ControllerDeposit model;
  auto result = model.CalculateDeposit(1000000, 58, 1, 8, 7.5, 2, 1);
  EXPECT_EQ(result.second, true);
  auto it = --result.first.end();
  ASSERT_NEAR(it->second.payment_of_interest_, 470363.02, 30.5);
  ASSERT_NEAR(it->second.deposit_amount_, 1470363.91, 30.5);
}

TEST(test_class, deposit_02) {
  s21::ControllerDeposit model;
  auto result = model.CalculateDeposit(1000000, 50, 2, 8, 7.5, 0, 0);
  EXPECT_EQ(result.second, true);
  auto it = --result.first.end();
  ASSERT_NEAR(it->second.payment_of_interest_, 4000029.54, 30);
  ASSERT_NEAR(it->second.deposit_amount_, 1000000.00, 0);
}

TEST(test_class, deposit_03) {
  s21::ControllerDeposit model;
  auto result = model.CalculateDeposit(1000000, 50, 2, -1, 7.5, 0, 0);
  EXPECT_EQ(result.second, false);
}

TEST(test_class, graph) {
  s21::ControllerGraph model;
  s21::ControllerPolish polish;
  auto result = model.CreateGraph(-4, 4, "sin(x)", polish);
  EXPECT_EQ(result.second, true);
}

TEST(test_class, graph_02) {
  s21::ControllerGraph model;
  s21::ControllerPolish polish;
  auto result = model.CreateGraph(4, 4, "sin(x)", polish);
  EXPECT_EQ(result.second, false);
}

int main() {
  testing::InitGoogleTest();
  if (RUN_ALL_TESTS()) {
    std::cout << "Some tests have failed :(\n"
              << "Get back to work!" << std::endl;
  }
}
