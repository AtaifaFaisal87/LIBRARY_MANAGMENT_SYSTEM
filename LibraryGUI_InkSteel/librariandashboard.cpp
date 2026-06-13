#include "librariandashboard.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QMenuBar>
#include <QStatusBar>
#include <cstring>
#include <ctime>

static QFrame* libSep() {
    auto* f = new QFrame; f->setFrameShape(QFrame::HLine);
    f->setStyleSheet("color:#2D4059;"); return f;
}

static QTableWidget* makeTable(const QStringList& headers) {
    auto* t = new QTableWidget(0, headers.size());
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    t->setAlternatingRowColors(true);
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    return t;
}

LibrarianDashboard::LibrarianDashboard(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Library System – Librarian Panel");
    resize(1200, 760);

    auto* mb = menuBar();
    auto* fm = mb->addMenu("File");
    auto* logoutAct = fm->addAction("🔓 Logout");
    connect(logoutAct, &QAction::triggered, this, &QMainWindow::close);

    auto* central    = new QWidget;
    auto* rootLayout = new QVBoxLayout(central);
    rootLayout->setContentsMargins(10,10,10,10);
    rootLayout->setSpacing(8);

    // banner
    auto* banner = new QFrame;
    banner->setStyleSheet(
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0,"
        "stop:0 #16212E, stop:1 #1E2D3D); border-radius:8px; padding:10px;");
    auto* bl = new QHBoxLayout(banner);
    auto* ttl = new QLabel("🔑  Librarian Administration Panel");
    ttl->setStyleSheet("color:#FFFFFF; font-size:18px; font-weight:bold; background:transparent;");
    auto* logBtn = new QPushButton("🔓 Logout");
    logBtn->setStyleSheet("background:rgba(255,255,255,0.2); color:#FFFFFF;"
                          "border-radius:5px; padding:6px 14px;");
    connect(logBtn, &QPushButton::clicked, this, &QMainWindow::close);
    bl->addWidget(ttl); bl->addStretch(); bl->addWidget(logBtn);
    rootLayout->addWidget(banner);

    tabs = new QTabWidget;
    buildOverviewTab(tabs);
    buildUsersTab(tabs);
    buildBooksTab(tabs);
    buildTransactionsTab(tabs);
    buildFinesTab(tabs);
    buildTablesTab(tabs);
    buildReservationsTab(tabs);
    rootLayout->addWidget(tabs, 1);

    setCentralWidget(central);
    statusBar()->showMessage("Librarian Admin  |  All operations here are permanent.");
    refreshAll();
}

// ===========================================================
//  BUILD TABS
// ===========================================================
void LibrarianDashboard::buildOverviewTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(14); pl->setContentsMargins(14,14,14,14);

    auto* hdr = new QLabel("📊  System Overview");
    hdr->setObjectName("sectionLabel"); hdr->setStyleSheet("color:#0D9488;font-size:18px;font-weight:bold;");
    pl->addWidget(hdr); pl->addWidget(libSep());

    auto* grid = new QGridLayout; grid->setSpacing(14);

    auto makeCard = [](const QString& icon, const QString& lbl,
                       const QString& sty, QLabel*& valLbl) -> QWidget* {
        auto* card = new QFrame;
        card->setStyleSheet(sty);
        card->setMinimumHeight(100);
        auto* cl = new QVBoxLayout(card); cl->setAlignment(Qt::AlignCenter);
        auto* il = new QLabel(icon); il->setAlignment(Qt::AlignCenter);
        il->setStyleSheet("font-size:28px;background:transparent;");
        valLbl = new QLabel("0"); valLbl->setObjectName("statValue");
        valLbl->setAlignment(Qt::AlignCenter);
        auto* ll = new QLabel(lbl); ll->setObjectName("statLabel"); ll->setAlignment(Qt::AlignCenter);
        cl->addWidget(il); cl->addWidget(valLbl); cl->addWidget(ll);
        return card;
    };

    grid->addWidget(makeCard("👥","Total Active Users",STAT_CARD_STYLE_BLUE,  totalUsersLbl), 0,0);
    grid->addWidget(makeCard("📚","Total Books",       STAT_CARD_STYLE_CYAN,  totalBooksLbl), 0,1);
    grid->addWidget(makeCard("📋","Transactions",      STAT_CARD_STYLE_GREEN, totalTxnsLbl),  0,2);
    grid->addWidget(makeCard("⚠","Total Fines",       STAT_CARD_STYLE_ROSE,  totalFinesLbl), 0,3);

    pl->addLayout(grid);
    pl->addStretch();
    t->addTab(page, "📊  Overview");
}

