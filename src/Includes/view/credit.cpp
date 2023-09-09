#include "credit.h"

#include "mainwindow.h"
#include "ui_credit.h"

Credit::Credit(QWidget *parent) : QWidget(parent), ui_(new Ui::Credit) {
  ui_->setupUi(this);
  setFixedSize(1141, 501);
  ui_->le_loan_amount->setText("1000000");
  ui_->le_loan_term->setText("15");
  ui_->le_interest_rate->setText("8");
  ui_->le_loan_amount->setValidator(validator_);
  ui_->le_loan_term->setValidator(validator_int_);
  ui_->le_interest_rate->setValidator(validator_);
  ui_->l_tip->setText("0");
  ui_->l_tip_plus_pap->setText("0");
  ui_->tableWidget->setRowCount(1);
  ui_->tableWidget->setColumnCount(5);
  ui_->tableWidget->setHorizontalHeaderLabels(QStringList()
                                              << "period"
                                              << "month payment"
                                              << "principal amount payment"
                                              << "interest payment"
                                              << "debt balance");

  connect(ui_->calculate, SIGNAL(clicked()), this, SLOT(CalculateCredit()));
}

Credit::~Credit() { delete ui_; }

void Credit::CalculateCredit() {
  bool ok = 0;

  loan_term_ = ui_->le_loan_term->text().toInt(&ok);
  loan_amount_ = ui_->le_loan_amount->text().toDouble(&ok);
  interest_rate_ = ui_->le_interest_rate->text().toDouble(&ok);
  type_ = (ui_->Annuity->isChecked()) && !ui_->Differential->isChecked();
  auto result = credit_model_.CalculateCredit(loan_amount_, loan_term_,
                                              interest_rate_, type_);
  int row = 0;
  if (result.second) {
    ui_->tableWidget->setRowCount(loan_term_);
    auto it = result.first.begin();
    for (; it != --result.first.end(); ++it) {
      std::string sts = date::format("%Y/%m", it->first);
      QTableWidgetItem *item1 =
          new QTableWidgetItem(QString::fromStdString(sts));
      ui_->tableWidget->setItem(row, 0, item1);
      ui_->tableWidget->setItem(row, 1,
                                new QTableWidgetItem(QString::number(
                                    it->second.month_payment_, 'f', 2)));
      ui_->tableWidget->setItem(
          row, 2,
          new QTableWidgetItem(
              QString::number(it->second.principal_amount_payment_, 'f', 2)));
      ui_->tableWidget->setItem(row, 3,
                                new QTableWidgetItem(QString::number(
                                    it->second.interest_payment_, 'f', 2)));
      ui_->tableWidget->setItem(row, 4,
                                new QTableWidgetItem(QString::number(
                                    it->second.debt_balance_, 'f', 2)));
      ++row;
    }
    ui_->l_tip->setText(QString::number(it->second.interest_payment_, 'f', 2));
    ui_->l_tip_plus_pap->setText(
        QString::number(it->second.month_payment_, 'f', 2));
  } else {
    ui_->l_tip->setText("Error");
    ui_->l_tip_plus_pap->setText("Error");
    ui_->tableWidget->setRowCount(1);
    ui_->tableWidget->setColumnCount(5);
    for (; row < 5; ++row) {
      ui_->tableWidget->setItem(
          0, row, new QTableWidgetItem(QString::number(0, 'f', 2)));
    }
  }
}
