//
// Created by Alesander Britni on 5/24/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERCREDIT_H
#define CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERCREDIT_H

#include "../models/s21_ModelCredit.h"

namespace s21 {
class ControllerCredit {
 public:
  using controller_type = s21::CreditModel::model_type;

  inline controller_type CalculateCredit(long double loan_amount,
                                         long int loan_term,
                                         double interest_rate, int type) {
    return model_.CalculateCredit(loan_amount, loan_term, interest_rate, type);
  }

 private:
  s21::CreditModel model_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERCREDIT_H
