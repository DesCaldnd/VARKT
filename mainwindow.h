#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QTimer>
#include <QTableWidgetItem>

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

    void on_horizontalSlider_sliderMoved(int position);

    void on_actionSave_PNG_triggered();

    void on_comboBox_currentIndexChanged(int index);

    void timer_fired();

    void on_checkBox_stateChanged(int arg1);

    void on_spinBox_editingFinished();

    void on_horizontalSlider_sliderReleased();

private:
    Ui::MainWindow *ui;
    long double x = 0, t = 0, delta = 0.01, v = 0, vPrevious = 0, m = 65149,  mPrevious = 0, maxX = 0, h = 0, hPrevious = 0;
    QVector<double> T, M, V, H;
    const long double a = 0, b = 700, mDelta = 128, mMin = 10977, U = 2080, G = 0.000000000066, mEarth = 6000000000000000000000000.0;
    const long double rEarth = 6400000;
    double currentPos = 0;
    double mMax = 65149, vMax = 0, hMax = 0;
    QTimer* timer = new QTimer(this);
    int rows = 1, time = 0, timedelta = 33;
    QStringList str;
    bool isMoving = false;

    void buildGraph(bool);
    void calcValues(bool);
    void mass();
    void velocity();
    void height();
    void iterations();
    void endBuild();
    void buildTable();

};
#endif // MAINWINDOW_H
