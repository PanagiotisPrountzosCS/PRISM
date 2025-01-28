#include "gui/mainwindow.h"

#include <QCoreApplication>
#include <QTimer>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "app/helpers.h"

namespace PRISM {

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    auto pollInterval_ms_int = pollInterval_ms.count();
    _pollTimer = new QTimer(this);
    _pollTimer->start(pollInterval_ms_int);
    connect(_pollTimer, &QTimer::timeout, this, &MainWindow::pollingCallback);
}

MainWindow::~MainWindow() { delete _pollTimer; }

void sigintHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nCaught SIGINT (Ctrl + C). Exiting gracefully...\n";
        QCoreApplication::quit();
    }
}

void MainWindow::pollingCallback() {
    for (auto& [id, sensor] : *_sensors) {
        sensor.pollAndUpdate();
        if (sensor.size() >= maxMeasurements) {
            sensor.saveMeasurements();
            sensor.clear();
            sensor.freeHeap();
        }
    }
    //also update current sensor in gui
}

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