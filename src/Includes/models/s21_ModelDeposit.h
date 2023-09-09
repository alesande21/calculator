//
// Created by Alesander Britni on 5/24/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_MODELDEPOSIT_H
#define CPP3_SMARTCALC_V2_0_1_S21_MODELDEPOSIT_H

#include <cmath>
#include <iostream>
#include <map>

#include "../date/date.h"
#include "../date/tz.h"

using namespace date;

namespace s21 {
class DepositModel {
 public:
  struct Components {
    double payment_of_interest_{};
    double tax_amount_{};
    double deposit_amount_{};

    Components(){};

    Components(double payment_of_interest_, double tax_amount_,
               double deposit_amount_)
        : payment_of_interest_(payment_of_interest_),
          tax_amount_(tax_amount_),
          deposit_amount_(deposit_amount_) {}
  };

  using key_type = date::year_month_day;
  using mapped_type = Components;
  using value_type = std::map<key_type, mapped_type>;
  using const_value_type = std::map<key_type, const mapped_type>;
  using iterator = value_type::iterator;
  using model_type = std::pair<const_value_type, bool>;
  using next_date_type = std::pair<key_type, unsigned int>;

  model_type CalculateDeposit(double deposit_amount, int deposit_period,
                              int type_period, double deposit_interest_rate,
                              double tax_rate, int payment_frequency,
                              int cap_of_interest);

 private:
  bool status_error_ = true;
  value_type head_{};
  key_type ymdl_ = GetCurrentDate();
  key_type ymd_end_ = GetCurrentDate();
  double deposit_amount_{};
  double deposit_interest_rate_{};
  double revenue_total_{};
  double taxable_base_{};
  int payment_frequency_{};

  const_value_type ConstObj();
  bool CheckError(double deposit_amount, int deposit_period,
                  double deposit_interest_rate, double tax_rate);
  key_type GetCurrentDate();
  next_date_type NextDate();
  void CalculateTax(next_date_type dif);

  key_type InitField(double deposit_amount, int deposit_period, int type_period,
                     double deposit_interest_rate, double tax_rate,
                     int payment_frequency);
  key_type EndPeriod(int deposit_period, int type_period);
  unsigned int DaysInYear();
  void SumComponents(iterator it);
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_MODELDEPOSIT_H
