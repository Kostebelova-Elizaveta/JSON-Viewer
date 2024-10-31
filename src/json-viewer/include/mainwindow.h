#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include "logger.h"
#include "jsonhighlighter.h"
#include "jsonmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openButton_clicked();

    void on_updateButton_clicked();

    void on_showButton_clicked();

    void on_jsonTreeView_collapsed(const QModelIndex &index);

    void on_jsonTreeView_expanded(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    JsonHighlighter *highlighter;
    JsonModel *model;

    void expandAll(const QModelIndex &index);
    void collapseAll(const QModelIndex &index);
    bool isTreeExpanded(const QModelIndex &index);
    bool isTreeCollapsed(const QModelIndex &index);
};
#endif // MAINWINDOW_H