void LibrarianDashboard::buildUsersTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14,14,14,14);

    auto* btnRow = new QHBoxLayout;
    auto* viewBtn   = new QPushButton("👁  View Details");
    auto* deleteBtn = new QPushButton("🗑  Delete User");
    viewBtn->setObjectName("cyanBtn");
    deleteBtn->setObjectName("dangerBtn");
    btnRow->addWidget(viewBtn); btnRow->addWidget(deleteBtn); btnRow->addStretch();
    pl->addLayout(btnRow);

    usersTable = makeTable({"ID","Name","Email","Phone","Type","Status","Membership","Limit","Balance"});
    pl->addWidget(usersTable, 1);

    connect(viewBtn,   &QPushButton::clicked, this, &LibrarianDashboard::onViewUserDetails);
    connect(deleteBtn, &QPushButton::clicked, this, &LibrarianDashboard::onDeleteUser);

    t->addTab(page, "👥  Users");
}

void LibrarianDashboard::buildBooksTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14,14,14,14);

    auto* btnRow = new QHBoxLayout;
    auto* addBtn = new QPushButton("➕  Add Book");
    auto* rmBtn  = new QPushButton("🗑  Remove Book");
    addBtn->setObjectName("successBtn");
    rmBtn->setObjectName("dangerBtn");
    btnRow->addWidget(addBtn); btnRow->addWidget(rmBtn); btnRow->addStretch();
    pl->addLayout(btnRow);

    booksTable = makeTable({"ID","Title","Author","Category","ISBN","Publisher","Year","Copies","Downloads","ReadOnSite","Borrowed"});
    pl->addWidget(booksTable, 1);

    connect(addBtn, &QPushButton::clicked, this, &LibrarianDashboard::onAddBook);
    connect(rmBtn,  &QPushButton::clicked, this, &LibrarianDashboard::onRemoveBook);

    t->addTab(page, "📚  Books");
}

void LibrarianDashboard::buildTransactionsTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14,14,14,14);

    auto* lbl = new QLabel("📋  All Transactions");
    lbl->setObjectName("sectionLabel");
    pl->addWidget(lbl);

    txnsTable = makeTable({"TxnID","UserID","BookID","Borrowed","Due","Returned","Status"});
    pl->addWidget(txnsTable, 1);

    t->addTab(page, "📋  Transactions");
}

void LibrarianDashboard::buildFinesTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14,14,14,14);

    auto* lbl = new QLabel("⚠  All Fines");
    lbl->setObjectName("sectionLabel");
    pl->addWidget(lbl);

    finesTable = makeTable({"UserID","BookID","Mins Late","Rate","Original","Discount","Final"});
    pl->addWidget(finesTable, 1);

    t->addTab(page, "⚠  Fines");
}

void LibrarianDashboard::buildTablesTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14,14,14,14);

    auto* btnRow = new QHBoxLayout;
    auto* addBtn = new QPushButton("➕  Add Table");
    auto* rmBtn  = new QPushButton("🗑  Remove Table");
    addBtn->setObjectName("successBtn"); rmBtn->setObjectName("dangerBtn");
    btnRow->addWidget(addBtn); btnRow->addWidget(rmBtn); btnRow->addStretch();
    pl->addLayout(btnRow);

    tablesTable = makeTable({"Table ID","Location","Capacity"});
    pl->addWidget(tablesTable, 1);

    connect(addBtn, &QPushButton::clicked, this, &LibrarianDashboard::onAddTable);
    connect(rmBtn,  &QPushButton::clicked, this, &LibrarianDashboard::onRemoveTable);

    t->addTab(page, "🪑  Tables");
}

