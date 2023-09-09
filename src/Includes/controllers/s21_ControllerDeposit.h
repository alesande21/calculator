//
// Created by Alesander Britni on 5/24/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERDEPOSIT_H
#define CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERDEPOSIT_H

#include "../models/s21_ModelDeposit.h"

namespace s21 {
class ControllerDeposit {
 public:
  using controller_type = s21::DepositModel::model_type;

  inline controller_type CalculateDeposit(double deposit_amount,
                                          int deposit_period, int type_period,
                                          double deposit_interest_rate,
                                          double tax_rate,
                                          int payment_frequency,
                                          int cap_of_interest) {
    return model_.CalculateDeposit(deposit_amount, deposit_period, type_period,
                                   deposit_interest_rate, tax_rate,
                                   payment_frequency, cap_of_interest);
  }

 private:
  s21::DepositModel model_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERDEPOSIT_H
