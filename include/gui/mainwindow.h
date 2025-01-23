#pragma once

#include <QChart>
#include <QListWidget>
#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onListItemClicked(QListWidgetItem *item);  // Slot for switching charts

private:
    QListWidget *listWidget;         // List widget for choosing charts
    QStackedWidget *stackedWidget;   // Stacked widget to hold charts
    QWidget *centralWidget;          // Central widget container
    QChart *currentChart = nullptr;  // Pointer to the currently loaded chart
    void loadChart(int index);       // Function to load a chart lazily
    void clearCurrentChart();        // Function to clear the current chart
};
