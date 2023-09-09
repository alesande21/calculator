#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setFixedSize(419, 449);
  graph_window_ = new Graph(controller_polish_);
  credit_window_ = new Credit();
  deposit_window_ = new Deposit();

  ui->str_x->setText("0");
  ui->oo_left->setText("-10");
  ui->oo_right->setText("10");
  ui->oz_left->setText("-1");
  ui->oz_right->setText("1");
  ui->str_x->setValidator(validator_);
  ui->oo_left->setValidator(validator_);
  ui->oo_right->setValidator(validator_);
  ui->oz_left->setValidator(validator_);
  ui->oz_right->setValidator(validator_);

  connect(ui->b_0, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_1, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_2, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_3, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_4, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_5, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_6, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_7, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_8, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_9, SIGNAL(clicked()), this, SLOT(ButtonAction()));

  connect(ui->b_x, SIGNAL(clicked()), this, SLOT(ButtonAction()));

  connect(ui->b_unar, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_mult, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_div, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_plus, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_minus, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_pow, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_mod, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_bopen, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_bclose, SIGNAL(clicked()), this, SLOT(ButtonAction()));

  connect(ui->b_cos, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_sqrt, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_sin, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_asin, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_acos, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_tan, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_atan, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_ln, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->b_log, SIGNAL(clicked()), this, SLOT(ButtonAction()));

  connect(ui->b_dot, SIGNAL(clicked()), this, SLOT(ButtonAction()));
  connect(ui->graph, SIGNAL(clicked()), graph_window_, SLOT(show()));

  connect(ui->b_ac, SIGNAL(clicked()), this, SLOT(AcClicked()));
  connect(ui->b_itog, SIGNAL(clicked()), this, SLOT(ItogClicked()));

  connect(ui->credit, SIGNAL(clicked()), this, SLOT(CreditOpen()));
  connect(ui->deposit, SIGNAL(clicked()), this, SLOT(DepositOpen()));

  connect(ui->graph, SIGNAL(clicked()), this, SLOT(OpenGraph()));
  connect(this,
          SIGNAL(SendDataToGraph(QString, QString, QString, QString, QString)),
          graph_window_,
          SLOT(ReceiveData(QString, QString, QString, QString, QString)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::OpenGraph() {
  emit SendDataToGraph(ui->oo_left->text(), ui->oo_right->text(),
                       ui->oz_left->text(), ui->oz_right->text(),
                       ui->str->text());
}

void MainWindow::ButtonAction() {
  QPushButton *button = (QPushButton *)sender();

  if (ui->str->text().length() == 1 && ui->str->text().endsWith('0')) {
    if (button->text() == "+/-") {
      ui->str->setText("-");
    } else {
      ui->str->setText(button->text());
    }
  } else {
    if (button->text() == "+/-" && ui->str->text().length() == 1) {
      if (ui->str->text().endsWith('+')) {
        ui->str->setText("-");
      } else if (ui->str->text().endsWith('-')) {
        ui->str->setText("+");
      }
    } else {
      if (button->text() == "+/-") {
        ui->str->setText(ui->str->text() + "-");
      } else {
        ui->str->setText(ui->str->text() + button->text());
      }
    }
  }
}

void MainWindow::AcClicked() {
  ui->str->setText("0");
  ui->str_x->setText("0");
}

void MainWindow::ItogClicked() {
  QByteArray barr = ui->str->text().toLocal8Bit();
  char *str = barr.data();

  QByteArray barr2 = ui->str_x->text().toLocal8Bit();
  char *x_str = barr2.data();

  auto result = controller_polish_.СalculateExpression(str, x_str);

  ui->str->setText(QString::fromStdString(result.first));
}

void MainWindow::CreditOpen() { credit_window_->show(); }

void MainWindow::DepositOpen() { deposit_window_->show(); }
