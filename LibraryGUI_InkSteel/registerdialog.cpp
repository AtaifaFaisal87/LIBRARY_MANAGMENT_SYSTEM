#include "registerdialog.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QMessageBox>
#include <cstring>

RegisterDialog::RegisterDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("Register New User");
    setMinimumWidth(520);
    setStyleSheet("background-color:#1E2D3D;");

    auto* root = new QVBoxLayout(this);
    root->setSpacing(14);
    root->setContentsMargins(30, 24, 30, 24);

    // Header
    auto* hdr = new QLabel("📝  Register New User");
    hdr->setStyleSheet("color:#0D9488; font-size:18px; font-weight:bold;");
    root->addWidget(hdr);

    auto* sep = new QFrame; sep->setFrameShape(QFrame::HLine);
    sep->setStyleSheet("color:#2D4059;");
    root->addWidget(sep);

    // Type selector
    auto* typeRow = new QHBoxLayout;
    typeRow->addWidget(new QLabel("User Type:"));
    typeCombo = new QComboBox;
    typeCombo->addItems({"STUDENT", "FACULTY", "GUEST"});
    typeRow->addWidget(typeCombo, 1);
    root->addLayout(typeRow);

    // Form
    auto* form = new QFormLayout;
    form->setSpacing(10);
    form->setLabelAlignment(Qt::AlignRight);

    nameEdit     = new QLineEdit; nameEdit->setPlaceholderText("Full name (letters only)");
    cnicEdit     = new QLineEdit; cnicEdit->setPlaceholderText("xxxxx-xxxxxxx-x");
    emailEdit    = new QLineEdit; emailEdit->setPlaceholderText("user@example.com");
    phoneEdit    = new QLineEdit; phoneEdit->setPlaceholderText("03xxxxxxxxx");
    usernameEdit = new QLineEdit; usernameEdit->setPlaceholderText("Choose a username");
    passwordEdit = new QLineEdit; passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setPlaceholderText("Min 6 characters");
    balanceSpin  = new QDoubleSpinBox;
    balanceSpin->setRange(0, 999999); balanceSpin->setPrefix("Rs. ");

    form->addRow("Name:",     nameEdit);
    form->addRow("CNIC:",     cnicEdit);
    form->addRow("Email:",    emailEdit);
    form->addRow("Phone:",    phoneEdit);
    form->addRow("Username:", usernameEdit);
    form->addRow("Password:", passwordEdit);
    form->addRow("Balance:",  balanceSpin);

    root->addLayout(form);

    // ---- Extra fields (stacked) ----
    extraStack = new QStackedWidget;

    // Student page
    auto* studentPage = new QWidget;
    auto* sf = new QFormLayout(studentPage);
    seatEdit  = new QLineEdit; seatEdit->setPlaceholderText("e.g. CS-101");
    majorEdit = new QLineEdit; majorEdit->setPlaceholderText("e.g. Computer Science");
    sf->addRow("Seat No:",  seatEdit);
    sf->addRow("Major:",    majorEdit);

    // Faculty page
    auto* facultyPage = new QWidget;
    auto* ff = new QFormLayout(facultyPage);
    deptEdit  = new QLineEdit; deptEdit->setPlaceholderText("e.g. Mathematics");
    desigEdit = new QLineEdit; desigEdit->setPlaceholderText("e.g. Professor");
    ff->addRow("Department:",  deptEdit);
    ff->addRow("Designation:", desigEdit);

    // Guest page
    auto* guestPage = new QWidget;
    auto* gf = new QFormLayout(guestPage);
    purposeEdit = new QLineEdit; purposeEdit->setPlaceholderText("Reason for visit");
    gf->addRow("Purpose:", purposeEdit);

    extraStack->addWidget(studentPage);
    extraStack->addWidget(facultyPage);
    extraStack->addWidget(guestPage);
    root->addWidget(extraStack);

    // Status
    statusLabel = new QLabel;
    statusLabel->setObjectName("errorLabel");
    statusLabel->hide();
    root->addWidget(statusLabel);

    // Buttons
    auto* btnRow = new QHBoxLayout;
    auto* cancelBtn = new QPushButton("Cancel");
    cancelBtn->setObjectName("dangerBtn");
    auto* regBtn = new QPushButton("✔  Register");
    regBtn->setObjectName("successBtn");
    btnRow->addWidget(cancelBtn);
    btnRow->addStretch();
    btnRow->addWidget(regBtn);
    root->addLayout(btnRow);

    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RegisterDialog::onTypeChanged);
    connect(regBtn,    &QPushButton::clicked, this, &RegisterDialog::onRegister);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void RegisterDialog::onTypeChanged(int index) {
    extraStack->setCurrentIndex(index);
}

