#pragma once

#include <QChart>
#include <QChartView>
#include <QHBoxLayout>
#include <QListWidget>
#include <QMainWindow>
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
    void initGui();
    ~MainWindow();

private slots:
    void pollingCallback();
    void onListItemClicked();

private:
    void updateChartView(ObjectId id);

private:
    std::shared_ptr<QTimer> _pollTimer;  // timer counting up to the pollInterval
    std::shared_ptr<SensorMap> _sensors;
    std::shared_ptr<QListWidget> _listWidget;
    std::shared_ptr<QHBoxLayout> _hBoxLayout;
    QWidget* _centralWidget;
    QChartView* _chartView;
};

}  // namespace PRISM
