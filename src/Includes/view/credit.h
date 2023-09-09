#ifndef CREDIT_H
#define CREDIT_H

#include <QDoubleValidator>
#include <QIntValidator>
#include <QStandardItemModel>
#include <QWidget>

extern "C++" {
#include "../controllers/s21_ControllerCredit.h"
}

using namespace date;

namespace Ui {
class Credit;
}

class Credit : public QWidget {
  Q_OBJECT

 public:
  using Components = s21::CreditModel::Components;

  explicit Credit(QWidget *parent = nullptr);
  ~Credit();

 private:
  Ui::Credit *ui_;
  long int loan_term_{};
  int type_;
  double loan_amount_{}, interest_rate_{};
  s21::ControllerCredit credit_model_;
  QDoubleValidator *validator_ = new QDoubleValidator();
  QIntValidator *validator_int_ = new QIntValidator();

 private slots:
  void CalculateCredit();
};

#endif  // CREDIT_H
