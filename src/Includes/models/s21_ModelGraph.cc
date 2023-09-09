//
// Created by Alesander Britni on 5/29/23.
//

#include "s21_ModelGraph.h"

s21::GraphModel::model_type s21::GraphModel::CreateGraph(
    double x_begin, double x_end, std::string str,
    s21::ControllerPolish &controller_polish) {
  if (x_begin == x_end) {
    return {coordinates_, false};
  }
  coordinates_.clear();
  std::pair<std::string, std::pair<double, bool>> result{};
  long double step = (x_end - x_begin) / 3000;
  for (double x = x_begin; x <= x_end; x += step) {
    result = controller_polish.СalculateExpression(str, std::to_string(x));
    if (result.second.second) {
      coordinates_.push_back({x, result.second.first});
    } else {
      x = x_end;
    }
  }

  return {coordinates_, result.second.second};
}