void LibrarianDashboard::buildReservationsTab(QTabWidget* t) {
    auto* page = new QWidget;
    auto* pl   = new QVBoxLayout(page);
    pl->setSpacing(10); pl->setContentsMargins(14,14,14,14);

    reservationsTable = makeTable({"ResID","UserID","Type","TargetID","Date","Start","End","Active"});
    pl->addWidget(reservationsTable, 1);

    t->addTab(page, "📅  Reservations");
}

// ===========================================================
//  REFRESH
// ===========================================================
void LibrarianDashboard::refreshAll() {
    loadOverview();
    loadUsers();
    loadBooks();
    loadTransactions();
    loadFines();
    loadTables();
    loadReservations();
}

void LibrarianDashboard::loadOverview() {
    auto& db = Database::instance();
    int active = 0;
    for (auto& u : db.users) if (!u.isDeleted) active++;
    totalUsersLbl->setText(QString::number(active));
    totalBooksLbl->setText(QString::number(db.books.size()));
    totalTxnsLbl->setText(QString::number(db.transactions.size()));
    double totalFine = 0;
    for (auto& f : db.fines) totalFine += f.finalAmount;
    totalFinesLbl->setText(QString("Rs.%1").arg(totalFine,0,'f',0));
}

void LibrarianDashboard::loadUsers() {
    auto& db = Database::instance();
    usersTable->setRowCount(0);
    for (auto& u : db.users) {
        int row = usersTable->rowCount();
        usersTable->insertRow(row);
        usersTable->setItem(row,0, new QTableWidgetItem(QString::number(u.id)));
        usersTable->setItem(row,1, new QTableWidgetItem(u.name));
        usersTable->setItem(row,2, new QTableWidgetItem(u.email));
        usersTable->setItem(row,3, new QTableWidgetItem(u.phone));
        usersTable->setItem(row,4, new QTableWidgetItem(u.type));
        auto* si = new QTableWidgetItem(u.isDeleted ? "DELETED" : "ACTIVE");
        si->setForeground(u.isDeleted ? QColor("#EF4444") : QColor("#10B981"));
        usersTable->setItem(row,5, si);
        bool hasMem = u.hasMembership && u.membershipExpiry > time(nullptr);
        usersTable->setItem(row,6, new QTableWidgetItem(hasMem ? "✅ Yes" : "No"));
        std::string t = u.type;
        int base  = (t=="STUDENT") ? 3 : (t=="FACULTY") ? 5 : 1;
        int boost = hasMem ? ((t=="GUEST") ? 1 : 2) : 0;
        usersTable->setItem(row,7, new QTableWidgetItem(QString::number(base+boost)));
        usersTable->setItem(row,8, new QTableWidgetItem(
            QString("Rs.%1").arg(u.balance,0,'f',0)));
    }
}

void LibrarianDashboard::loadBooks() {
    auto& db = Database::instance();
    booksTable->setRowCount(0);
    for (auto& b : db.books) {
        int row = booksTable->rowCount();
        booksTable->insertRow(row);
        booksTable->setItem(row,0,  new QTableWidgetItem(QString::number(b.id)));
        booksTable->setItem(row,1,  new QTableWidgetItem(b.title));
        booksTable->setItem(row,2,  new QTableWidgetItem(b.author));
        booksTable->setItem(row,3,  new QTableWidgetItem(b.category));
        booksTable->setItem(row,4,  new QTableWidgetItem(b.isbn));
        booksTable->setItem(row,5,  new QTableWidgetItem(b.publisher));
        booksTable->setItem(row,6,  new QTableWidgetItem(QString::number(b.year)));
        auto* ci = new QTableWidgetItem(QString::number(b.copies));
        ci->setForeground(b.copies > 0 ? QColor("#10B981") : QColor("#EF4444"));
        booksTable->setItem(row,7,  ci);
        booksTable->setItem(row,8,  new QTableWidgetItem(QString::number(b.download_count)));
        booksTable->setItem(row,9,  new QTableWidgetItem(QString::number(b.read_onsite_count)));
        booksTable->setItem(row,10, new QTableWidgetItem(QString::number(b.times_borrowed)));
    }
}

