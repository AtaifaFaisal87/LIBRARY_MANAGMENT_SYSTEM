#include "loginwindow.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Library Management System – Login");
    setFixedSize(440, 520);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    auto* root = new QVBoxLayout(this);
    root->setSpacing(0);
    root->setContentsMargins(0, 0, 0, 0);

    // ---- Header banner ----
    auto* header = new QFrame;
    header->setFixedHeight(140);
    header->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #16212E, stop:1 #1E2D3D);"
        "border-top-left-radius:10px; border-top-right-radius:10px;");

    auto* hLayout = new QVBoxLayout(header);
    hLayout->setAlignment(Qt::AlignCenter);

    auto* icon = new QLabel("📚");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size:40px; background:transparent;");

    auto* titleLbl = new QLabel("LIBRARY SYSTEM");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet("color:#FFFFFF; font-size:22px; font-weight:bold;"
                            "background:transparent; letter-spacing:3px;");

    auto* subtitleLbl = new QLabel("Manage. Borrow. Reserve.");
    subtitleLbl->setAlignment(Qt::AlignCenter);
    subtitleLbl->setStyleSheet("color:rgba(255,255,255,0.8); font-size:12px;"
                               "background:transparent;");

    hLayout->addWidget(icon);
    hLayout->addWidget(titleLbl);
    hLayout->addWidget(subtitleLbl);

    // ---- Form card ----
    auto* card = new QFrame;
    card->setStyleSheet("background-color:#1E2D3D; border-bottom-left-radius:10px;"
                        "border-bottom-right-radius:10px;");

    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(14);
    cardLayout->setContentsMargins(40, 30, 40, 30);

    // Role selector
    auto* roleRow = new QHBoxLayout;
    auto* roleLbl = new QLabel("Login as:");
    roleLbl->setStyleSheet("color:#64748B; font-weight:bold;");
    roleCombo = new QComboBox;
    roleCombo->addItem("👤  User");
    roleCombo->addItem("🔑  Librarian");
    roleRow->addWidget(roleLbl);
    roleRow->addWidget(roleCombo, 1);

    // Username
    auto* uLabel = new QLabel("Username");
    uLabel->setStyleSheet("color:#64748B; font-weight:bold; font-size:12px;");
    usernameEdit = new QLineEdit;
    usernameEdit->setPlaceholderText("Enter username…");

    // Password
    auto* pLabel = new QLabel("Password");
    pLabel->setStyleSheet("color:#64748B; font-weight:bold; font-size:12px;");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Enter password…");

    // Status label
    statusLabel = new QLabel;
    statusLabel->setObjectName("errorLabel");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->hide();

    // Buttons
    auto* btnRow = new QHBoxLayout;
    loginBtn  = new QPushButton("LOGIN");
    cancelBtn = new QPushButton("EXIT");
    cancelBtn->setObjectName("dangerBtn");
    loginBtn->setFixedHeight(40);
    cancelBtn->setFixedHeight(40);
    btnRow->addWidget(cancelBtn);
    btnRow->addWidget(loginBtn);

    cardLayout->addLayout(roleRow);
    cardLayout->addWidget(uLabel);
    cardLayout->addWidget(usernameEdit);
    cardLayout->addWidget(pLabel);
    cardLayout->addWidget(passwordEdit);
    cardLayout->addWidget(statusLabel);
    cardLayout->addSpacing(6);
    cardLayout->addLayout(btnRow);

    root->addWidget(header);
    root->addWidget(card, 1);

    connect(loginBtn, &QPushButton::clicked, this, &LoginWindow::onLogin);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onLogin);
}

void LoginWindow::onLogin() {
    QString uname = usernameEdit->text().trimmed();
    QString pass  = passwordEdit->text();

    if (uname.isEmpty() || pass.isEmpty()) {
        statusLabel->setText("⚠  Please fill in all fields.");
        statusLabel->show();
        return;
    }

    auto& db = Database::instance();
    bool isLibrarian = (roleCombo->currentIndex() == 1);

    if (isLibrarian) {
        // hard-coded librarian credentials (same as original)
        if (uname == "admin" && pass == "123") {
            loggedRole = LIBRARIAN;
            accept();
        } else {
            statusLabel->setText("✗  Invalid librarian credentials.");
            statusLabel->show();
        }
        return;
    }

    // user login
    for (auto& u : db.users) {
        if (std::string(u.username) == uname.toStdString() &&
            std::string(u.password) == pass.toStdString()) {
            if (u.isDeleted) {
                statusLabel->setText("✗  This account has been deleted.");
                statusLabel->show();
                return;
            }
            loggedUser = u;
            loggedRole = USER;
            accept();
            return;
        }
    }
    statusLabel->setText("✗  Invalid username or password.");
    statusLabel->show();
}
