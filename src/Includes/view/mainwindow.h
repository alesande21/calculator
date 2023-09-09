#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDoubleValidator>
#include <QIntValidator>
#include <QMainWindow>
#include <QMessageBox>
#include <QVector>

#include "credit.h"
#include "deposit.h"
#include "graph.h"

extern "C++" {
#include "../controllers/s21_ControllerPolish.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 signals:
  void SendDataToGraph(QString str_x_begin, QString str_x_end,
                       QString str_y_begin, QString str_y_end, QString result);

 private:
  Ui::MainWindow *ui;
  s21::ControllerPolish controller_polish_;
  Graph *graph_window_;
  Credit *credit_window_;
  Deposit *deposit_window_;
  QDoubleValidator *validator_ = new QDoubleValidator();

 private slots:
  void ButtonAction();
  void AcClicked();
  void ItogClicked();
  void OpenGraph();
  void CreditOpen();
  void DepositOpen();
};
#endif  // MAINWINDOW_H
