//
// Created by Alesander Britni on 5/26/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERGRAPH_H
#define CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERGRAPH_H

#include "../models/s21_ModelGraph.h"

namespace s21 {
class ControllerGraph {
 public:
  inline std::pair<std::vector<std::pair<double, double>>, bool> CreateGraph(
      double x_begin, double x_end, std::string str,
      s21::ControllerPolish &controller_polish) {
    return model_.CreateGraph(x_begin, x_end, str, controller_polish);
  }

 private:
  s21::GraphModel model_;
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_CONTROLLERGRAPH_H
