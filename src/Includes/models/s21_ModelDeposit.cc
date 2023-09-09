//
// Created by Alesander Britni on 5/29/23.
//

#include "s21_ModelDeposit.h"

s21::DepositModel::model_type s21::DepositModel::CalculateDeposit(
    double deposit_amount, int deposit_period, int type_period,
    double deposit_interest_rate, double tax_rate, int payment_frequency,
    int cap_of_interest) {
  if (CheckError(deposit_amount, deposit_period, deposit_interest_rate,
                 tax_rate)) {
    return {ConstObj(), false};
  }

  InitField(deposit_amount, deposit_period, type_period, deposit_interest_rate,
            tax_rate, payment_frequency);
  Components temp;
  head_.emplace(ymd_end_ + months{deposit_period + 1}, Components());
  for (; ymdl_ < ymd_end_;) {
    auto dif = NextDate();
    temp.payment_of_interest_ =
        deposit_amount_ * deposit_interest_rate_ / DaysInYear() * dif.second;
    temp.deposit_amount_ = deposit_amount_ =
        deposit_amount_ + temp.payment_of_interest_ * cap_of_interest;
    auto it = head_.emplace(ymdl_, temp);
    CalculateTax(dif);
    SumComponents(it.first);
  }

  return {ConstObj(), status_error_};
}

s21::DepositModel::const_value_type s21::DepositModel::ConstObj() {
  s21::DepositModel::const_value_type const_head;
  for (const auto& entry : head_) {
    const_head.insert({entry.first, entry.second});
  }
  return const_head;
}

bool s21::DepositModel::CheckError(double deposit_amount, int deposit_period,
                                   double deposit_interest_rate,
                                   double tax_rate) {
  return (deposit_amount <= 0 || deposit_period <= 0 ||
          deposit_interest_rate <= 0 || tax_rate <= 0);
}

s21::DepositModel::key_type s21::DepositModel::GetCurrentDate() {
  auto initial_date = date::year_month_day{
      date::floor<date::days>(std::chrono::system_clock::now())};
  key_type ymdl = initial_date;
  return ymdl;
}

s21::DepositModel::next_date_type s21::DepositModel::NextDate() {
  key_type dif = ymdl_;
  if (payment_frequency_ == 0) {
    ymdl_ = sys_days{ymdl_} + date::days{1};
  } else if (payment_frequency_ == 1) {
    ymdl_ = sys_days{ymdl_} + date::days{7};
  } else if (payment_frequency_ == 2) {
    ymdl_ = ymdl_ + date::months{1};
  } else if (payment_frequency_ == 3) {
    ymdl_ = ymdl_ + date::months{3};
  } else if (payment_frequency_ == 4) {
    ymdl_ = ymdl_ + date::months{6};
  } else if (payment_frequency_ == 5) {
    ymdl_ = ymdl_ + date::years{1};
  } else if (payment_frequency_ == 6) {
    ymdl_ = ymd_end_;
  }
  return {dif, (sys_days{ymdl_} - sys_days{dif}).count()};
}

void s21::DepositModel::CalculateTax(s21::DepositModel::next_date_type dif) {
  if (dif.first.year() == ymdl_.year() && ymdl_ != ymd_end_) {
    revenue_total_ =
        revenue_total_ + std::prev(head_.end(), 2)->second.payment_of_interest_;
    return;
  }
  auto startOfYear = date::sys_days(ymdl_.year() / date::jan / date::day{1});
  unsigned int date_difference =
      (startOfYear - date::sys_days(dif.first)).count() * (ymdl_ != ymd_end_);
  revenue_total_ =
      revenue_total_ + std::prev(head_.end(), 2)->second.payment_of_interest_ /
                           dif.second * date_difference;
  if (revenue_total_ > taxable_base_) {
    std::prev(head_.end(), 2)->second.tax_amount_ =
        (revenue_total_ - taxable_base_) * 0.13;
    revenue_total_ = std::prev(head_.end(), 2)->second.payment_of_interest_ /
                     dif.second * (dif.second - date_difference);
  } else {
    revenue_total_ = std::prev(head_.end(), 2)->second.payment_of_interest_ /
                     dif.second * (dif.second - date_difference);
  }
}

s21::DepositModel::key_type s21::DepositModel::InitField(
    double deposit_amount, int deposit_period, int type_period,
    double deposit_interest_rate, double tax_rate, int payment_frequency) {
  deposit_amount_ = deposit_amount;
  payment_frequency_ = payment_frequency;
  deposit_interest_rate_ = deposit_interest_rate / 100;
  taxable_base_ = 1000000 * tax_rate / 100;
  ymdl_ = GetCurrentDate();
  head_.clear();
  revenue_total_ = 0;
  return EndPeriod(deposit_period, type_period);
}

s21::DepositModel::key_type s21::DepositModel::EndPeriod(int deposit_period,
                                                         int type_period) {
  ymd_end_ = ymdl_;
  if (type_period == 0) {
    ymd_end_ = sys_days{ymd_end_} + date::days{deposit_period};
  } else if (type_period == 1) {
    ymd_end_ += date::months{deposit_period};
  } else if (type_period == 2) {
    ymd_end_ += date::years{deposit_period};
  }
  return ymd_end_;
}

unsigned int s21::DepositModel::DaysInYear() {
  auto ym = date::year_month_day{ymdl_.year(), date::month{1}, date::day{1}};
  auto startOfYear = date::sys_days(ym.year() / date::jan / date::day{1});
  return (date::sys_days(ym + date::years{1}) - startOfYear).count();
}

void s21::DepositModel::SumComponents(s21::DepositModel::iterator it) {
  auto it_last = --head_.end();
  it_last->second.payment_of_interest_ =
      it_last->second.payment_of_interest_ + it->second.payment_of_interest_;
  it_last->second.tax_amount_ =
      it_last->second.tax_amount_ + it->second.tax_amount_;
  it_last->second.deposit_amount_ = it->second.deposit_amount_;
}