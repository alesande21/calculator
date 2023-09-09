#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDoubleValidator>
#include <QIntValidator>
#include <QWidget>

extern "C++" {
#include "../controllers/s21_ControllerDeposit.h"
}

namespace Ui {
class Deposit;
}

class Deposit : public QWidget {
  Q_OBJECT

 public:
  explicit Deposit(QWidget *parent = nullptr);
  ~Deposit();

 private:
  Ui::Deposit *ui_;
  long int deposit_period_{};
  int payment_frequency_{}, type_period_{}, cap_of_interest_{};
  double deposit_amount_{}, interest_rate_{}, tax_rate_{};
  s21::ControllerDeposit controller_deposit_;
  QDoubleValidator *validator_ = new QDoubleValidator();
  QIntValidator *validator_int_ = new QIntValidator();

 private slots:
  void CalculateDeposit();
};

#endif  // DEPOSIT_H
