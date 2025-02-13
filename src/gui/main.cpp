#include <QApplication>
#include <iostream>

#include "app/helpers.h"
#include "gui/mainwindow.h"

double PRISM::appStartTime_ms;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_path>" << std::endl;
        return 1;
    }

    PRISM::appStartTime_ms =
        static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::high_resolution_clock::now().time_since_epoch())
                                .count());

    QApplication app(argc, argv);
    PRISM::MainWindow window;
    window.initApp(argv[1]);
    window.setWindowTitle("PRISM");
    window.resize(1080, 640);
    window.show();

    app.exec();
    window.cleanup();

    return 0;
}
