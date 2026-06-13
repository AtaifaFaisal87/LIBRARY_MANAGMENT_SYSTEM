#include <QApplication>
#include "mainwindow.h"
#include "library_core.h"
#include "styles.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    app.setStyleSheet(APP_STYLE);

    // Load all data from binary files on startup
    Database::instance().loadAll();

    MainWindow w;
    w.show();

    return app.exec();
}
