#include "userdashboard.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QScrollArea>
#include <QInputDialog>
#include <QMessageBox>
#include <QHeaderView>
#include <QStatusBar>
#include <QMenuBar>
#include <QSpinBox>
#include <QDateEdit>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <cstring>
#include <ctime>

// ---- helpers ----
static QFrame* makeSep() {
    auto* f = new QFrame; f->setFrameShape(QFrame::HLine);
    f->setStyleSheet("color:#2D4059;"); return f;
}
static QWidget* makeStatCard(const QString& icon, const QString& val,
                              const QString& lbl, const QString& cardStyle) {
    auto* card = new QFrame;
    card->setStyleSheet(cardStyle);
    card->setMinimumHeight(90);
    auto* cl = new QVBoxLayout(card);
    cl->setAlignment(Qt::AlignCenter);
    auto* ico = new QLabel(icon); ico->setAlignment(Qt::AlignCenter);
    ico->setStyleSheet("font-size:22px; background:transparent;");
    auto* vl = new QLabel(val);   vl->setAlignment(Qt::AlignCenter);
    vl->setObjectName("statValue");
    auto* ll = new QLabel(lbl);   ll->setAlignment(Qt::AlignCenter);
    ll->setObjectName("statLabel");
    cl->addWidget(ico); cl->addWidget(vl); cl->addWidget(ll);
    return card;
}

// ===========================================================
//  CONSTRUCTOR
// ===========================================================
UserDashboard::UserDashboard(const UserRecord& user, QWidget* parent)
    : QMainWindow(parent), currentUser(user)
{
    setWindowTitle(QString("Library – %1 (%2)").arg(user.name).arg(user.type));
    resize(1100, 720);

    // Menu bar
    auto* mb  = menuBar();
    auto* fm  = mb->addMenu("File");
    auto* logoutAct = fm->addAction("🔓 Logout");
    connect(logoutAct, &QAction::triggered, this, &QMainWindow::close);
    auto* profileAct = mb->addAction("👤 Update Profile");
    connect(profileAct, &QAction::triggered, this, &UserDashboard::onUpdateProfile);
    auto* balAct = mb->addAction("💰 Add Balance");
    connect(balAct, &QAction::triggered, this, &UserDashboard::onAddBalance);

    // Central widget
    auto* central = new QWidget;
    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(10, 10, 10, 10);
    rootLayout->setSpacing(8);

    // Top banner
    auto* banner = new QFrame;
    banner->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #16212E, stop:1 #1E2D3D);"
        "border-radius:8px; padding:10px;");
    auto* bannerL = new QHBoxLayout(banner);
    auto* welcomeLbl = new QLabel(QString("👋  Welcome, %1!").arg(user.name));
    welcomeLbl->setStyleSheet("color:#FFFFFF; font-size:18px; font-weight:bold; background:transparent;");
    auto* typeLbl = new QLabel(QString("[ %1 ]").arg(user.type));
    typeLbl->setStyleSheet("color:rgba(255,255,255,0.85); font-size:13px; background:transparent;");
    auto* logoutBtn = new QPushButton("🔓 Logout");
    logoutBtn->setStyleSheet("background:rgba(13,148,136,0.2); color:#E2E8F0;"
                             "border-radius:5px; padding:6px 14px; font-weight:bold;");
    connect(logoutBtn, &QPushButton::clicked, this, &QMainWindow::close);
    bannerL->addWidget(welcomeLbl);
    bannerL->addWidget(typeLbl);
    bannerL->addStretch();
    bannerL->addWidget(logoutBtn);

    rootLayout->addWidget(banner);

    // Tabs
    tabs = new QTabWidget;
    buildDashboardTab(tabs);
    buildBooksTab(tabs);
    buildTransactionsTab(tabs);
    buildFinesTab(tabs);
    buildReservationsTab(tabs);
    buildMembershipTab(tabs);
    rootLayout->addWidget(tabs, 1);

    setCentralWidget(central);
    statusBar()->showMessage(QString("Logged in as: %1  |  ID: %2")
                             .arg(user.name).arg(user.id));
    refreshAll();
}

