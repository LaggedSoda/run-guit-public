#include "MainWindow.h"
#include "ServiceManager.h"
#include "ui_MainWindow.h"

#include <QTableWidgetItem>
#include <QStringList>
#include <QVector>
#include <QBrush>
#include <QColor>
#include <QMenu>
#include <QProcess>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)      // ← allocate & construct the UI
{
    ui->setupUi(this);            // ← set up widgets from your .ui file
    ui->serviceTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->serviceTable, &QTableWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);
    populateServiceTable();

    connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::filterServices);
}

MainWindow::~MainWindow()
{
    delete ui;                     // ← clean up
}

void MainWindow::populateServiceTable()
{
    QVector<ServiceInfo> services = manager.getAllServices();
    ui->serviceTable->setRowCount(services.size());
    ui->serviceTable->setColumnCount(2);

    QStringList headers;
    headers << "Service" << "Status";
    ui->serviceTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < services.size(); ++i) {
        const ServiceInfo& info = services[i];

        auto *nameItem   = new QTableWidgetItem(info.name);
        auto *statusItem = new QTableWidgetItem;

        switch (info.status) {
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
            default:
                statusItem->setText("Unknown");
                statusItem->setBackground(Qt::yellow);
                break;
        }

        // Adjust text color (bold for non-running/error states)
        if(info.status != ServiceStatus::Running)
        {
            statusItem->setForeground(Qt::black);    // Set text color (for visibility)
            statusItem->setFont(QFont("Arial", 10, QFont::Bold));   // Make text bold for non-running/error statuses
        }

        ui->serviceTable->setItem(i, 0, nameItem);
        ui->serviceTable->setItem(i, 1, statusItem);
    }
    ui->serviceTable->horizontalHeader()->setStretchLastSection(true);
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
    QStringList arguments;
    arguments << "restart" << serviceName;
    QProcess::startDetached("sv", arguments);
}

void MainWindow::startService(const QString &serviceName) {
    QStringList arguments;
    arguments << "start" << serviceName;
    QProcess::startDetached("sv", arguments);
}

void MainWindow::stopService(const QString &serviceName) {
    QStringList arguments;
    arguments << "stop" << serviceName;
    QProcess::startDetached("sv", arguments);
}
