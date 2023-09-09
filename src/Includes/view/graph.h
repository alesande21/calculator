#ifndef GRAPH_H
#define GRAPH_H

#include <QStringList>
#include <QTimer>
#include <QWidget>

#include "../qcustomplot/qcustomplot.h"

extern "C++" {
#include "../controllers/s21_ControllerGraph.h"
#include "../controllers/s21_ControllerPolish.h"
}

namespace Ui {
class Graph;
}

class Graph : public QWidget {
  Q_OBJECT

 public:
  explicit Graph(s21::ControllerPolish &controller_polish,
                 QWidget *parent = nullptr);
  ~Graph();

 public slots:
  void ReceiveData(QString str_x_begin, QString str_x_end, QString str_y_begin,
                   QString str_y_end, QString label);

 private:
  Ui::Graph *ui_;
  s21::ControllerPolish &controller_polish_;
  s21::ControllerGraph controller_graph_{};
  double x_begin_{}, x_end_{}, X, y_begin_{}, y_end_{};

  QVector<double> x_, y_;

  //    QVector<std::pair<double, double>> n_;
};

#endif  // GRAPH_H
