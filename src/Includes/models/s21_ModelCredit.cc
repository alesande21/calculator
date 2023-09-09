//
// Created by Alesander Britni on 5/29/23.
//

#include "s21_ModelCredit.h"

s21::CreditModel::model_type s21::CreditModel::CalculateCredit(
    long double loan_amount, long int loan_term, double interest_rate,
    int type) {
  if (loan_amount <= 0 || loan_term <= 0 || interest_rate <= 0) {
    return {ConstObj(), false};
  }
  InitField(loan_amount, loan_term, interest_rate);
  if (type) {
    AnnuityPayments();
  } else {
    DifferentiatedPayments();
  }
  return {ConstObj(), status_error_};
}

s21::CreditModel::const_value_type s21::CreditModel::ConstObj() {
  s21::CreditModel::const_value_type const_head;
  for (const auto& entry : head_) {
    const_head.insert({entry.first, entry.second});
  }
  return const_head;
}

void s21::CreditModel::AnnuityPayments() {
  Components temp;
  temp.month_payment_ = kef_annuity_ * loan_amount_;
  head_.emplace(ymdl_ + months{loan_term_ + 1}, Components());
  for (; loan_term_ > 0; --loan_term_) {
    Components tm;
    tm.month_payment_ = temp.month_payment_;
    tm.interest_payment_ = loan_amount_ * month_rate_;
    tm.principal_amount_payment_ = temp.month_payment_ - tm.interest_payment_;
    tm.debt_balance_ = loan_amount_ =
        loan_amount_ - tm.principal_amount_payment_;
    auto it = head_.emplace(ymdl_, tm);
    SumComponents(it.first);
    ymdl_ = ymdl_ + months{1};
  }
}

void s21::CreditModel::DifferentiatedPayments() {
  Components temp;
  temp.principal_amount_payment_ = loan_amount_ / loan_term_;
  head_.emplace(ymdl_ + months{loan_term_ + 1}, Components());
  for (; loan_term_ > 0; --loan_term_) {
    Components tm;
    tm.interest_payment_ =
        (loan_amount_ * interest_rate_ * ymdl_.day().operator unsigned int()) /
        DaysInYear();
    tm.principal_amount_payment_ = temp.principal_amount_payment_;
    tm.debt_balance_ = loan_amount_ =
        loan_amount_ - tm.principal_amount_payment_;
    tm.month_payment_ = temp.principal_amount_payment_ + tm.interest_payment_;
    auto it = head_.emplace(ymdl_, tm);
    SumComponents(it.first);
    ymdl_ = ymdl_ + months{1};
  }
}

unsigned int s21::CreditModel::DaysInYear() {
  auto ym = date::year_month_day{ymdl_.year(), date::month{1}, date::day{1}};
  auto startOfYear = date::sys_days(ym.year() / date::jan / date::day{1});
  return (date::sys_days(ym + date::years{1}) - startOfYear).count();
}

void s21::CreditModel::SumComponents(s21::CreditModel::iterator it) {
  auto it_last = --head_.end();
  it_last->second.month_payment_ =
      it_last->second.month_payment_ + it->second.month_payment_;
  it_last->second.principal_amount_payment_ =
      it_last->second.principal_amount_payment_ +
      it->second.principal_amount_payment_;
  it_last->second.interest_payment_ =
      it_last->second.interest_payment_ + it->second.interest_payment_;
}

void s21::CreditModel::InitField(long double loan_amount, size_t loan_term,
                                 double interest_rate) {
  loan_amount_ = loan_amount;
  loan_term_ = loan_term;
  interest_rate_ = interest_rate / 100;
  month_rate_ = interest_rate_ / 12;
  kef_annuity_ = (month_rate_ * powl((1 + month_rate_), loan_term_)) /
                 (powl((1 + month_rate_), loan_term_) - 1);
  head_.clear();
  ymdl_ = GetCurrentDate();
}

s21::CreditModel::key_type s21::CreditModel::GetCurrentDate() {
  auto initial_date = date::year_month_day{
      date::floor<date::days>(std::chrono::system_clock::now())};
  key_type ymdl = initial_date.month() / last / initial_date.year();
  return ymdl;
}