#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QComboBox>
#include "library_core.h"

class UserDashboard : public QMainWindow {
    Q_OBJECT
public:
    explicit UserDashboard(const UserRecord& user, QWidget* parent = nullptr);

private slots:
    void refreshAll();
    void onBorrow();
    void onReturn();
    void onReadOnSite();
    void onDownload();
    void onReserveTable();
    void onReserveBook();
    void onCancelReservation();
    void onBuyMembership();
    void onCancelMembership();
    void onAddBalance();
    void onUpdateProfile();

private:
    UserRecord currentUser;

    // tabs
    QTabWidget* tabs;

    // Dashboard tab
    QLabel* balanceLbl;
    QLabel* membershipLbl;
    QLabel* borrowedLbl;
    QLabel* finesLbl;
    QTableWidget* borrowedTable;

    // Books tab
    QTableWidget* bookTable;

    // Transactions tab
    QTableWidget* txnTable;

    // Fines tab
    QTableWidget* finesTable;

    // Reservations tab
    QTableWidget* resTable;

    void buildDashboardTab(QTabWidget* t);
    void buildBooksTab(QTabWidget* t);
    void buildTransactionsTab(QTabWidget* t);
    void buildFinesTab(QTabWidget* t);
    void buildReservationsTab(QTabWidget* t);
    void buildMembershipTab(QTabWidget* t);

    void loadBooks();
    void loadTransactions();
    void loadFines();
    void loadReservations();
    void loadDashboard();

    int  getFineRate();
    int  getEffectiveBorrowLimit();
    bool hasOverdueBooks();
    bool hasActiveMembership();
    double calcLiveFine();

    // membership panel widgets
    QLabel*      memStatusLbl;
    QLabel*      memExpiryLbl;
    QPushButton* buyMemBtn;
    QPushButton* cancelMemBtn;
};
