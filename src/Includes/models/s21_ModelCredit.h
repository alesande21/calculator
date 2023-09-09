//
// Created by Alesander Britni on 5/20/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_MODELCREDIT_H
#define CPP3_SMARTCALC_V2_0_1_S21_MODELCREDIT_H

#include <cmath>
#include <iostream>
#include <map>

#include "../date/date.h"
#include "../date/tz.h"

using namespace date;

namespace s21 {
class CreditModel {
 public:
  struct Components {
    double month_payment_{};
    double principal_amount_payment_{};
    double interest_payment_{};
    double debt_balance_{};

    Components(){};

    Components(double month_payment_, double principal_amount_payment_,
               double interest_payment_, double debt_balance_)
        : month_payment_(month_payment_),
          principal_amount_payment_(principal_amount_payment_),
          interest_payment_(interest_payment_),
          debt_balance_(debt_balance_) {}
  };

  using key_type = date::year_month_day_last;
  using mapped_type = Components;
  using value_type = std::map<key_type, mapped_type>;
  using const_value_type = std::map<key_type, const mapped_type>;
  using iterator = value_type::iterator;
  using model_type = std::pair<const_value_type, bool>;

  model_type CalculateCredit(long double loan_amount, long int loan_term,
                             double interest_rate, int type);

 private:
  bool status_error_ = true;
  double loan_amount_{};
  long int loan_term_{};
  double interest_rate_{};
  double kef_annuity_{};
  double month_rate_{};
  key_type ymdl_ = GetCurrentDate();
  value_type head_{};

  const_value_type ConstObj();
  void AnnuityPayments();
  void DifferentiatedPayments();
  unsigned int DaysInYear();
  void SumComponents(iterator it);
  void InitField(long double loan_amount, size_t loan_term,
                 double interest_rate);
  key_type GetCurrentDate();
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_MODELCREDIT_H
