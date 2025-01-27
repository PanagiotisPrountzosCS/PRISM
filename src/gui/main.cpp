#include <QApplication>
#include <iostream>

#include "gui/mainwindow.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_path>" << std::endl;
        return 1;
    }
    QApplication app(argc, argv);

    PRISM::MainWindow window;
    window.initApp(argv[1]);
    window.setWindowTitle("PRISM");
    window.show();

    app.exec();

    window.cleanup();

    return 0;
}
