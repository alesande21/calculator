#include "graph.h"

#include "mainwindow.h"
#include "ui_graph.h"

Graph::Graph(s21::ControllerPolish &controller_polish, QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::Graph),
      controller_polish_(controller_polish) {
  ui_->setupUi(this);
}

Graph::~Graph() { delete ui_; }

void Graph::ReceiveData(QString str_x_begin, QString str_x_end,
                        QString str_y_begin, QString str_y_end, QString label) {
  ui_->widget->clearGraphs();
  bool ok{};
  x_begin_ = str_x_begin.toDouble(&ok);
  x_end_ = str_x_end.toDouble(&ok);
  y_begin_ = str_y_begin.toDouble(&ok);
  y_end_ = str_y_end.toDouble(&ok);

  ui_->widget->xAxis->setRange(x_begin_, x_end_);
  ui_->widget->yAxis->setRange(y_begin_, y_end_);

  QByteArray barr = label.toLocal8Bit();
  char *str = barr.data();

  auto result =
      controller_graph_.CreateGraph(x_begin_, x_end_, str, controller_polish_);

  if (result.second) {
    auto it = result.first.begin();
    for (; it != result.first.end(); ++it) {
      x_.push_back(it->first);
      y_.push_back(it->second);
    }
    ui_->status_graph->setText("Good");
  } else {
    ui_->status_graph->setText("Error");
  }

  ui_->widget->addGraph();
  ui_->widget->graph(0)->addData(x_, y_);
  ui_->widget->replot();
  x_.clear();
  y_.clear();
}
