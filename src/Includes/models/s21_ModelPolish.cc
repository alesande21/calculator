//
// Created by Alesander Britni on 5/30/23.
//

#include "s21_ModelPolish.h"

s21::PolishModel::model_type s21::PolishModel::СalculateExpression(
    key_type str, key_type str_x) {
  Clear();
  status_error_ = !str.empty() && !str_x.empty();
  CheckString(str, str_x);
  CreateRpn();
  Calculate();

  return {CreateResult(), {value_x_, status_error_}};
}

void s21::PolishModel::Clear() {
  head_.clear();
  head_reserve_.clear();
  head_ready_.clear();
  status_error_ = true;
  value_x_ = 0;
  numb_of_brackets_ = 0;
  pow_open_ = 0;
}

s21::PolishModel::key_type s21::PolishModel::CreateResult() {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(7) << value_x_;

  std::string result = oss.str();
  result.erase(result.find_last_not_of('0') + 1, std::string::npos);
  if (result.back() == '.') result.pop_back();

  return status_error_ ? result : "Error";
}

void s21::PolishModel::CheckString(key_type &str, key_type &str_x) {
  auto it = str.begin();
  for (; it != str.end() && status_error_; ++it) {
    if (isdigit(*it) && MustBeforeNum()) {
      CheckIsNum(str, it);
    } else if ((operator_.find(std::string(1, *it)) != operator_.end()) ||
               *it == 'm') {
      CheckIsOperator(str, it);
    } else if (*it == '(' || *it == ')') {
      CheckIsBreckets(it);
    } else if (CheckFirtstLetter(it) && MustBeforeFun()) {
      CheckIsFunction(str, it);
    } else if (*it == 'x' && MustBeforeNum()) {
      CheckIsX(str_x);
    } else {
      status_error_ = false;
    }
  }
  status_error_ = numb_of_brackets_ == 0 && status_error_ && LastAction();
}

s21::PolishModel::key_type s21::PolishModel::PrevAction() {
  if (!head_.empty() && !head_.back().type_of_action_.empty()) {
    return head_.back().type_of_action_;
  }
  return "";
}

void s21::PolishModel::CreateRpn() {
  if (!status_error_) {
    return;
  }
  auto it = head_.begin();
  for (; it != head_.end() && status_error_;) {
    if (it->action_ == "NUMBER") {
      ++it;
      head_ready_.splice(head_ready_.end(), head_, head_.begin());
    } else if (it->action_ == ")") {  // it->action_ == "(" ||
      IsBreckets(it);
    } else if (operator_.find(it->action_) != operator_.end() ||
               it->action_ == "(") {
      IsOperator(it);
    } else if (function_.find(it->action_) != function_.end()) {
      IsFunction(it);
    } else {
      status_error_ = false;
    }
  }
  if (!head_reserve_.empty()) {
    head_reserve_.reverse();
    head_ready_.splice(head_ready_.end(), head_reserve_);
  }
  status_error_ = head_.empty() && head_reserve_.empty();
  head_ = std::move(head_ready_);
}

void s21::PolishModel::IsBreckets(iterator &it) {
  if (it->action_ == ")") {
    auto it_reserve = --head_reserve_.end();
    while (it_reserve->action_ != "(") {
      --it_reserve;
      head_ready_.splice(head_ready_.end(), head_reserve_,
                         --(head_reserve_.end()));
    }
    if (it->action_ == "(" && pow_open_ > 0) {
      numb_of_brackets_--;
    }
    ++it;
    head_.erase(head_.begin());
    head_reserve_.erase(it_reserve);
  }
}

void s21::PolishModel::IsOperator(iterator &it) {
  if (MustBeforePushOperator()) {
    if (it->action_ == "(" && pow_open_ > 0) {
      numb_of_brackets_++;
    }
    if (it->action_ == "^") {
      pow_open_++;
    }
    ++it;
    head_reserve_.splice(head_reserve_.end(), head_, head_.begin());
  } else {
    if (pow_open_ > 0) {
      pow_open_--;
    }
    head_ready_.splice(head_ready_.end(), head_reserve_,
                       --(head_reserve_.end()));
  }
}

void s21::PolishModel::IsFunction(iterator &it) {
  if (CheckBeforePushOper1()) {
    ++it;
    head_reserve_.splice(head_reserve_.end(), head_, head_.begin());
  } else {
    head_ready_.splice(head_ready_.end(), head_reserve_,
                       --(head_reserve_.end()));
  }
}

bool s21::PolishModel::CheckFirtstLetter(key_type::iterator it) {
  auto it_func = function_.begin();
  bool match_found = false;
  for (; it_func != function_.end() && !match_found; ++it_func) {
    if (it_func->first[0] == *it) {
      match_found = true;
    }
  }
  return match_found;
}

