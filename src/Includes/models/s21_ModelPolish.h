//
// Created by Alesander Britni on 5/8/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_MODELPOLISH_H
#define CPP3_SMARTCALC_V2_0_1_S21_MODELPOLISH_H

#include <algorithm>
#include <bitset>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {
class PolishModel {
 private:
  struct ElementOfExpression;

 public:
  using oper = std::function<double(double, double)>;
  using func = std::function<double(double)>;
  using key_type = std::string;
  using oper_type = std::map<key_type, oper>;
  using func_type = std::map<key_type, func>;
  using prior_type = std::map<key_type, int>;

  using res_type = std::pair<double, bool>;
  using model_type = std::pair<key_type, res_type>;
  using struct_type = ElementOfExpression;
  using list_type = std::list<struct_type>;
  using iterator = list_type::iterator;

  model_type СalculateExpression(key_type str, key_type str_x);

 private:
  struct ElementOfExpression {
    double value_{};
    int order_of_priority_{};
    key_type action_{};
    key_type type_of_action_{};

    ElementOfExpression(){};

    ElementOfExpression(double value_, int order_of_priority_, key_type action_,
                        key_type type_of_action_)
        : value_(value_),
          order_of_priority_(order_of_priority_),
          action_(std::move(action_)),
          type_of_action_(std::move(type_of_action_)) {}
  };

  oper_type operator_{{"+", std::plus<double>()},
                      {"-", std::minus<double>()},
                      {"/", std::divides<double>()},
                      {"*", std::multiplies<double>()},
                      {"mod", fmodl},
                      {"^", powl}};

  func_type function_{{"cos", cosl},   {"sin", sinl},   {"tan", tanl},
                      {"asin", asinl}, {"acos", acosl}, {"atan", atanl},
                      {"sqrt", sqrtl}, {"ln", logl},    {"log", log10l}};

  prior_type prioritet_{{"(", 0},    {")", 0},    {"+", 1},    {"-", 1},
                        {"/", 3},    {"*", 3},    {"mod", 2},  {"^", 4},
                        {"cos", 5},  {"sin", 5},  {"tan", 5},  {"asin", 5},
                        {"acos", 5}, {"atan", 5}, {"sqrt", 5}, {"ln", 5},
                        {"log", 5}};

  list_type head_{};
  list_type head_reserve_{};
  list_type head_ready_{};
  bool status_error_ = true;
  double value_x_{};
  int numb_of_brackets_{};
  int pow_open_{};

  void Clear();
  key_type CreateResult();
  void CheckString(key_type &str, key_type &str_x);
  key_type PrevAction();
  void CreateRpn();
  void IsBreckets(iterator &it);
  void IsOperator(iterator &it);
  void IsFunction(iterator &it);
  void CheckIsNum(key_type str, key_type::iterator &it);
  void CheckIsOperator(key_type str, key_type::iterator &it);
  void CheckIsUnar(key_type::iterator it);
  void CheckIsBreckets(key_type::iterator &it);
  void CheckIsFunction(key_type str, key_type::iterator &it);
  void CheckIsX(key_type str_x);
  void Calculate();
  bool CheckFirtstLetter(key_type::iterator it);
  inline bool MustBeforePushOperator() {
    return (CheckBeforePushOper1() || CheckBeforePushOper2() ||
            CheckBeforePushOper3() || CheckBeforePushOper4());
  }
  inline bool CheckBeforePushOper1() {
    return head_reserve_.empty() ||
           head_reserve_.back().order_of_priority_ <
               head_.front().order_of_priority_ ||
           head_.front().action_ == "(";
  }
  inline bool CheckBeforePushOper2() {
    return head_reserve_.back().order_of_priority_ ==
               head_.front().order_of_priority_ &&
           head_.front().action_ == "^";
  }
  inline bool CheckBeforePushOper3() {
    return pow_open_ > 0 &&
           function_.find(head_.front().action_) != function_.end();
  }
  inline bool CheckBeforePushOper4() {
    return head_.front().action_ == "(" && head_reserve_.back().action_ == "^";
  }
  inline bool MustBeforeNum() {
    return (PrevAction().empty() || PrevAction() == "B_CLOSE" ||
            PrevAction() == "B_OPEN" || PrevAction() == "OPERATOR");
  }
  inline bool MustBeforeBOpen() {
    return (PrevAction().empty() || PrevAction() == "B_OPEN" ||
            PrevAction() == "OPERATOR" || PrevAction() == "FUNCTION");
  }
  inline bool MustBeforeFun() {
    return (PrevAction().empty() || PrevAction() == "B_OPEN" ||
            PrevAction() == "OPERATOR");
  }
  inline bool MustBeforeBClose() {
    return (PrevAction() == "OPERAND" || PrevAction() == "B_CLOSE");
  }
  inline bool MustBeforeOper() {
    return (PrevAction() == "OPERAND" || PrevAction() == "B_CLOSE");
  }
  inline bool MustBeforeUnar() {
    return (PrevAction().empty() || PrevAction() == "B_OPEN");
  }
  inline bool LastAction() {
    return head_.back().action_ == "NUMBER" || head_.back().action_ == ")";
  }
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_MODELPOLISH_H
