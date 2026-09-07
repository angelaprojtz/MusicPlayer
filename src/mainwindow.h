#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void durationChanged(qint64 duration);

    void positionChanged(qint64 progress);

    void on_addButton_clicked();

    void on_remButton_clicked();

    void on_playButton_clicked();

    void on_pauseButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_progressSlider_sliderReleased();

    void on_volumeSlider_valueChanged(int value);

private:
    void updateduration(qint64 duration);
    Ui::MainWindow *ui;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    qint64 Mduration;

};
#endif // MAINWINDOW_H
