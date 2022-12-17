#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Velocity");
    ui->comboBox->addItem("Height");
    ui->comboBox->addItem("Mass");

    str << "Time (s)" << "Velocity (m/s)" << "Mass (kg)" << "Height (m)";

    ui->tableWidget->setColumnCount(4);

    ui->widget->xAxis->setLabel("Time");

    setWindowTitle("VARKT");

    ui->tableWidget->resize(4, b / 0.01);

    connect(timer, &QTimer::timeout, this, &MainWindow::timer_fired);

    buildGraph(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildGraph(bool tab)
{
    T.clear();
    x = 0;
    t = 0;
    v = 0;
    vPrevious = 0;
    m = 65149;
    mPrevious = 0;
    maxX = 0;
    h = 0;
    hPrevious = 0;
    M.clear();
    V.clear();
    H.clear();
    currentPos = 0;
    if (timer->isActive())
        timer->stop();

    calcValues(tab);
}

void MainWindow::iterations()
{
    T.push_back(x);
    M.push_back(m);
    V.push_back(v);
    H.push_back(h);

    mMax = mMax > m ? mMax : m;
    vMax = vMax > v ? vMax : v;
    hMax = hMax > h ? hMax : h;

    mass();
    height();
    velocity();
}

void MainWindow::endBuild()
{
    QVector<double> X;
    double yMax = 0;
    switch (ui->comboBox->currentIndex())
    {
    case 0:
        X = V;
        yMax = vMax;
        ui->widget->yAxis->setLabel("Velocity");
        break;
    case 1:
        X = H;
        yMax = hMax;
        ui->widget->yAxis->setLabel("Height");
        break;
    case 2:
        X = M;
        yMax = mMax;
        ui->widget->yAxis->setLabel("Mass");
        break;
    }

    ui->widget->xAxis->setRange(0, b);
    ui->widget->yAxis->setRange(0, yMax * 1.1);

    if (ui->widget->graphCount() == 0)
    {
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(X, T);
    }

    ui->widget->graph(0)->setData(T, X);

    ui->widget->replot();
}

void MainWindow::calcValues(bool tab)
{
    if (!ui->checkBox->isChecked())
    {
        for(x = 0; x < b + 0.0001; x += delta)
        {
            iterations();
        }
        if (!isMoving && tab)
            buildTable();
        endBuild();
    } else
    {

        time = 0;
        timer->start(timedelta);
    }
}

void MainWindow::buildTable()
{
    rows = T.size();
    int tableSize = rows > maxRows ? maxRows : rows;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(tableSize);
    ui->tableWidget->setHorizontalHeaderLabels(str);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < tableSize; ++j)
        {
            switch (i)
            {
            case 0:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(T[(int)(j * ((double) rows / tableSize))])));
                break;
            case 1:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(V[(int)(j * ((double) rows / tableSize))])));
                break;
            case 2:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(M[(int)(j * ((double) rows / tableSize))])));
                break;
            case 3:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(H[(int)(j * ((double) rows / tableSize))])));
                break;
            }
        }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

}

void MainWindow::timer_fired()
{
    time += timedelta;
    int n = (((long double)time / 5000.0) - (x / b)) * (b / delta);
    for (int i = 0; i < n; i++)
    {
        iterations();
        x += delta;
    }
    endBuild();
    if (x > b)
    {
        timer->stop();
        if (!isMoving)
            buildTable();
    }
}

void MainWindow::mass()
{
    mPrevious = m;
    long double x = m - mDelta * delta;
    if (x < mMin + 7000)
    {
        m -= mDelta * delta * (1 - (mMin + 7000 - x) / 7000);
    } else
        m -= mDelta * delta;
}

void MainWindow::velocity()
{
    long double tmpVelocity = v;
    v = vPrevious - U * logl(m / mPrevious) - (G * mEarth)/powl(rEarth + h + h - hPrevious, 2) * logl(m / mPrevious);
    vPrevious = tmpVelocity;
}

void MainWindow::height()
{
    hPrevious = h;
    long double x = h + b * delta;
    if (x > 200000)
    {
        h += v * delta * (240000 - x) / 40000;
    } else
        h += v * delta;
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    isMoving = true;
    delta = position/100.0;
    buildGraph(false);
}

void MainWindow::on_actionSave_PNG_triggered()
{
    DialogSavePNG dialog;
    connect(&dialog, &DialogSavePNG::accepted, this, [&](){
        ui->widget->savePng(dialog.path, dialog.hRes, dialog.vRes);
    });
    dialog.exec();
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    buildGraph(false);
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    buildGraph(false);
}


void MainWindow::on_spinBox_editingFinished()
{
    timedelta = ui->spinBox->value();
    buildGraph(false);
}


void MainWindow::on_horizontalSlider_sliderReleased()
{
    isMoving = false;
    buildGraph(true);
}


void MainWindow::on_spinBoxTable_editingFinished()
{
    maxRows = ui->spinBoxTable->value();
    buildTable();
}

