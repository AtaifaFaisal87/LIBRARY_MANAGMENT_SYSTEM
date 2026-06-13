#include "mainwindow.h"
#include "loginwindow.h"
#include "registerdialog.h"
#include "userdashboard.h"
#include "librariandashboard.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Library Management System");
    resize(860, 600);
    setMinimumSize(700, 500);

    auto* central = new QWidget;
    auto* root    = new QVBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ---- Gradient hero section ----
    auto* hero = new QFrame;
    hero->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #16212E, stop:0.5 #1A2332, stop:1 #16212E);");
    auto* heroL = new QVBoxLayout(hero);
    heroL->setAlignment(Qt::AlignCenter);
    heroL->setSpacing(14);
    heroL->setContentsMargins(40, 60, 40, 40);

    // Decorative circles (CSS-style blobs via overlapping labels in a relative widget)
    auto* iconLbl = new QLabel("📚");
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet("font-size:64px; background:transparent;");

    auto* titleLbl = new QLabel("LIBRARY MANAGEMENT SYSTEM");
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet(
        "color: transparent;"
        "font-size: 32px;"
        "font-weight: bold;"
        "letter-spacing: 4px;"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #16212E, stop:1 #1E2D3D);"
        "-webkit-background-clip: text;");
    // Fallback – Qt doesn't support -webkit-background-clip, so just use cyan
    titleLbl->setStyleSheet(
        "color: #0D9488;"
        "font-size: 30px;"
        "font-weight: bold;"
        "letter-spacing: 4px;"
        "background: transparent;");

    auto* subtitleLbl = new QLabel("Manage books, users, reservations and more");
    subtitleLbl->setAlignment(Qt::AlignCenter);
    subtitleLbl->setStyleSheet("color: #64748B; font-size: 15px; background:transparent; font-style:italic;");

    // Feature pills row
    auto* pillsRow = new QHBoxLayout;
    pillsRow->setAlignment(Qt::AlignCenter);
    pillsRow->setSpacing(10);
    auto makePill = [](const QString& txt) {
        auto* p = new QLabel(txt);
        p->setStyleSheet(
            "background: rgba(61,90,241,0.25);"
            "color: #64748B;"
            "border: 1px solid #2D4059;"
            "border-radius: 14px;"
            "padding: 5px 14px;"
            "font-size: 12px;");
        return p;
    };
    pillsRow->addWidget(makePill("📖 Borrow & Return"));
    pillsRow->addWidget(makePill("🪑 Reservations"));
    pillsRow->addWidget(makePill("⭐ Memberships"));
    pillsRow->addWidget(makePill("⚠ Fine Management"));
    pillsRow->addWidget(makePill("👥 User Management"));

    // Buttons
    auto* btnRow = new QHBoxLayout;
    btnRow->setAlignment(Qt::AlignCenter);
    btnRow->setSpacing(16);

    auto* loginBtn = new QPushButton("🔐  Login");
    auto* regBtn   = new QPushButton("📝  Register");
    loginBtn->setFixedSize(160, 48);
    regBtn->setFixedSize(160, 48);
    loginBtn->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #16212E, stop:1 #1E2D3D);"
        "color: #FFFFFF; border-radius: 8px; font-size: 15px; font-weight: bold;");
    regBtn->setStyleSheet(
        "background: transparent;"
        "color: #0D9488; border: 2px solid #0D9488;"
        "border-radius: 8px; font-size: 15px; font-weight: bold;");
    regBtn->setObjectName("cyanBtn");

    btnRow->addWidget(loginBtn);
    btnRow->addWidget(regBtn);

    heroL->addWidget(iconLbl);
    heroL->addWidget(titleLbl);
    heroL->addWidget(subtitleLbl);
    heroL->addLayout(pillsRow);
    heroL->addSpacing(20);
    heroL->addLayout(btnRow);

    // ---- Footer strip ----
    auto* footer = new QFrame;
    footer->setFixedHeight(40);
    footer->setStyleSheet("background-color:#16212E; border-top:1px solid #2D4059;");
    auto* fl = new QHBoxLayout(footer);
    fl->setContentsMargins(20, 0, 20, 0);
    auto* footerLbl = new QLabel("© 2025 Library Management System  |  Librarian: admin / 123");
    footerLbl->setStyleSheet("color:#4B5563; font-size:11px;");
    fl->addWidget(footerLbl);
    fl->addStretch();
    auto* versionLbl = new QLabel("v2.0 – Qt GUI Edition");
    versionLbl->setStyleSheet("color:#4B5563; font-size:11px;");
    fl->addWidget(versionLbl);

    root->addWidget(hero, 1);
    root->addWidget(footer);

    setCentralWidget(central);
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLogin);
    connect(regBtn,   &QPushButton::clicked, this, &MainWindow::onRegister);
}

void MainWindow::onLogin() {
    LoginWindow dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    if (dlg.loggedRole == LoginWindow::LIBRARIAN) {
        auto* libDash = new LibrarianDashboard(nullptr);
        libDash->setAttribute(Qt::WA_DeleteOnClose);
        libDash->show();
    } else if (dlg.loggedRole == LoginWindow::USER) {
        auto* userDash = new UserDashboard(dlg.loggedUser, nullptr);
        userDash->setAttribute(Qt::WA_DeleteOnClose);
        userDash->show();
    }
}

void MainWindow::onRegister() {
    RegisterDialog dlg(this);
    dlg.exec();
}