void LibrarianDashboard::loadTransactions() {
    auto& db = Database::instance();
    txnsTable->setRowCount(0);
    for (auto& t : db.transactions) {
        int row = txnsTable->rowCount();
        txnsTable->insertRow(row);
        txnsTable->setItem(row,0, new QTableWidgetItem(QString::number(t.tid)));
        txnsTable->setItem(row,1, new QTableWidgetItem(QString::number(t.uid)));
        txnsTable->setItem(row,2, new QTableWidgetItem(QString::number(t.bid)));
        txnsTable->setItem(row,3, new QTableWidgetItem(
            QString::fromStdString(Database::timeStr(t.borrow_time))));
        txnsTable->setItem(row,4, new QTableWidgetItem(
            QString::fromStdString(Database::timeStr(t.due_time))));
        txnsTable->setItem(row,5, new QTableWidgetItem(
            t.returned ? QString::fromStdString(Database::timeStr(t.return_time)) : "–"));
        bool overdue = !t.returned && time(nullptr) > t.due_time;
        auto* si = new QTableWidgetItem(t.returned ? "Returned" : overdue ? "⚠ OVERDUE" : "Active");
        si->setForeground(t.returned ? QColor("#10B981") : overdue ? QColor("#EF4444") : QColor("#F59E0B"));
        txnsTable->setItem(row,6, si);
    }
}

void LibrarianDashboard::loadFines() {
    auto& db = Database::instance();
    finesTable->setRowCount(0);
    for (auto& f : db.fines) {
        int row = finesTable->rowCount();
        finesTable->insertRow(row);
        finesTable->setItem(row,0, new QTableWidgetItem(QString::number(f.user_id)));
        finesTable->setItem(row,1, new QTableWidgetItem(QString::number(f.book_id)));
        finesTable->setItem(row,2, new QTableWidgetItem(QString::number(f.minuteslate)));
        finesTable->setItem(row,3, new QTableWidgetItem(QString("Rs.%1").arg(f.rate)));
        finesTable->setItem(row,4, new QTableWidgetItem(QString("Rs.%1").arg(f.originalAmount,0,'f',2)));
        finesTable->setItem(row,5, new QTableWidgetItem(f.discountApplied ? "15%" : "–"));
        finesTable->setItem(row,6, new QTableWidgetItem(QString("Rs.%1").arg(f.finalAmount,0,'f',2)));
    }
}

void LibrarianDashboard::loadTables() {
    auto& db = Database::instance();
    tablesTable->setRowCount(0);
    for (auto& t : db.tables) {
        int row = tablesTable->rowCount();
        tablesTable->insertRow(row);
        tablesTable->setItem(row,0, new QTableWidgetItem(QString::number(t.id)));
        tablesTable->setItem(row,1, new QTableWidgetItem(t.location));
        tablesTable->setItem(row,2, new QTableWidgetItem(QString::number(t.capacity)));
    }
}

void LibrarianDashboard::loadReservations() {
    auto& db = Database::instance();
    reservationsTable->setRowCount(0);
    for (auto& r : db.reservations) {
        int row = reservationsTable->rowCount();
        reservationsTable->insertRow(row);
        reservationsTable->setItem(row,0, new QTableWidgetItem(QString::number(r.reservationid)));
        reservationsTable->setItem(row,1, new QTableWidgetItem(QString::number(r.userid)));
        reservationsTable->setItem(row,2, new QTableWidgetItem(r.bookid ? "Book" : "Table"));
        reservationsTable->setItem(row,3, new QTableWidgetItem(
            QString::number(r.bookid ? r.bookid : r.tableid)));
        reservationsTable->setItem(row,4, new QTableWidgetItem(r.date));
        reservationsTable->setItem(row,5, new QTableWidgetItem(r.startTime));
        reservationsTable->setItem(row,6, new QTableWidgetItem(r.endTime));
        auto* ai = new QTableWidgetItem(r.active ? "✅ Active" : "❌ Inactive");
        ai->setForeground(r.active ? QColor("#10B981") : QColor("#64748B"));
        reservationsTable->setItem(row,7, ai);
    }
}

