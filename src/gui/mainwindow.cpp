#include "gui/mainwindow.h"

#include <QChartView>
#include <QCoreApplication>
#include <QLabel>
#include <QLineSeries>
#include <QTimer>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include "app/helpers.h"

namespace PRISM {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      _centralWidget(new QWidget(this)),
      _hBoxLayout(std::make_shared<QHBoxLayout>()) {
    setCentralWidget(_centralWidget);
    auto pollInterval_ms_int = pollInterval_ms.count();
    _pollTimer = std::make_shared<QTimer>(this);
    _pollTimer->start(pollInterval_ms_int);
    connect(_pollTimer.get(), &QTimer::timeout, this, &MainWindow::pollingCallback);
}

MainWindow::~MainWindow() {}

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
    // also update current sensor in gui
    onListItemClicked();
}

void MainWindow::onListItemClicked() {
    QListWidgetItem* currentItem = _listWidget->currentItem();
    if (currentItem) {
        ObjectId id = currentItem->data(Qt::UserRole).value<ObjectId>();

        // cleanCurrentChart();

        updateChartView(id);
    }
}

void MainWindow::updateChartView(ObjectId id) {
    QChart* newChart = new QChart();
    auto sensor = _sensors->at(id);
    newChart->setTitle(QString::fromStdString(sensor.getName()));

    QLineSeries* newDataSeries = new QLineSeries();

    double x, y;
    for (int i = 0; i < sensor.size(); i++) {
        x = sensor.getXByIndex(i);
        y = sensor.getYByIndex(i);
        newDataSeries->append(x, y);
    }

    newChart->addSeries(newDataSeries);
    newChart->createDefaultAxes();

    // if (sensor.size() > 0) {
    //     float x0 = sensor.getXByIndex(0);
    //     float x1 = sensor.getXByIndex(sensor.size() - 1);
    //     newChart->axisX()->setRange(x0 - chartRangePadding, x1 + chartRangePadding);
    // }

    _chartView->setChart(newChart);
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

    initGui();
}

void MainWindow::initGui() {
    if (_sensors->size() == 0) {
        _hBoxLayout->addWidget(new QLabel("No sensors defined as active in config!"));
        _hBoxLayout->setAlignment(Qt::AlignCenter);
    } else {
        _listWidget = std::make_shared<QListWidget>(this);
        _listWidget->setMaximumWidth(200);
        connect(_listWidget.get(), &QListWidget::itemSelectionChanged, this,
                &MainWindow::onListItemClicked);

        auto sortedSensors = std::map<ObjectId, Sensor>(_sensors->begin(), _sensors->end());
        for (auto& [id, sensor] : sortedSensors) {
            std::string name = id.toString() + " " + sensor.getName();
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
            item->setData(Qt::UserRole, QVariant::fromValue(id));
            _listWidget->addItem(item);
        }
        _hBoxLayout->addWidget(_listWidget.get());
        _chartView = new QChartView();
        _hBoxLayout->addWidget(_chartView);
    }
    _centralWidget->setLayout(_hBoxLayout.get());
}

}  // namespace PRISM