// ===========================================================
//  BUILD TABS
// ===========================================================
void UserDashboard::buildDashboardTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(12); pl->setContentsMargins(14, 14, 14, 14);

    // Stat cards row
    auto* grid = new QGridLayout;
    grid->setSpacing(12);

    balanceLbl    = new QLabel("Rs. 0");   balanceLbl->setObjectName("statValue");
    membershipLbl = new QLabel("NONE");    membershipLbl->setObjectName("statValue");
    borrowedLbl   = new QLabel("0");       borrowedLbl->setObjectName("statValue");
    finesLbl      = new QLabel("Rs. 0");   finesLbl->setObjectName("statValue");

    grid->addWidget(makeStatCard("💰","Rs. 0","Balance",     STAT_CARD_STYLE_GREEN), 0,0);
    grid->addWidget(makeStatCard("⭐","–","Membership",      STAT_CARD_STYLE_BLUE),  0,1);
    grid->addWidget(makeStatCard("📖","0","Books Borrowed", STAT_CARD_STYLE_CYAN),  0,2);
    grid->addWidget(makeStatCard("⚠","Rs. 0","Live Fines",  STAT_CARD_STYLE_ROSE),  0,3);

    // grab label widgets from stat cards to update later
    auto getStatVal = [](QWidget* card) -> QLabel* {
        for (auto* c : card->findChildren<QLabel*>())
            if (c->objectName() == "statValue") return c;
        return nullptr;
    };
    balanceLbl    = getStatVal(grid->itemAtPosition(0,0)->widget());
    membershipLbl = getStatVal(grid->itemAtPosition(0,1)->widget());
    borrowedLbl   = getStatVal(grid->itemAtPosition(0,2)->widget());
    finesLbl      = getStatVal(grid->itemAtPosition(0,3)->widget());

    pl->addLayout(grid);

    // Currently borrowed books table
    auto* lbl = new QLabel("📚  Currently Borrowed Books");
    lbl->setObjectName("sectionLabel");
    pl->addWidget(lbl);

    borrowedTable = new QTableWidget(0, 5);
    borrowedTable->setHorizontalHeaderLabels({"Book ID","Title","Author","Due Date","Status"});
    borrowedTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    borrowedTable->setAlternatingRowColors(true);
    borrowedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pl->addWidget(borrowedTable, 1);

    t->addTab(page, "🏠  Dashboard");
}

void UserDashboard::buildBooksTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14, 14, 14, 14);

    // Action buttons
    auto* btnRow = new QHBoxLayout;
    auto* borrowBtn   = new QPushButton("📥  Borrow Book");
    auto* returnBtn   = new QPushButton("📤  Return Book");
    auto* readBtn     = new QPushButton("👁  Read On-Site");
    auto* dlBtn       = new QPushButton("⬇  Download");
    borrowBtn->setObjectName("successBtn");
    returnBtn->setObjectName("warningBtn");
    readBtn->setObjectName("cyanBtn");
    btnRow->addWidget(borrowBtn);
    btnRow->addWidget(returnBtn);
    btnRow->addWidget(readBtn);
    btnRow->addWidget(dlBtn);
    btnRow->addStretch();
    pl->addLayout(btnRow);

    bookTable = new QTableWidget(0, 8);
    bookTable->setHorizontalHeaderLabels(
        {"ID","Title","Author","Category","ISBN","Publisher","Year","Copies"});
    bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookTable->setAlternatingRowColors(true);
    bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pl->addWidget(bookTable, 1);

    connect(borrowBtn, &QPushButton::clicked, this, &UserDashboard::onBorrow);
    connect(returnBtn, &QPushButton::clicked, this, &UserDashboard::onReturn);
    connect(readBtn,   &QPushButton::clicked, this, &UserDashboard::onReadOnSite);
    connect(dlBtn,     &QPushButton::clicked, this, &UserDashboard::onDownload);

    t->addTab(page, "📚  Books");
}

void UserDashboard::buildTransactionsTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14, 14, 14, 14);

    auto* lbl = new QLabel("📋  Your Transaction History");
    lbl->setObjectName("sectionLabel");
    pl->addWidget(lbl);

    txnTable = new QTableWidget(0, 7);
    txnTable->setHorizontalHeaderLabels(
        {"Txn ID","Book ID","Borrow Time","Due Time","Return Time","Returned","Overdue Mins"});
    txnTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    txnTable->setAlternatingRowColors(true);
    txnTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pl->addWidget(txnTable, 1);

    t->addTab(page, "📋  Transactions");
}

void UserDashboard::buildFinesTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14, 14, 14, 14);

    auto* lbl = new QLabel("⚠  Your Fines");
    lbl->setObjectName("sectionLabel");
    pl->addWidget(lbl);

    finesTable = new QTableWidget(0, 6);
    finesTable->setHorizontalHeaderLabels(
        {"Book ID","Mins Late","Rate/Min","Original","Discount","Final"});
    finesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    finesTable->setAlternatingRowColors(true);
    finesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    pl->addWidget(finesTable, 1);

    t->addTab(page, "⚠  Fines");
}

void UserDashboard::buildReservationsTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14, 14, 14, 14);

    auto* btnRow = new QHBoxLayout;
    auto* resTableBtn = new QPushButton("🪑  Reserve Table");
    auto* resBookBtn  = new QPushButton("📖  Reserve Book");
    auto* cancelBtn2  = new QPushButton("❌  Cancel Reservation");
    resTableBtn->setObjectName("cyanBtn");
    resBookBtn->setObjectName("successBtn");
    cancelBtn2->setObjectName("dangerBtn");
    btnRow->addWidget(resTableBtn);
    btnRow->addWidget(resBookBtn);
    btnRow->addWidget(cancelBtn2);
    btnRow->addStretch();
    pl->addLayout(btnRow);

    resTable = new QTableWidget(0, 6);
    resTable->setHorizontalHeaderLabels(
        {"Res ID","Type","Target ID","Date","Start","End"});
    resTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resTable->setAlternatingRowColors(true);
    resTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    resTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    pl->addWidget(resTable, 1);

    connect(resTableBtn, &QPushButton::clicked, this, &UserDashboard::onReserveTable);
    connect(resBookBtn,  &QPushButton::clicked, this, &UserDashboard::onReserveBook);
    connect(cancelBtn2,  &QPushButton::clicked, this, &UserDashboard::onCancelReservation);

    t->addTab(page, "🪑  Reservations");
}