// ===========================================================
//  BOOK MANAGEMENT
// ===========================================================
void LibrarianDashboard::onAddBook() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add New Book");
    dlg.setStyleSheet("background-color:#1E2D3D; color:#E2E8F0;");
    auto* fl = new QFormLayout(&dlg);
    fl->setContentsMargins(24,20,24,20); fl->setSpacing(10);

    auto* titleE  = new QLineEdit;
    auto* authorE = new QLineEdit;
    auto* catE    = new QLineEdit;
    auto* isbnE   = new QLineEdit;
    auto* pubE    = new QLineEdit;
    auto* yearS   = new QSpinBox;  yearS->setRange(1000, 2026);
    auto* copS    = new QSpinBox;  copS->setRange(1, 1000);

    fl->addRow("Title:",     titleE);
    fl->addRow("Author:",    authorE);
    fl->addRow("Category:",  catE);
    fl->addRow("ISBN:",      isbnE);
    fl->addRow("Publisher:", pubE);
    fl->addRow("Year:",      yearS);
    fl->addRow("Copies:",    copS);

    auto* okBtn = new QPushButton("Add Book"); okBtn->setObjectName("successBtn");
    fl->addRow(okBtn);
    connect(okBtn, &QPushButton::clicked, &dlg, &QDialog::accept);
    if (dlg.exec() != QDialog::Accepted) return;

    QString title = titleE->text().trimmed();
    if (title.isEmpty()) { QMessageBox::warning(this,"Error","Title cannot be empty."); return; }

    auto& db = Database::instance();
    // duplicate check
    if (db.findBookByTitle(title.toStdString())) {
        QMessageBox::warning(this,"Duplicate","A book with this title already exists.");
        return;
    }

    BookRecord br{};
    br.id = db.nextBookID++;
    strncpy(br.title,     title.toStdString().c_str(), 99);
    strncpy(br.author,    authorE->text().trimmed().toStdString().c_str(), 99);
    strncpy(br.category,  catE->text().trimmed().toStdString().c_str(),   49);
    strncpy(br.isbn,      isbnE->text().trimmed().toStdString().c_str(),  29);
    strncpy(br.publisher, pubE->text().trimmed().toStdString().c_str(),   49);
    br.year   = yearS->value();
    br.copies = copS->value();

    db.books.push_back(br);
    db.appendBook(br);
    QMessageBox::information(this,"Added",
        QString("✅  Book '%1' added (ID: %2).").arg(br.title).arg(br.id));
    loadBooks(); loadOverview();
}

