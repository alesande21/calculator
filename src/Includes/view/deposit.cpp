#include "deposit.h"

#include "ui_deposit.h"

Deposit::Deposit(QWidget *parent) : QWidget(parent), ui_(new Ui::Deposit) {
  ui_->setupUi(this);
  setFixedSize(1233, 545);
  ui_->le_deposit_amount->setText("1000000");
  ui_->le_tax_rate->setText("7.5");
  ui_->le_interest_rate->setText("8");
  ui_->le_period->setText("15");

  ui_->tableWidget->setRowCount(1);
  ui_->tableWidget->setColumnCount(4);
  ui_->tableWidget->setHorizontalHeaderLabels(QStringList()
                                              << "period"
                                              << "payment of interest"
                                              << "tax amount"
                                              << "deposit amount");

  ui_->le_deposit_amount->setValidator(validator_);
  ui_->le_tax_rate->setValidator(validator_);
  ui_->le_interest_rate->setValidator(validator_);
  ui_->le_period->setValidator(validator_int_);

  ui_->l_total_income->setText("0");
  ui_->l_total_tax_payment->setText("0");
  ui_->l_total_amount->setText("0");

  connect(ui_->calculate, SIGNAL(clicked()), this, SLOT(CalculateDeposit()));
}

Deposit::~Deposit() { delete ui_; }

void Deposit::CalculateDeposit() {
  bool ok{};
  deposit_period_ = ui_->le_period->text().toInt(&ok);
  deposit_amount_ = ui_->le_deposit_amount->text().toDouble(&ok);
  interest_rate_ = ui_->le_interest_rate->text().toDouble(&ok);
  tax_rate_ = ui_->le_tax_rate->text().toDouble(&ok);
  cap_of_interest_ = ui_->rb_cap->isChecked();

  auto result = controller_deposit_.CalculateDeposit(
      deposit_amount_, deposit_period_, ui_->cb_deposit_period->currentIndex(),
      interest_rate_, tax_rate_, ui_->cb_payment_frequency->currentIndex(),
      cap_of_interest_);

  int row = 0;
  if (result.second) {
    ui_->tableWidget->setRowCount(result.first.size() - 1);
    auto it = result.first.begin();
    for (; it != --result.first.end(); ++it) {
      std::string str_date = date::format("%Y-%m-%d", it->first);
      QTableWidgetItem *item1 =
          new QTableWidgetItem(QString::fromStdString(str_date));
      ui_->tableWidget->setItem(row, 0, item1);
      ui_->tableWidget->setItem(row, 1,
                                new QTableWidgetItem(QString::number(
                                    it->second.payment_of_interest_, 'f', 2)));
      ui_->tableWidget->setItem(row, 2,
                                new QTableWidgetItem(QString::number(
                                    it->second.tax_amount_, 'f', 2)));
      ui_->tableWidget->setItem(row, 3,
                                new QTableWidgetItem(QString::number(
                                    it->second.deposit_amount_, 'f', 2)));
      ++row;
    }

    ui_->l_total_income->setText(
        QString::number(it->second.payment_of_interest_, 'f', 2));
    ui_->l_total_tax_payment->setText(
        QString::number(it->second.tax_amount_, 'f', 2));
    ui_->l_total_amount->setText(
        QString::number(it->second.deposit_amount_, 'f', 2));

  } else {
    ui_->l_total_income->setText("Error");
    ui_->l_total_tax_payment->setText("Error");
    ui_->l_total_amount->setText("Error");
    ui_->tableWidget->setRowCount(1);
    ui_->tableWidget->setColumnCount(4);
    for (; row < 4; ++row) {
      ui_->tableWidget->setItem(
          0, row, new QTableWidgetItem(QString::number(0, 'f', 2)));
    }
  }
}
