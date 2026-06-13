#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLabel>
#include "library_core.h"

class LibrarianDashboard : public QMainWindow {
    Q_OBJECT
public:
    explicit LibrarianDashboard(QWidget* parent = nullptr);

private slots:
    void refreshAll();
    // Book management
    void onAddBook();
    void onRemoveBook();
    // Table management
    void onAddTable();
    void onRemoveTable();
    // User management
    void onDeleteUser();
    void onViewUserDetails();

private:
    QTabWidget*   tabs;

    // Overview
    QLabel* totalUsersLbl;
    QLabel* totalBooksLbl;
    QLabel* totalTxnsLbl;
    QLabel* totalFinesLbl;

    // Tables
    QTableWidget* usersTable;
    QTableWidget* booksTable;
    QTableWidget* txnsTable;
    QTableWidget* finesTable;
    QTableWidget* tablesTable;
    QTableWidget* reservationsTable;

    void buildOverviewTab(QTabWidget* t);
    void buildUsersTab(QTabWidget* t);
    void buildBooksTab(QTabWidget* t);
    void buildTransactionsTab(QTabWidget* t);
    void buildFinesTab(QTabWidget* t);
    void buildTablesTab(QTabWidget* t);
    void buildReservationsTab(QTabWidget* t);

    void loadUsers();
    void loadBooks();
    void loadTransactions();
    void loadFines();
    void loadTables();
    void loadReservations();
    void loadOverview();
};
