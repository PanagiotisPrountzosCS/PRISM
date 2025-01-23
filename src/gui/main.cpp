#include <QApplication>

#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("PRISM GUI");
    window.show();

    return app.exec();
}