void UserDashboard::buildMembershipTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(16); pl->setContentsMargins(40, 30, 40, 30);
    pl->setAlignment(Qt::AlignTop);

    auto* titleLbl = new QLabel("⭐  Membership Management");
    titleLbl->setObjectName("titleLabel");
    pl->addWidget(titleLbl);
    pl->addWidget(makeSep());

    // membership info card
    auto* card = new QFrame;
    card->setStyleSheet(CARD_STYLE);
    auto* cl = new QVBoxLayout(card);
    cl->setSpacing(10);

    memStatusLbl = new QLabel("Status: –");
    memStatusLbl->setStyleSheet("font-size:16px; font-weight:bold; color:#0D9488;");
    memExpiryLbl = new QLabel("Expires: –");
    memExpiryLbl->setStyleSheet("color:#64748B;");

    auto* benfits = new QLabel;
    int boost = (std::string(currentUser.type)=="STUDENT") ? 2
              : (std::string(currentUser.type)=="FACULTY") ? 2 : 1;
    int base  = (std::string(currentUser.type)=="STUDENT") ? 3
              : (std::string(currentUser.type)=="FACULTY") ? 5 : 1;
    benfits->setText(QString(
        "✅  15%% fine discount\n"
        "✅  Borrow limit: %1  →  %2 books\n"
        "✅  Duration: 1 Year").arg(base).arg(base+boost));
    benfits->setStyleSheet("color:#D1FAE5; font-size:14px; line-height:1.6;");

    cl->addWidget(memStatusLbl);
    cl->addWidget(memExpiryLbl);
    cl->addWidget(makeSep());
    cl->addWidget(benfits);

    auto* btnRow = new QHBoxLayout;
    buyMemBtn    = new QPushButton("⭐  Purchase Membership (1 Year)");
    cancelMemBtn = new QPushButton("❌  Cancel Membership");
    buyMemBtn->setObjectName("successBtn");
    buyMemBtn->setFixedHeight(42);
    cancelMemBtn->setObjectName("dangerBtn");
    cancelMemBtn->setFixedHeight(42);
    btnRow->addWidget(buyMemBtn);
    btnRow->addWidget(cancelMemBtn);
    cl->addLayout(btnRow);

    pl->addWidget(card);
    pl->addStretch();

    connect(buyMemBtn,    &QPushButton::clicked, this, &UserDashboard::onBuyMembership);
    connect(cancelMemBtn, &QPushButton::clicked, this, &UserDashboard::onCancelMembership);

    t->addTab(page, "⭐  Membership");
}

// ===========================================================
//  REFRESH
// ===========================================================
void UserDashboard::refreshAll() {
    // re-fetch currentUser from DB
    auto* ur = Database::instance().findUser(currentUser.id);
    if (ur) currentUser = *ur;

    loadDashboard();
    loadBooks();
    loadTransactions();
    loadFines();
    loadReservations();

    // membership tab
    bool hasMem = currentUser.hasMembership &&
                  currentUser.membershipExpiry > time(nullptr);
    memStatusLbl->setText(hasMem ? "✅  ACTIVE" : "❌  INACTIVE / NONE");
    memStatusLbl->setStyleSheet(hasMem
        ? "font-size:16px;font-weight:bold;color:#10B981;"
        : "font-size:16px;font-weight:bold;color:#EF4444;");
    if (hasMem)
        memExpiryLbl->setText("Expires: " +
            QString::fromStdString(Database::timeStr(currentUser.membershipExpiry)));
    else
        memExpiryLbl->setText("No active membership.");
    buyMemBtn->setEnabled(!hasMem);
    cancelMemBtn->setEnabled(hasMem);
}

void UserDashboard::loadDashboard() {
    auto& db = Database::instance();
    // balance
    balanceLbl->setText(QString("Rs. %1").arg(currentUser.balance, 0, 'f', 0));
    // membership
    bool hasMem = currentUser.hasMembership &&
                  currentUser.membershipExpiry > time(nullptr);
    membershipLbl->setText(hasMem ? "ACTIVE" : "NONE");
    // borrowed
    int borCount = 0;
    for (auto& t : db.transactions)
        if (t.uid == currentUser.id && !t.returned) borCount++;
    borrowedLbl->setText(QString::number(borCount));

    // live fines
    double liveFine = calcLiveFine();
    finesLbl->setText(QString("Rs. %1").arg(liveFine, 0, 'f', 0));

    // borrowed books table
    borrowedTable->setRowCount(0);
    time_t now = time(nullptr);
    for (auto& txn : db.transactions) {
        if (txn.uid != currentUser.id || txn.returned) continue;
        auto* bk = db.findBook(txn.bid);
        int row = borrowedTable->rowCount();
        borrowedTable->insertRow(row);
        borrowedTable->setItem(row,0, new QTableWidgetItem(QString::number(txn.bid)));
        borrowedTable->setItem(row,1, new QTableWidgetItem(bk ? bk->title : "?"));
        borrowedTable->setItem(row,2, new QTableWidgetItem(bk ? bk->author : "?"));
        borrowedTable->setItem(row,3, new QTableWidgetItem(
            QString::fromStdString(Database::timeStr(txn.due_time))));
        bool overdue = (now > txn.due_time);
        auto* statusItem = new QTableWidgetItem(overdue ? "⚠ OVERDUE" : "✅ On Time");
        statusItem->setForeground(overdue ? QColor("#EF4444") : QColor("#10B981"));
        borrowedTable->setItem(row,4, statusItem);
    }
}

