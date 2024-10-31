#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->openButton->setIcon(QIcon(IMAGE_OPEN_FILE_PATH));
    ui->openButton->setIconSize(QSize(16, 16));

    ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
    ui->showButton->setIconSize(QSize(16, 16));
    ui->showButton->setText("Expand all");

    ui->updateButton->setIcon(QIcon(IMAGE_UPDATE_FILE_PATH));
    ui->updateButton->setIconSize(QSize(16, 16));

    model = new JsonModel;
    highlighter = new JsonHighlighter(ui->jsonTextEdit->document());
}

MainWindow::~MainWindow() {
    delete highlighter;
    delete model;
    delete ui;
    Logger::log(DEBUG, "MainWindow is closed");
}


void MainWindow::on_openButton_clicked() {
    Logger::log(DEBUG, "Open button clicked");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose JSON-file"),
                                                    "", tr("JSON (*.json)"));
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Error"), tr("Cannot open file: ") + fileName);
            Logger::log(ERROR, "Cannot open file: " + fileName);
            return;
        }
        QTextStream in(&file);
        QString jsonString = in.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
        if (jsonDoc.isNull()) {
            QMessageBox::warning(this, tr("Error"), tr("Invalid JSON format"));
            Logger::log(ERROR, "Invalid JSON format");
            return;
        }
        ui->jsonTextEdit->setPlainText(jsonString);
        Logger::log(INFO, "Text from file is loaded");
        delete model;
        model = new JsonModel;
        if (jsonDoc.isArray()) {
            model->createTree(jsonDoc.array(), jsonDoc.array(), model->rootIndex());
        }
        else {
            if (jsonDoc.isObject()) {
                model->createTree(jsonDoc.object(), jsonDoc.object(), model->rootIndex());
            }
        }
        ui->jsonTreeView->setModel(model);
        Logger::log(INFO, "Tree is built");
        ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
        ui->showButton->setText("Expand all");
    }
    else {
        Logger::log(ERROR, "File is empty!");
    }
}


void MainWindow::on_updateButton_clicked() {
    Logger::log(DEBUG, "Update button clicked");
    QString jsonString = ui->jsonTextEdit->toPlainText();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid JSON format"));
        Logger::log(ERROR, "Invalid JSON format");
        return;
    }
    delete model;
    model = new JsonModel;
    if (jsonDoc.isArray()) {
        model->createTree(jsonDoc.array(), jsonDoc.array());
    }
    else {
        if (jsonDoc.isObject()) {
            model->createTree(jsonDoc.object(), jsonDoc.object());
        }
    }
    ui->jsonTreeView->setModel(model);
    Logger::log(INFO, "Tree is built");
    ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
    ui->showButton->setText("Expand all");
}

bool MainWindow::isTreeExpanded(const QModelIndex &index) {
    if (!ui->jsonTreeView->model()->hasChildren(index)) {
        return true;
    }
    else {
        if (!ui->jsonTreeView->isExpanded(index)) {
            return false;
        }
    }
    int count = ui->jsonTreeView->model()->rowCount(index);
    for (int i = 0; i < count; ++i) {
        QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
        if (!isTreeExpanded(childIndex)) {
            return false;
        }
    }
    return true;
}


bool MainWindow::isTreeCollapsed(const QModelIndex &index) {
    if (!ui->jsonTreeView->isExpanded(index)) {
        return true;
    }
    int count = ui->jsonTreeView->model()->rowCount(index);
    for (int i = 0; i < count; ++i) {
        QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
        if (isTreeCollapsed(childIndex)) {
            return true;
        }
    }
    return false;
}


void MainWindow::expandAll(const QModelIndex &index) {
    ui->jsonTreeView->expand(index);
    int count = ui->jsonTreeView->model()->rowCount(index);
    for (int i = 0; i < count; ++i) {
        QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
        expandAll(childIndex);
    }
}

void MainWindow::collapseAll(const QModelIndex &index) {
    ui->jsonTreeView->collapse(index);
    int count = ui->jsonTreeView->model()->rowCount(index);
    for (int i = 0; i < count; ++i) {
        QModelIndex childIndex = ui->jsonTreeView->model()->index(i, 0, index);
        collapseAll(childIndex);
    }
}


void MainWindow::on_showButton_clicked() {
    Logger::log(DEBUG, "Expand/Collapse button clicked");
    if (model && model->rowCount() != 0) {
        if (ui->showButton->text() == "Expand all") {
            for (int i = 0; i <= ui->jsonTreeView->model()->columnCount(); i++) {
                expandAll(ui->jsonTreeView->model()->index(0, i));
            }
            Logger::log(INFO, "Tree is expanded");
            ui->showButton->setIcon(QIcon(IMAGE_COLLAPSE_FILE_PATH));
            ui->showButton->setText("Collapse all");
        }
        else {
            for (int i = 0; i <= ui->jsonTreeView->model()->columnCount(); i++) {
                collapseAll(ui->jsonTreeView->model()->index(0, i));
            }
            Logger::log(INFO, "Tree is collapsed");
            ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
            ui->showButton->setText("Expand all");
        }
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Tree is empty!"));
        Logger::log(ERROR, "Tree is empty!");
        return;
    }
}


void MainWindow::on_jsonTreeView_collapsed(const QModelIndex &index) {
    Logger::log(DEBUG, "Node " + index.data(Qt::DisplayRole).toString() + " is collapsed");
    bool check = true;
    for (int i = 0; i < ui->jsonTreeView->model()->columnCount(); i++) {
        check = isTreeCollapsed(ui->jsonTreeView->model()->index(0, i));
        if (!check) {
            return;
        }
    }
    ui->showButton->setIcon(QIcon(IMAGE_EXPAND_FILE_PATH));
    ui->showButton->setText("Expand all");
}


void MainWindow::on_jsonTreeView_expanded(const QModelIndex &index) {
    Logger::log(DEBUG, "Node " + index.data(Qt::DisplayRole).toString() + " is expanded");
    bool check = true;
    for (int i = 0; i < ui->jsonTreeView->model()->columnCount(); i++) {
        check = isTreeExpanded(ui->jsonTreeView->model()->index(0, i));
        if (!check) {
            return;
        }
    }
    ui->showButton->setIcon(QIcon(IMAGE_COLLAPSE_FILE_PATH));
    ui->showButton->setText("Collapse all");
}

