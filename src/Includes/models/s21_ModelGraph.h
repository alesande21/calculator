//
// Created by Alesander Britni on 5/26/23.
//

#ifndef CPP3_SMARTCALC_V2_0_1_S21_MODELGRAPH_H
#define CPP3_SMARTCALC_V2_0_1_S21_MODELGRAPH_H

#include "../controllers/s21_ControllerPolish.h"

namespace s21 {
class GraphModel {
 public:
  using value_type = std::vector<std::pair<double, double>>;
  using model_type = std::pair<value_type, bool>;

  model_type CreateGraph(double x_begin, double x_end, std::string str,
                         s21::ControllerPolish &controller_polish);

 private:
  std::vector<std::pair<double, double>> coordinates_{};
};
}  // namespace s21

#endif  // CPP3_SMARTCALC_V2_0_1_S21_MODELGRAPH_H
