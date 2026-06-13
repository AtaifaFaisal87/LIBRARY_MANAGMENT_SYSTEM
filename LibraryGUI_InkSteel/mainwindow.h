#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onLogin();
    void onRegister();

private:
    void buildLandingPage();
    QStackedWidget* stack;
};
