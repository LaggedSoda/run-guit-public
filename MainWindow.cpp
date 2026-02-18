#include "MainWindow.h"
#include "ServiceManager.h"
#include "ui_MainWindow.h"

#include <QTableWidgetItem>
#include <QStringList>
#include <QVector>
#include <QMenu>
#include <QProcess>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)      // allocate & construct the UI
{
    ui->setupUi(this);            // set up widgets from your .ui file
    ui->serviceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->serviceTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    populateServiceTable();

    connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::filterServices);
}

MainWindow::~MainWindow()
{
    delete ui;                     // clean up
}

void MainWindow::populateServiceTable()
{
    QVector<ServiceInfo> services = manager.getAllServices();
    ui->serviceTable->setRowCount(services.size());
    ui->serviceTable->setColumnCount(2);

    QStringList headers { "Service", "Status" };
    ui->serviceTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < services.size(); ++i) {
        const ServiceInfo& info = services[i];

        auto *nameItem   = new QTableWidgetItem(info.name);
        auto *statusItem = new QTableWidgetItem;

        ui->serviceTable->setItem(i, 0, nameItem);
        ui->serviceTable->setItem(i, 1, statusItem);

        updateServiceStatus(statusItem, info.status); //Update status item


    }
    ui->serviceTable->horizontalHeader()->setStretchLastSection(true);
}


void MainWindow::updateServiceStatus(QTableWidgetItem* statusItem, ServiceStatus status)
{
    switch (status) {
        case ServiceStatus::Running:
            statusItem->setText("Running");
            statusItem->setBackground(Qt::green);
            break;
        case ServiceStatus::Down:
            statusItem->setText("Stopped");
            statusItem->setBackground(Qt::gray);
            break;
        case ServiceStatus::Error:
            statusItem->setText("Error");
            statusItem->setBackground(Qt::red);
            break;
        case ServiceStatus::Restarting:
            statusItem->setText("Restarting...");
        case ServiceStatus::Starting:
            statusItem->setText("Starting...");
        case ServiceStatus::Stopping:
            statusItem->setText("Stopping...");
        default:
            statusItem->setText("Unknown");
            statusItem->setBackground(Qt::yellow);
            break;
    }
}


void MainWindow::filterServices(const QString &text)
{
    for (int row = 0; row < ui->serviceTable->rowCount(); ++row)
    {
        bool matches = false;
        for (int col = 0; col < ui->serviceTable->columnCount(); ++col)
        {
            QTableWidgetItem *item = ui->serviceTable->item(row, col);
            if (item && item->text().contains(text, Qt::CaseInsensitive))
            {
                matches = true;
                break;
            }
        }

        ui->serviceTable->setRowHidden(row, !matches);  // Hide the row if it doesn't match
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QTableWidgetItem *item = ui->serviceTable->itemAt(pos);
    if (!item) return;

    int row = item->row();
    QString serviceName = ui->serviceTable->item(row, 0)->text();

    QMenu contextMenu;
    QAction *restartAction = contextMenu.addAction("Restart Service");
    QAction *startAction = contextMenu.addAction("Start Service");
    QAction *stopAction = contextMenu.addAction("Stop Service");


    QAction *selectedAction = contextMenu.exec(ui->serviceTable->viewport()->mapToGlobal(pos));
    if (selectedAction == restartAction) {
        restartService(serviceName);}

    if (selectedAction == startAction) {
        startService(serviceName);}

    if (selectedAction == stopAction) {
        stopService(serviceName);
    }
}


void MainWindow::restartService(const QString &serviceName) {
    QProcess::startDetached("sv", {"restart", serviceName});
    // Find the QTableWidgetItem for this service
    int row = -1;
    for (int i = 0; i < ui->serviceTable->rowCount(); ++i) {
        if (ui->serviceTable->item(i,0)->text() == serviceName) {
            row = i;
            break;
        }
    }
    if (row == -1) return;

    QTableWidgetItem* statusItem = ui->serviceTable->item(row, 1);
    updateServiceStatus(statusItem, ServiceStatus::Restarting); //need more logic for detecting status from the restart
    bool success = waitForServiceStatus(serviceName, ServiceStatus::Running, 20, 100);

    if (row != -1)
        updateServiceStatus(ui->serviceTable->item(row, 1), success ? ServiceStatus::Running : ServiceStatus::Error);
}

void MainWindow::startService(const QString &serviceName) {
    QProcess::startDetached("sv", {"start", serviceName});
    int row = -1;
    for (int i = 0; i < ui->serviceTable->rowCount(); ++i) {
        if (ui->serviceTable->item(i,0)->text() == serviceName) {
            row = i;
            break;
        }
    }
    if (row == -1) return;

    QTableWidgetItem* statusItem = ui->serviceTable->item(row, 1);
    updateServiceStatus(statusItem, ServiceStatus::Starting); //need more logic for detecting status from the restart
    bool success = waitForServiceStatus(serviceName, ServiceStatus::Running, 20, 100);

    if (row != -1)
        updateServiceStatus(ui->serviceTable->item(row, 1), success ? ServiceStatus::Running : ServiceStatus::Error);
}

void MainWindow::stopService(const QString &serviceName) {
    QProcess::startDetached("sv", {"stop", serviceName});
    int row = -1;
    for (int i = 0; i < ui->serviceTable->rowCount(); ++i) {
        if (ui->serviceTable->item(i,0)->text() == serviceName) {
            row = i;
            break;
        }
    }
    if (row == -1) return;

    QTableWidgetItem* statusItem = ui->serviceTable->item(row, 1);
    updateServiceStatus(statusItem, ServiceStatus::Stopping); //need more logic for detecting status from the restart
    bool success = waitForServiceStatus(serviceName, ServiceStatus::Down, 20, 100);

    if (row != -1)
        updateServiceStatus(ui->serviceTable->item(row, 1), success ? ServiceStatus::Down : ServiceStatus::Error);
}



bool MainWindow::waitForServiceStatus(const QString &serviceName, ServiceStatus expected, int attempts, int delayMs)
{
    for (int i = 0; i < attempts; ++i)
    {
        ServiceStatus current = manager.getServiceStatus(serviceName);
        if (current == expected) return true;

        QThread::msleep(delayMs);  // small delay
        QCoreApplication::processEvents(); // keep UI responsive
    }
    return false; // service didn’t reach expected status
}