void LibrarianDashboard::onRemoveBook() {
    int row = booksTable->currentRow();
    if (row < 0) { QMessageBox::information(this,"Select","Select a book to remove."); return; }
    int bid = booksTable->item(row,0)->text().toInt();
    QString title = booksTable->item(row,1)->text();

    auto res = QMessageBox::question(this,"Remove Book",
        QString("Remove '%1' (ID %2)?").arg(title).arg(bid),
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;

    auto& db = Database::instance();
    db.books.erase(std::remove_if(db.books.begin(), db.books.end(),
        [bid](const BookRecord& b){ return b.id == bid; }), db.books.end());
    db.rewriteBooks();
    QMessageBox::information(this,"Removed","✅  Book removed.");
    loadBooks(); loadOverview();
}

// ===========================================================
//  TABLE MANAGEMENT
// ===========================================================
void LibrarianDashboard::onAddTable() {
    auto& db = Database::instance();
    if ((int)db.tables.size() >= 10) {
        QMessageBox::warning(this,"Limit","Maximum 10 tables allowed."); return;
    }
    QDialog dlg(this);
    dlg.setWindowTitle("Add Table");
    dlg.setStyleSheet("background-color:#1E2D3D;");
    auto* fl = new QFormLayout(&dlg);
    fl->setContentsMargins(20,18,20,18); fl->setSpacing(10);
    auto* locEdit = new QLineEdit; locEdit->setPlaceholderText("e.g. Reading Room A");
    auto* capSpin = new QSpinBox;  capSpin->setRange(1,50);
    fl->addRow("Location:", locEdit);
    fl->addRow("Capacity:", capSpin);
    auto* ok = new QPushButton("Add"); ok->setObjectName("successBtn");
    fl->addRow(ok);
    connect(ok, &QPushButton::clicked, &dlg, &QDialog::accept);
    if (dlg.exec() != QDialog::Accepted) return;

    TableRecord tr{};
    tr.id = db.nextTableID++;
    strncpy(tr.location, locEdit->text().trimmed().toStdString().c_str(), 99);
    tr.capacity = capSpin->value();
    db.tables.push_back(tr);
    db.appendTable(tr);
    QMessageBox::information(this,"Added",QString("✅  Table ID %1 added.").arg(tr.id));
    loadTables();
}

void LibrarianDashboard::onRemoveTable() {
    int row = tablesTable->currentRow();
    if (row < 0) { QMessageBox::information(this,"Select","Select a table to remove."); return; }
    int tid = tablesTable->item(row,0)->text().toInt();
    auto res = QMessageBox::question(this,"Remove Table",
        QString("Remove table ID %1?").arg(tid),
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;

    auto& db = Database::instance();
    db.tables.erase(std::remove_if(db.tables.begin(), db.tables.end(),
        [tid](const TableRecord& t){ return t.id == tid; }), db.tables.end());
    db.rewriteTables();
    QMessageBox::information(this,"Removed","✅  Table removed.");
    loadTables();
}

// ===========================================================
//  USER MANAGEMENT
// ===========================================================
void LibrarianDashboard::onDeleteUser() {
    int row = usersTable->currentRow();
    if (row < 0) { QMessageBox::information(this,"Select","Select a user to delete."); return; }
    int uid  = usersTable->item(row,0)->text().toInt();
    auto& db = Database::instance();
    auto* ur = db.findUser(uid);
    if (!ur) return;
    if (ur->isDeleted) { QMessageBox::information(this,"Already Deleted","User already deleted."); return; }
    if (ur->borrowed_count > 0) {
        QMessageBox::warning(this,"Cannot Delete","User has unreturned books."); return;
    }
    auto res = QMessageBox::question(this,"Delete User",
        QString("Soft-delete user '%1' (ID %2)?\nData is preserved.").arg(ur->name).arg(uid),
        QMessageBox::Yes | QMessageBox::No);
    if (res != QMessageBox::Yes) return;

    ur->isDeleted = true;
    // cancel reservations
    for (auto& r : db.reservations)
        if (r.userid == uid && r.active) r.active = false;
    db.rewriteUsers();
    db.rewriteReservations();
    QMessageBox::information(this,"Deleted",
        QString("✅  User '%1' marked as deleted.").arg(ur->name));
    refreshAll();
}

void LibrarianDashboard::onViewUserDetails() {
    int row = usersTable->currentRow();
    if (row < 0) { QMessageBox::information(this,"Select","Select a user."); return; }
    int uid  = usersTable->item(row,0)->text().toInt();
    auto& db = Database::instance();
    auto* ur = db.findUser(uid);
    if (!ur) return;

    // Count txns & fines
    int txnCnt = 0; double totalFine = 0;
    for (auto& t : db.transactions) if (t.uid == uid) txnCnt++;
    for (auto& f : db.fines) if (f.user_id == uid) totalFine += f.finalAmount;

    QString info =
        QString("ID:          %1\n"
                "Name:        %2\n"
                "Email:       %3\n"
                "Phone:       %4\n"
                "CNIC:        %5\n"
                "Type:        %6\n"
                "Username:    %7\n"
                "Balance:     Rs.%8\n"
                "Fines Paid:  Rs.%9\n"
                "Membership:  %10\n"
                "Transactions:%11\n"
                "Status:      %12")
        .arg(ur->id).arg(ur->name).arg(ur->email).arg(ur->phone)
        .arg(ur->cnic).arg(ur->type).arg(ur->username)
        .arg(ur->balance,0,'f',2).arg(ur->total_fines_paid,0,'f',2)
        .arg(ur->hasMembership && ur->membershipExpiry > time(nullptr) ? "Active" : "None")
        .arg(txnCnt)
        .arg(ur->isDeleted ? "DELETED" : "ACTIVE");

    QMessageBox::information(this, QString("User Details – %1").arg(ur->name), info);
}