bool RegisterDialog::validateInputs(QString& err) {
    if (nameEdit->text().trimmed().isEmpty())     { err="Name cannot be empty."; return false; }
    if (cnicEdit->text().trimmed().length() != 15){ err="CNIC must be in format xxxxx-xxxxxxx-x (15 chars)."; return false; }
    if (!emailEdit->text().contains('@'))         { err="Invalid email."; return false; }
    if (phoneEdit->text().trimmed().length() != 11){ err="Phone must be 11 digits starting with 03."; return false; }
    if (usernameEdit->text().trimmed().isEmpty()) { err="Username cannot be empty."; return false; }
    if (passwordEdit->text().length() < 6)        { err="Password must be at least 6 characters."; return false; }
    return true;
}

void RegisterDialog::onRegister() {
    QString err;
    if (!validateInputs(err)) {
        statusLabel->setText("⚠  " + err);
        statusLabel->show();
        return;
    }

    auto& db = Database::instance();

    // Duplicate checks
    std::string cnic  = cnicEdit->text().trimmed().toStdString();
    std::string email = emailEdit->text().trimmed().toStdString();
    std::string phone = phoneEdit->text().trimmed().toStdString();
    std::string uname = usernameEdit->text().trimmed().toStdString();

    for (auto& u : db.users) {
        if (u.isDeleted) continue;
        if (std::string(u.cnic)     == cnic)  { statusLabel->setText("✗ CNIC already registered.");     statusLabel->show(); return; }
        if (std::string(u.email)    == email) { statusLabel->setText("✗ Email already registered.");    statusLabel->show(); return; }
        if (std::string(u.phone)    == phone) { statusLabel->setText("✗ Phone already registered.");    statusLabel->show(); return; }
        if (std::string(u.username) == uname) { statusLabel->setText("✗ Username already taken.");      statusLabel->show(); return; }
    }

    UserRecord r{};
    r.id = db.nextUserID++;
    strncpy(r.name,     nameEdit->text().trimmed().toStdString().c_str(),     49);
    strncpy(r.email,    email.c_str(),  49);
    strncpy(r.phone,    phone.c_str(),  19);
    strncpy(r.cnic,     cnic.c_str(),   19);
    strncpy(r.username, uname.c_str(),  49);
    strncpy(r.password, passwordEdit->text().toStdString().c_str(), 49);
    r.balance      = balanceSpin->value();
    r.isDeleted    = false;
    r.hasMembership= false;
    r.borrowed_count = 0;
    r.daily_borrows  = 0;

    int typeIdx = typeCombo->currentIndex();
    if (typeIdx == 0) {
        strncpy(r.type, "STUDENT", 19);
        strncpy(r.seatno, seatEdit->text().trimmed().toStdString().c_str(), 29);
        strncpy(r.major,  majorEdit->text().trimmed().toStdString().c_str(), 49);
    } else if (typeIdx == 1) {
        strncpy(r.type, "FACULTY", 19);
        strncpy(r.department,  deptEdit->text().trimmed().toStdString().c_str(), 49);
        strncpy(r.designation, desigEdit->text().trimmed().toStdString().c_str(), 49);
    } else {
        strncpy(r.type, "GUEST", 19);
        strncpy(r.purpose, purposeEdit->text().trimmed().toStdString().c_str(), 99);
    }

    db.users.push_back(r);
    db.appendUser(r);

    QMessageBox::information(this, "Success",
        QString("✔  User '%1' registered successfully!\nUser ID: %2")
            .arg(r.name).arg(r.id));
    accept();
}
