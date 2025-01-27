#pragma once

#include <QChart>
#include <QListWidget>
#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>

#include "core/objectid.h"
#include "core/sensor.h"

namespace PRISM {

using SensorMap = std::unordered_map<ObjectId, Sensor, ObjectIdHash>;

void sigintHandler(int signum);

class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    explicit MainWindow(QWidget* parent = nullptr);
    void cleanup();
    void initApp(const char* configPath);
    ~MainWindow();

public slots:
    void pollingCallback();

private:
    QTimer* _pollTimer;  // timer counting up to the pollInterval
    std::shared_ptr<SensorMap> _sensors;
};

}  // namespace PRISM