void s21::PolishModel::CheckIsNum(key_type str, key_type::iterator &it) {
  std::string str_number;
  size_t shift =
      std::strspn(str.c_str() + std::distance(str.begin(), it), "0123456789.");
  str_number.resize(shift);
  std::copy(it, it + shift, str_number.begin());
  head_.emplace_back(
      ElementOfExpression(stod(str_number), 0, "NUMBER", "OPERAND"));
  std::advance(it, shift - 1);
  status_error_ =
      (str_number.find_first_of(".") == str_number.find_last_of(".")) &&
      (str_number.back() != '.');
}

void s21::PolishModel::CheckIsOperator(key_type str, key_type::iterator &it) {
  if (std::strspn(str.c_str() + std::distance(str.begin(), it), "mod") == 3) {
    head_.emplace_back(ElementOfExpression({}, prioritet_.find("mod")->second,
                                           prioritet_.find("mod")->first,
                                           "OPERATOR"));
    std::advance(it, 2);
  } else if (MustBeforeOper()) {
    head_.emplace_back(ElementOfExpression(
        {}, prioritet_.find(std::string(1, *it))->second,
        prioritet_.find(std::string(1, *it))->first, "OPERATOR"));
  } else if (MustBeforeUnar()) {
    CheckIsUnar(it);
  } else {
    status_error_ = false;
  }
}

void s21::PolishModel::CheckIsUnar(key_type::iterator it) {
  if (*it == '-') {
    head_.emplace_back(ElementOfExpression(-1, 0, "NUMBER", "OPERAND"));
    head_.emplace_back(ElementOfExpression({}, prioritet_.find("*")->second,
                                           prioritet_.find("*")->first,
                                           "OPERATOR"));
  } else if (*it == '+') {
    head_.emplace_back(ElementOfExpression(0, 0, "NUMBER", "OPERAND"));
    head_.emplace_back(ElementOfExpression({}, prioritet_.find("+")->second,
                                           prioritet_.find("+")->first,
                                           "OPERATOR"));
  } else {
    status_error_ = false;
  }
}

void s21::PolishModel::CheckIsBreckets(key_type::iterator &it) {
  if (*it == '(' && MustBeforeBOpen()) {
    head_.emplace_back(ElementOfExpression({}, prioritet_.find("(")->second,
                                           prioritet_.find("(")->first,
                                           "B_OPEN"));
    numb_of_brackets_++;
  } else if (*it == ')' && MustBeforeBClose()) {
    head_.emplace_back(ElementOfExpression({}, prioritet_.find(")")->second,
                                           prioritet_.find(")")->first,
                                           "B_CLOSE"));
    numb_of_brackets_--;
  } else {
    status_error_ = false;
  }
}

void s21::PolishModel::CheckIsFunction(key_type str, key_type::iterator &it) {
  auto it_prior = prioritet_.begin();
  std::advance(it_prior, 7);
  size_t shift = 0;
  while ((shift != it_prior->first.length()) &&
         (it_prior != prioritet_.end())) {
    shift = std::strspn(str.c_str() + std::distance(str.begin(), it),
                        it_prior->first.c_str());
    if (shift == it_prior->first.length()) {
      head_.emplace_back(ElementOfExpression({}, it_prior->second,
                                             it_prior->first, "FUNCTION"));
      std::advance(it, shift - 1);
    } else {
      ++it_prior;
      shift = 0;
    }
  }
  if (it_prior == prioritet_.end()) {
    status_error_ = false;
  }
}

void s21::PolishModel::CheckIsX(key_type str_x) {
  double sign_value_x = -1 * (str_x[0] == '-') + (str_x[0] != '-');
  auto it = str_x.begin();
  if (isdigit(str_x[0])) {
    CheckIsNum(str_x, it);
  } else if (sign_value_x == -1) {
    ++it;
    CheckIsNum(str_x, it);
  }
  head_.back().value_ = head_.back().value_ * sign_value_x;
  value_x_ = head_.back().value_;
}

void s21::PolishModel::Calculate() {
  if (!status_error_) {
    return;
  }
  auto it = head_.begin();
  for (; it != head_.end();) {
    if (it->action_ == "NUMBER") {
      ++it;
    } else if (operator_.find(it->action_) != operator_.end()) {
      std::prev((it)++, 2)->value_ = operator_[it->action_](
          std::prev(it, 2)->value_, std::prev(it, 1)->value_);
      head_.erase(std::prev(it, 2), it);
    } else if (function_.find(it->action_) != function_.end()) {
      std::prev((it)++, 1)->value_ =
          function_[it->action_](std::prev(it, 1)->value_);
      head_.erase(std::prev(it, 1));
    }
  }
  value_x_ = head_.front().value_;
}