void UserDashboard::loadBooks() {
    auto& db = Database::instance();
    bookTable->setRowCount(0);
    for (auto& b : db.books) {
        int row = bookTable->rowCount();
        bookTable->insertRow(row);
        bookTable->setItem(row,0, new QTableWidgetItem(QString::number(b.id)));
        bookTable->setItem(row,1, new QTableWidgetItem(b.title));
        bookTable->setItem(row,2, new QTableWidgetItem(b.author));
        bookTable->setItem(row,3, new QTableWidgetItem(b.category));
        bookTable->setItem(row,4, new QTableWidgetItem(b.isbn));
        bookTable->setItem(row,5, new QTableWidgetItem(b.publisher));
        bookTable->setItem(row,6, new QTableWidgetItem(QString::number(b.year)));
        auto* copItem = new QTableWidgetItem(QString::number(b.copies));
        copItem->setForeground(b.copies > 0 ? QColor("#10B981") : QColor("#EF4444"));
        bookTable->setItem(row,7, copItem);
    }
}

void UserDashboard::loadTransactions() {
    auto& db = Database::instance();
    txnTable->setRowCount(0);
    for (auto& t : db.transactions) {
        if (t.uid != currentUser.id) continue;
        int row = txnTable->rowCount();
        txnTable->insertRow(row);
        txnTable->setItem(row,0, new QTableWidgetItem(QString::number(t.tid)));
        txnTable->setItem(row,1, new QTableWidgetItem(QString::number(t.bid)));
        txnTable->setItem(row,2, new QTableWidgetItem(
            QString::fromStdString(Database::timeStr(t.borrow_time))));
        txnTable->setItem(row,3, new QTableWidgetItem(
            QString::fromStdString(Database::timeStr(t.due_time))));
        txnTable->setItem(row,4, new QTableWidgetItem(
            t.returned ? QString::fromStdString(Database::timeStr(t.return_time)) : "–"));
        auto* retItem = new QTableWidgetItem(t.returned ? "Yes" : "No");
        retItem->setForeground(t.returned ? QColor("#10B981") : QColor("#F59E0B"));
        txnTable->setItem(row,5, retItem);
        int overMin = 0;
        if (t.returned && t.return_time > t.due_time)
            overMin = Database::minutesDiff(t.due_time, t.return_time);
        txnTable->setItem(row,6, new QTableWidgetItem(QString::number(overMin)));
    }
}

void UserDashboard::loadFines() {
    auto& db = Database::instance();
    finesTable->setRowCount(0);
    for (auto& f : db.fines) {
        if (f.user_id != currentUser.id) continue;
        int row = finesTable->rowCount();
        finesTable->insertRow(row);
        finesTable->setItem(row,0, new QTableWidgetItem(QString::number(f.book_id)));
        finesTable->setItem(row,1, new QTableWidgetItem(QString::number(f.minuteslate)));
        finesTable->setItem(row,2, new QTableWidgetItem(QString("Rs. %1").arg(f.rate)));
        finesTable->setItem(row,3, new QTableWidgetItem(QString("Rs. %1").arg(f.originalAmount, 0,'f',2)));
        finesTable->setItem(row,4, new QTableWidgetItem(f.discountApplied ? "15%" : "–"));
        finesTable->setItem(row,5, new QTableWidgetItem(QString("Rs. %1").arg(f.finalAmount, 0,'f',2)));
    }
}

