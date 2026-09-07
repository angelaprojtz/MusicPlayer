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

    setWindowTitle("Music Player");

    player = new QMediaPlayer(this); //The player is built once when the window opens, not every time someone hits play.
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);

    ui->volumeSlider->setMinimum(0.0);
    ui->volumeSlider->setMaximum(100.0);
    ui->volumeSlider->setValue(50.0);

    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    ui->progressSlider->setRange(0,player->duration() / 1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateduration(qint64 duration)
{
    QString timeString;
    if (duration || Mduration)
    {
        QTime CurrentTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000);
        QTime totalTime((Mduration / 3600) % 60, (Mduration / 60) % 60, Mduration % 60, (Mduration * 1000) % 1000);

        QString format = "mm:ss";
        if (Mduration > 3600)
            format = "hh:mm:ss";
        ui->progressCurr->setText(CurrentTime.toString(format));
        ui->fullDurationLabel->setText(totalTime.toString(format));
    }
}

void MainWindow::durationChanged(qint64 duration)
{
    Mduration = duration / 1000;
    ui->progressSlider->setMaximum(Mduration);
}

void MainWindow::positionChanged(qint64 progress)
{
    if (!ui->progressSlider->isSliderDown())
    {
        ui->progressSlider->setValue(progress / 1000);
    }

    updateduration(progress / 1000);
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
    if (player->playbackState() == QMediaPlayer::PausedState)
    {
        player->play();

        return;
    }
    else if (player->playbackState() == QMediaPlayer::PlayingState ||
             player->playbackState() == QMediaPlayer::StoppedState)
    {
        return;
    }

    else {

        QListWidgetItem *item = ui->listWidget->currentItem();

        QString path = item->data(Qt::UserRole).toString();
        player->setSource(QUrl::fromLocalFile(path));
        player->play();

        ui->nowPlayingActual->setText(item->text());
    }
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


void MainWindow::on_progressSlider_sliderReleased()
{
    player->setPosition(ui->progressSlider->value() * 1000);
}


void MainWindow::on_volumeSlider_valueChanged(int value)
{
    audioOutput->setVolume(value/100.0);

    ui->volumeValue->setText(QString::number(value) + "%");
}

void MainWindow::on_searchButton_clicked()
{
    QString searchText = ui->searchSong->text();

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);
        bool match = item->text().contains(searchText, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}


void MainWindow::on_prevButton_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    int earlierRow = currentRow - 1;

    if (earlierRow >= 0) {
        ui->listWidget->setCurrentRow(earlierRow);
    }

    QListWidgetItem *item = ui->listWidget->currentItem();

    QString path = item->data(Qt::UserRole).toString();
    player->setSource(QUrl::fromLocalFile(path));
    player->play();

    ui->nowPlayingActual->setText(item->text());
}


void MainWindow::on_skipButton_clicked()
{
    int currentRow = ui->listWidget->currentRow();
    int nextRow = currentRow + 1;

    if (nextRow < ui->listWidget->count()) {
        ui->listWidget->setCurrentRow(nextRow);
    }

    QListWidgetItem *item = ui->listWidget->currentItem();

    QString path = item->data(Qt::UserRole).toString();
    player->setSource(QUrl::fromLocalFile(path));
    player->play();

    ui->nowPlayingActual->setText(item->text());
}

