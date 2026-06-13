#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>
#include <QDoubleSpinBox>
#include "library_core.h"

class RegisterDialog : public QDialog {
    Q_OBJECT
public:
    explicit RegisterDialog(QWidget* parent = nullptr);

private slots:
    void onTypeChanged(int index);
    void onRegister();

private:
    QComboBox*      typeCombo;
    QLineEdit*      nameEdit;
    QLineEdit*      cnicEdit;
    QLineEdit*      emailEdit;
    QLineEdit*      phoneEdit;
    QLineEdit*      usernameEdit;
    QLineEdit*      passwordEdit;
    QDoubleSpinBox* balanceSpin;
    QLabel*         statusLabel;

    // Student extras
    QLineEdit* seatEdit;
    QLineEdit* majorEdit;
    // Faculty extras
    QLineEdit* deptEdit;
    QLineEdit* desigEdit;
    // Guest extras
    QLineEdit* purposeEdit;

    QStackedWidget* extraStack;

    bool validateInputs(QString& err);
};
