#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this); //The player is built once when the window opens, not every time someone hits play.
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    ui->volumeSlider->setMinimum(0.0);
    ui->volumeSlider->setMaximum(100.0);
    ui->volumeSlider->setValue(50.0);
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

        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, currFile); //Full path is hidden on Qt::UserRole, user only sees file name
        ui->listWidget->addItem(item);

        qDebug() << name;
    }
}


void MainWindow::on_remButton_clicked()
{
    QListWidgetItem *it = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete it;
}

void MainWindow::on_playButton_clicked()
{
    QListWidgetItem *item = ui->listWidget->currentItem();

    QString path = item->data(Qt::UserRole).toString();
    player->setSource(QUrl::fromLocalFile(path));
    player->play();

    ui->nowPlayingActual->setText(item->text());
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = item->data(Qt::UserRole).toString();
    player->setSource(QUrl::fromLocalFile(path));
    player->play();

    ui->nowPlayingActual->setText(item->text());
}


void MainWindow::on_pauseButton_clicked()
{
    player->pause();
}


void MainWindow::on_progressSlider_valueChanged(int value)
{

}


void MainWindow::on_volumeSlider_valueChanged(int value)
{
    audioOutput->setVolume(value/100.0);

    ui->volumeValue->setText(QString::number(value) + "%");
}