void UserDashboard::loadReservations() {
    auto& db = Database::instance();
    // auto-expire
    time_t now = time(nullptr);
    for (auto& r : db.reservations) {
        if (r.active) {
            // parse endTime + date to time_t (simplified)
            struct tm tm{};
            sscanf(r.date, "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year);
            tm.tm_mon -= 1; tm.tm_year -= 1900;
            int eh, em;
            sscanf(r.endTime, "%d:%d", &eh, &em);
            tm.tm_hour = eh; tm.tm_min = em; tm.tm_isdst = -1;
            if (mktime(&tm) < now) r.active = false;
        }
    }
    db.rewriteReservations();

    resTable->setRowCount(0);
    for (auto& r : db.reservations) {
        if (r.userid != currentUser.id || !r.active) continue;
        int row = resTable->rowCount();
        resTable->insertRow(row);
        resTable->setItem(row,0, new QTableWidgetItem(QString::number(r.reservationid)));
        resTable->setItem(row,1, new QTableWidgetItem(r.bookid ? "Book" : "Table"));
        resTable->setItem(row,2, new QTableWidgetItem(
            QString::number(r.bookid ? r.bookid : r.tableid)));
        resTable->setItem(row,3, new QTableWidgetItem(r.date));
        resTable->setItem(row,4, new QTableWidgetItem(r.startTime));
        resTable->setItem(row,5, new QTableWidgetItem(r.endTime));
    }
}

// ===========================================================
//  HELPERS
// ===========================================================
int UserDashboard::getFineRate() {
    std::string t = currentUser.type;
    if (t == "STUDENT") return 10;
    if (t == "FACULTY") return 15;
    return 20;
}
int UserDashboard::getEffectiveBorrowLimit() {
    std::string t = currentUser.type;
    int base = (t=="STUDENT") ? 3 : (t=="FACULTY") ? 5 : 1;
    bool hasMem = currentUser.hasMembership && currentUser.membershipExpiry > time(nullptr);
    int boost   = hasMem ? ((t=="GUEST") ? 1 : 2) : 0;
    return base + boost;
}
bool UserDashboard::hasOverdueBooks() {
    auto& db = Database::instance();
    time_t now = time(nullptr);
    for (auto& t : db.transactions)
        if (t.uid == currentUser.id && !t.returned && now > t.due_time) return true;
    return false;
}
bool UserDashboard::hasActiveMembership() {
    return currentUser.hasMembership && currentUser.membershipExpiry > time(nullptr);
}
double UserDashboard::calcLiveFine() {
    auto& db = Database::instance();
    time_t now = time(nullptr);
    double total = 0;
    for (auto& t : db.transactions) {
        if (t.uid == currentUser.id && !t.returned && now > t.due_time)
            total += Database::minutesDiff(t.due_time, now) * getFineRate();
    }
    if (hasActiveMembership()) total *= (1.0 - Database::FINE_DISCOUNT);
    return total;
}

// ===========================================================
//  BORROW
// ===========================================================
void UserDashboard::onBorrow() {
    auto& db = Database::instance();

    if (hasOverdueBooks()) {
        QMessageBox::warning(this, "Overdue Books",
            "⚠  You have overdue books! Please return them before borrowing.");
        return;
    }

    // Count active borrows
    int activeBorrows = 0;
    for (auto& t : db.transactions)
        if (t.uid == currentUser.id && !t.returned) activeBorrows++;

    if (activeBorrows >= getEffectiveBorrowLimit()) {
        QMessageBox::warning(this, "Borrow Limit",
            QString("You have reached your borrow limit (%1 books).").arg(getEffectiveBorrowLimit()));
        return;
    }
    if (currentUser.balance <= -Database::MAX_UNPAID) {
        QMessageBox::warning(this, "Balance Issue",
            "Your unpaid balance exceeds the limit. Please add funds.");
        return;
    }

    // get selected row
    int row = bookTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this,"Select Book","Please select a book from the table first.");
        return;
    }
    int bid = bookTable->item(row,0)->text().toInt();
    auto* bk = db.findBook(bid);
    if (!bk || bk->copies <= 0) {
        QMessageBox::warning(this,"No Copies","No copies available for this book.");
        return;
    }

    // Ask duration
    QDialog dlg(this);
    dlg.setWindowTitle("Borrow Duration");
    dlg.setStyleSheet("background-color:#1E2D3D;");
    auto* fl = new QFormLayout(&dlg);
    fl->setContentsMargins(20,20,20,20);
    auto* unitCombo = new QComboBox; unitCombo->addItems({"Minutes","Hours","Days"});
    auto* valSpin   = new QSpinBox;  valSpin->setRange(1, 10080);
    fl->addRow("Duration:", valSpin);
    fl->addRow("Unit:",     unitCombo);
    auto* okBtn = new QPushButton("Borrow"); okBtn->setObjectName("successBtn");
    fl->addRow(okBtn);
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    if (dlg.exec() != QDialog::Accepted) return;

    int mins = valSpin->value();
    if (unitCombo->currentIndex() == 1) mins *= 60;
    if (unitCombo->currentIndex() == 2) mins *= 1440;

    // Create transaction
    TransactionRecord txn{};
    txn.tid        = db.nextTxnID++;
    txn.uid        = currentUser.id;
    txn.bid        = bid;
    txn.borrow_time= time(nullptr);
    txn.due_time   = Database::addMinutes(txn.borrow_time, mins);
    txn.returned   = false;
    strncpy(txn.return_date, "NOT RETURNED", 29);

    db.transactions.push_back(txn);
    db.appendTransaction(txn);

    bk->copies--;
    bk->times_borrowed++;
    db.rewriteBooks();

    // update user daily borrow + balance check (just persist)
    db.rewriteUsers();

    QMessageBox::information(this,"Borrowed!",
        QString("✅  '%1' borrowed successfully!\nDue: %2\nFine rate: Rs.%3/min if late.")
        .arg(bk->title)
        .arg(QString::fromStdString(Database::timeStr(txn.due_time)))
        .arg(getFineRate()));
    refreshAll();
}

