#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include "library_core.h"

class LoginWindow : public QDialog {
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);

    enum Role { NONE, USER, LIBRARIAN };
    Role      loggedRole = NONE;
    UserRecord loggedUser{};

private slots:
    void onLogin();

private:
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QComboBox* roleCombo;
    QLabel*    statusLabel;
    QPushButton* loginBtn;
    QPushButton* cancelBtn;
};
