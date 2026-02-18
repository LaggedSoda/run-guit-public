#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "ServiceManager.h"

namespace Ui { class MainWindow; }  // forward declare only

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ServiceManager manager;
    void showContextMenu(const QPoint &pos);
    void restartService(const QString &serviceName);
    void startService(const QString &serviceName);
    void stopService(const QString &serviceName);
    void populateServiceTable();
    void updateServiceStatus(QTableWidgetItem* statusItem, ServiceStatus status);
    bool waitForServiceStatus(const QString &serviceName, ServiceStatus expected, int attempts = 10, int delayMs = 100);

private slots:
    void filterServices(const QString &text);
};

#endif // MAINWINDOW_H
