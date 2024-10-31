#include "mainwindow.h"

#include <QApplication>

const QString configPath = CONFIG_FILE_PATH;

int main(int argc, char *argv[]) {
    Logger::initFromConfig(configPath);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