// ===========================================================
//  RETURN
// ===========================================================
void UserDashboard::onReturn() {
    auto& db = Database::instance();

    // Build list of active borrows
    QStringList items;
    std::vector<int> tids;
    for (auto& t : db.transactions) {
        if (t.uid != currentUser.id || t.returned) continue;
        auto* bk = db.findBook(t.bid);
        items << QString("[TxnID:%1] %2 – Due: %3")
                    .arg(t.tid)
                    .arg(bk ? bk->title : "?")
                    .arg(QString::fromStdString(Database::timeStr(t.due_time)));
        tids.push_back(t.tid);
    }
    if (items.isEmpty()) {
        QMessageBox::information(this,"Return","You have no books to return.");
        return;
    }

    bool ok;
    QString sel = QInputDialog::getItem(this, "Return Book",
                                        "Select book to return:", items, 0, false, &ok);
    if (!ok) return;

    int idx = items.indexOf(sel);
    int tid  = tids[idx];

    // Find transaction
    for (auto& t : db.transactions) {
        if (t.tid != tid) continue;
        t.returned    = true;
        t.return_time = time(nullptr);
        snprintf(t.return_date, 30, "%s",
                 Database::timeStr(t.return_time).c_str());

        // restore copy
        auto* bk = db.findBook(t.bid);
        if (bk) bk->copies++;
        db.rewriteBooks();
        db.rewriteTransactions();

        // fine
        int overMin = 0;
        if (t.return_time > t.due_time)
            overMin = Database::minutesDiff(t.due_time, t.return_time);

        if (overMin > 0) {
            double orig  = overMin * getFineRate();
            double final_= orig;
            bool disc    = hasActiveMembership();
            if (disc) final_ *= (1.0 - Database::FINE_DISCOUNT);

            FineRecord fr{};
            fr.user_id       = currentUser.id;
            fr.book_id       = t.bid;
            fr.minuteslate   = overMin;
            fr.rate          = getFineRate();
            fr.originalAmount= orig;
            fr.finalAmount   = final_;
            fr.discountApplied = disc;
            db.fines.push_back(fr);
            db.appendFine(fr);

            // deduct from balance
            auto* ur = db.findUser(currentUser.id);
            if (ur) {
                ur->balance -= final_;
                ur->total_fines_paid += final_;
                db.rewriteUsers();
                currentUser = *ur;
            }

            QMessageBox::warning(this,"Fine Generated",
                QString("⚠  Book returned LATE by %1 minutes.\n"
                        "Original fine: Rs. %2\n%3"
                        "Final fine: Rs. %4\nDeducted from balance.")
                .arg(overMin).arg(orig,0,'f',2)
                .arg(disc ? "Membership discount (15%) applied.\n" : "")
                .arg(final_,0,'f',2));
        } else {
            QMessageBox::information(this,"Returned","✅  Book returned on time! No fine.");
        }
        break;
    }
    refreshAll();
}

// ===========================================================
//  READ ON-SITE
// ===========================================================
void UserDashboard::onReadOnSite() {
    int row = bookTable->currentRow();
    if (row < 0) { QMessageBox::information(this,"Select","Select a book first."); return; }
    int bid = bookTable->item(row,0)->text().toInt();
    auto* bk = Database::instance().findBook(bid);
    if (!bk) return;
    bk->read_onsite_count++;
    Database::instance().rewriteBooks();
    QMessageBox::information(this,"Read On-Site",
        QString("📖  Reading '%1' on-site.\nRead on-site count: %2").arg(bk->title).arg(bk->read_onsite_count));
    loadBooks();
}

// ===========================================================
//  DOWNLOAD
// ===========================================================
void UserDashboard::onDownload() {
    int row = bookTable->currentRow();
    if (row < 0) { QMessageBox::information(this,"Select","Select a book first."); return; }
    int bid = bookTable->item(row,0)->text().toInt();
    auto* bk = Database::instance().findBook(bid);
    if (!bk) return;
    bk->download_count++;
    Database::instance().rewriteBooks();
    QMessageBox::information(this,"Downloaded",
        QString("⬇  '%1' downloaded.\nDownload count: %2").arg(bk->title).arg(bk->download_count));
    loadBooks();
}

