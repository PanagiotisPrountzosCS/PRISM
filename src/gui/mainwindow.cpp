#include "gui/mainwindow.h"

#include <QChartView>
#include <QHBoxLayout>
#include <QLineSeries>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      listWidget(new QListWidget(this)),
      stackedWidget(new QStackedWidget(this)),
      centralWidget(new QWidget(this)) {
    // Set up the central widget and layout
    setCentralWidget(centralWidget);
    QHBoxLayout *layout = new QHBoxLayout;  // Use horizontal layout

    // Set up the list widget with options
    for (int i = 0; i < 200; ++i) {
        listWidget->addItem(QString("Chart %1").arg(i + 1));  // Add 200 items
    }

    // Set size policy to allow the list widget to expand vertically
    listWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);  // Enable scrollbar if needed

    // Set maximum width for the list widget
    listWidget->setMaximumWidth(200);  // Adjust this value as needed

    // Connect the list widget signal to the slot
    connect(listWidget, &QListWidget::itemClicked, this, &MainWindow::onListItemClicked);

    // Add the list widget and stacked widget to the layout
    layout->addWidget(listWidget);     // Add list widget to the left
    layout->addWidget(stackedWidget);  // Add stacked widget to the right

    centralWidget->setLayout(layout);  // Set the layout for the central widget

    // Set the initial widget (first option)
    stackedWidget->setCurrentIndex(0);  // Initially shows a placeholder
}

MainWindow::~MainWindow() {
    clearCurrentChart();  // Clean up when closing the window
}

// Slot that gets triggered when a list item is clicked
void MainWindow::onListItemClicked(QListWidgetItem *item) {
    int index = listWidget->row(item);  // Get the index of the clicked item
    clearCurrentChart();                // Clear the current chart
    loadChart(index);                   // Load the new chart lazily
}

// Function to load the chart lazily based on the selected index
void MainWindow::loadChart(int index) {
    // Create the chart widget when needed
    QChart *chart = new QChart();
    chart->setTitle(QString("Chart %1").arg(index + 1));

    // For demonstration, let's create a simple chart with random data
    QLineSeries *series = new QLineSeries();
    for (int i = 0; i < 5000; ++i) {
        series->append(i, qSin(i * 1.0 / 1000));  // Generating random points
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    QAbstractAxis *yAxis = chart->axisY(series);
    yAxis->setRange(-2, 2);

    // Add the chart to the stacked widget
    QChartView *chartView = new QChartView(chart);
    stackedWidget->addWidget(chartView);

    // Set the current index to show the newly loaded chart
    stackedWidget->setCurrentIndex(index);
    currentChart = chart;  // Track the current chart
}

// Function to clear the current chart to free memory
void MainWindow::clearCurrentChart() {
    if (currentChart) {
        // Remove and delete the current chart (memory cleanup)
        QList<QWidget *> widgets = stackedWidget->findChildren<QWidget *>();
        for (QWidget *widget : widgets) {
            stackedWidget->removeWidget(widget);
            // if (widget)
            // delete widget; // This ensures memory is freed
        }
        currentChart = nullptr;  // Reset the current chart pointer
    }
}
