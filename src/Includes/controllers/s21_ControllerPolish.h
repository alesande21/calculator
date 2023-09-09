//
// Created by Alesander Britni on 5/17/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERPOLISH_H
#define CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERPOLISH_H

#include "../models/s21_ModelPolish.h"

namespace s21 {
class ControllerPolish {
 public:
  using controller_type = s21::PolishModel::model_type;

  inline controller_type СalculateExpression(std::string str,
                                             std::string str_x) {
    return model_.СalculateExpression(str, str_x);
  }

 private:
  s21::PolishModel model_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERPOLISH_H