// ===========================================================
//  RESERVE TABLE
// ===========================================================
void UserDashboard::onReserveTable() {
    auto& db = Database::instance();
    if (db.tables.empty()) {
        QMessageBox::warning(this,"No Tables","No tables available in the library.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Reserve Table");
    dlg.setStyleSheet("background-color:#1E2D3D; color:#E2E8F0;");
    auto* fl = new QFormLayout(&dlg);
    fl->setContentsMargins(20,20,20,20); fl->setSpacing(10);

    auto* datEdit  = new QLineEdit; datEdit->setPlaceholderText("DD/MM/YYYY");
    auto* startEdit= new QLineEdit; startEdit->setPlaceholderText("HH:MM (24h)");
    auto* endEdit  = new QLineEdit; endEdit->setPlaceholderText("HH:MM (24h)");
    auto* autoChk  = new QComboBox; autoChk->addItems({"Auto-assign","Manual (pick table)"});

    fl->addRow("Date:",    datEdit);
    fl->addRow("Start:",   startEdit);
    fl->addRow("End:",     endEdit);
    fl->addRow("Mode:",    autoChk);

    auto* okBtn = new QPushButton("Reserve"); okBtn->setObjectName("successBtn");
    fl->addRow(okBtn);
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    if (dlg.exec() != QDialog::Accepted) return;

    QString date  = datEdit->text().trimmed();
    QString start = startEdit->text().trimmed();
    QString end   = endEdit->text().trimmed();
    if (date.isEmpty() || start.isEmpty() || end.isEmpty()) return;

    if (autoChk->currentIndex() == 0) {
        // auto-assign
        int tid = -1;
        for (auto& tbl : db.tables) {
            bool conflict = false;
            for (auto& r : db.reservations) {
                if (r.tableid == tbl.id && r.date == date.toStdString() && r.active) {
                    // simple string compare for overlap
                    QString rs = r.startTime, re = r.endTime;
                    if (!(re <= start || end <= rs)) { conflict = true; break; }
                }
            }
            if (!conflict) { tid = tbl.id; break; }
        }
        if (tid < 0) {
            QMessageBox::warning(this,"Full","No tables available for that time slot.");
            return;
        }
        ReservationRecord rr{};
        rr.reservationid = db.nextResID++;
        rr.userid = currentUser.id;
        rr.tableid= tid;
        rr.bookid = 0;
        rr.active = true;
        strncpy(rr.date, date.toStdString().c_str(), 14);
        strncpy(rr.startTime, start.toStdString().c_str(), 9);
        strncpy(rr.endTime,   end.toStdString().c_str(),   9);
        db.reservations.push_back(rr);
        db.appendReservation(rr);
        QMessageBox::information(this,"Reserved",
            QString("✅  Auto-assigned Table ID %1.").arg(tid));
    } else {
        // manual
        QStringList tblItems;
        for (auto& tbl : db.tables)
            tblItems << QString("Table %1 – %2 (cap %3)").arg(tbl.id).arg(tbl.location).arg(tbl.capacity);
        bool ok2;
        QString sel = QInputDialog::getItem(this,"Pick Table","Table:", tblItems, 0, false, &ok2);
        if (!ok2) return;
        int tidx = tblItems.indexOf(sel);
        int tid  = db.tables[tidx].id;
        // check conflict
        for (auto& r : db.reservations) {
            if (r.tableid == tid && r.date == date.toStdString() && r.active) {
                QString rs = r.startTime, re = r.endTime;
                if (!(re <= start || end <= rs)) {
                    QMessageBox::warning(this,"Conflict","That table is already booked for that time.");
                    return;
                }
            }
        }
        ReservationRecord rr{};
        rr.reservationid = db.nextResID++;
        rr.userid = currentUser.id;
        rr.tableid= tid; rr.bookid = 0; rr.active = true;
        strncpy(rr.date, date.toStdString().c_str(), 14);
        strncpy(rr.startTime, start.toStdString().c_str(), 9);
        strncpy(rr.endTime,   end.toStdString().c_str(),   9);
        db.reservations.push_back(rr);
        db.appendReservation(rr);
        QMessageBox::information(this,"Reserved","✅  Table reserved successfully!");
    }
    loadReservations();
}

// ===========================================================
//  RESERVE BOOK
// ===========================================================
void UserDashboard::onReserveBook() {
    auto& db = Database::instance();
    int row = bookTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this,"Select","Select a book from the Books tab first.");
        return;
    }
    int bid = bookTable->item(row,0)->text().toInt();
    auto* bk = db.findBook(bid);
    if (!bk || bk->copies <= 0) {
        QMessageBox::warning(this,"No Copies","No copies available to reserve.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("Reserve Book");
    dlg.setStyleSheet("background-color:#1E2D3D;");
    auto* fl = new QFormLayout(&dlg);
    fl->setContentsMargins(20,20,20,20); fl->setSpacing(10);
    auto* datEdit   = new QLineEdit; datEdit->setPlaceholderText("DD/MM/YYYY");
    auto* startEdit = new QLineEdit; startEdit->setPlaceholderText("HH:MM (24h)");
    auto* endEdit   = new QLineEdit; endEdit->setPlaceholderText("HH:MM (24h)");
    fl->addRow("Date:",  datEdit);
    fl->addRow("Start:", startEdit);
    fl->addRow("End:",   endEdit);
    auto* okBtn = new QPushButton("Reserve"); okBtn->setObjectName("successBtn");
    fl->addRow(okBtn);
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    if (dlg.exec() != QDialog::Accepted) return;

    ReservationRecord rr{};
    rr.reservationid = db.nextResID++;
    rr.userid = currentUser.id;
    rr.bookid = bid; rr.tableid = 0; rr.active = true;
    strncpy(rr.date,      datEdit->text().trimmed().toStdString().c_str(), 14);
    strncpy(rr.startTime, startEdit->text().trimmed().toStdString().c_str(), 9);
    strncpy(rr.endTime,   endEdit->text().trimmed().toStdString().c_str(),   9);
    db.reservations.push_back(rr);
    db.appendReservation(rr);
    bk->copies--;
    db.rewriteBooks();
    QMessageBox::information(this,"Reserved",
        QString("✅  Book '%1' reserved!").arg(bk->title));
    refreshAll();
}

// ===========================================================
//  CANCEL RESERVATION
// ===========================================================
void UserDashboard::onCancelReservation() {
    auto& db = Database::instance();
    int row = resTable->currentRow();
    if (row < 0) {
        QMessageBox::information(this,"Select","Select a reservation to cancel.");
        return;
    }
    int rid = resTable->item(row,0)->text().toInt();
    for (auto& r : db.reservations) {
        if (r.reservationid == rid && r.userid == currentUser.id) {
            r.active = false;
            db.rewriteReservations();
            QMessageBox::information(this,"Cancelled","❌  Reservation cancelled.");
            loadReservations();
            return;
        }
    }
    QMessageBox::warning(this,"Not Found","Reservation not found or not yours.");
}

// ===========================================================
//  MEMBERSHIP
// ===========================================================
void UserDashboard::onBuyMembership() {
    if (hasActiveMembership()) {
        QMessageBox::information(this,"Membership","You already have an active membership.");
        return;
    }
    auto res = QMessageBox::question(this, "Purchase Membership",
        "Activate 1-year membership?\n✅ 15% fine discount\n✅ Higher borrow limit",
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;

    auto* ur = Database::instance().findUser(currentUser.id);
    if (!ur) return;
    ur->hasMembership          = true;
    ur->membershipActivation   = time(nullptr);
    ur->membershipExpiry       = Database::addMinutes(ur->membershipActivation, 365*24*60);
    ur->membershipDurationDays = 365;
    Database::instance().rewriteUsers();
    currentUser = *ur;
    QMessageBox::information(this,"Membership",
        QString("⭐  Membership activated!\nExpires: %1")
        .arg(QString::fromStdString(Database::timeStr(ur->membershipExpiry))));
    refreshAll();
}

void UserDashboard::onCancelMembership() {
    if (!hasActiveMembership()) {
        QMessageBox::information(this,"Membership","No active membership to cancel.");
        return;
    }
    auto res = QMessageBox::question(this, "Cancel Membership",
        "Are you sure you want to cancel your membership?",
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;
    auto* ur = Database::instance().findUser(currentUser.id);
    if (!ur) return;
    ur->hasMembership  = false;
    ur->membershipExpiry = 0;
    Database::instance().rewriteUsers();
    currentUser = *ur;
    QMessageBox::information(this,"Cancelled","Membership cancelled.");
    refreshAll();
}

// ===========================================================
//  ADD BALANCE
// ===========================================================
void UserDashboard::onAddBalance() {
    bool ok;
    double amount = QInputDialog::getDouble(this, "Add Balance",
                        "Enter amount to add (Rs.):", 0, 10, 999999, 2, &ok);
    if (!ok || amount <= 0) return;
    auto* ur = Database::instance().findUser(currentUser.id);
    if (!ur) return;
    ur->balance += amount;
    Database::instance().rewriteUsers();
    currentUser = *ur;
    QMessageBox::information(this,"Balance Added",
        QString("✅  Rs.%1 added.\nNew Balance: Rs.%2").arg(amount).arg(ur->balance));
    refreshAll();
}

// ===========================================================
//  UPDATE PROFILE
// ===========================================================
void UserDashboard::onUpdateProfile() {
    auto& db = Database::instance();
    auto* ur = db.findUser(currentUser.id);
    if (!ur) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Update Profile");
    dlg.setStyleSheet("background-color:#1E2D3D;");
    auto* fl = new QFormLayout(&dlg);
    fl->setContentsMargins(24,20,24,20); fl->setSpacing(10);

    auto* uEdit = new QLineEdit(ur->username);
    auto* pEdit = new QLineEdit;
    pEdit->setEchoMode(QLineEdit::Password);
    pEdit->setPlaceholderText("Leave blank to keep current");
    auto* phEdit= new QLineEdit(ur->phone);

    fl->addRow("Username:", uEdit);
    fl->addRow("New Password:", pEdit);
    fl->addRow("Phone:", phEdit);

    auto* okBtn = new QPushButton("Save"); okBtn->setObjectName("successBtn");
    fl->addRow(okBtn);
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    if (dlg.exec() != QDialog::Accepted) return;

    std::string newUname = uEdit->text().trimmed().toStdString();
    // check duplicate username
    for (auto& u : db.users)
        if (u.id != ur->id && std::string(u.username) == newUname) {
            QMessageBox::warning(this,"Duplicate","Username already taken.");
            return;
        }
    strncpy(ur->username, newUname.c_str(), 49);
    if (!pEdit->text().isEmpty() && pEdit->text().length() >= 6)
        strncpy(ur->password, pEdit->text().toStdString().c_str(), 49);
    strncpy(ur->phone, phEdit->text().trimmed().toStdString().c_str(), 19);
    db.rewriteUsers();
    currentUser = *ur;
    QMessageBox::information(this,"Updated","✅  Profile updated.");
    refreshAll();
}
