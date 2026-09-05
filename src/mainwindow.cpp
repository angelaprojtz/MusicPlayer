#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select Audio Files"), "/home", tr("Audio Files (*.mp3 *.wav)"));

    for(const QString &currFile : fileNames){
        QFileInfo fi(currFile);
        QString name = fi.fileName();

        ui->listWidget->addItem(name);

        qDebug() << name;
    }
}


void MainWindow::on_remButton_clicked()
{
    QListWidgetItem *it = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete it;
}
