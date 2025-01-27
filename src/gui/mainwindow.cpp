#include "gui/mainwindow.h"

#include <QCoreApplication>
#include <QTimer>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "app/helpers.h"

namespace PRISM {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    _pollTimer = new QTimer(this);
    _pollTimer->start(500);
    connect(_pollTimer, &QTimer::timeout, this, &MainWindow::pollingCallback);
}

MainWindow::~MainWindow() { delete _pollTimer; }

void sigintHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nCaught SIGINT (Ctrl + C). Exiting gracefully...\n";
        QCoreApplication::quit();
    }
}

void MainWindow::pollingCallback() { std::cout << "Callback\n"; }

void MainWindow::cleanup() {
    std::cout << "Cleaning up resources...\n";

    for (auto& [id, sensor] : *_sensors) {
        sensor.saveMeasurements();
        sensor.clear();
        sensor.freeHeap();
    }
}

void MainWindow::initApp(const char* configPath) {
    std::signal(SIGINT, sigintHandler);

    PRISM::JSONParser::Value config = parseConfig(std::string(configPath));

    validateConfig(config);

    _sensors = createSensors(config);
}

}  // namespace PRISM