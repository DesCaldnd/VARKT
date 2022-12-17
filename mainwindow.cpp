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


    ui->widget->xAxis->setObjectName("Time");

    str << "Time (s)" << "Velocity (m/s)" << "Mass (kg)" << "Height (m)";

    ui->tableWidget->setColumnCount(4);

    setWindowTitle("VARKT");

    buildGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildGraph()
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

    calcValues();
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
        ui->widget->yAxis->setObjectName("Velocity");
        break;
    case 1:
        X = H;
        yMax = hMax;
        ui->widget->yAxis->setObjectName("Height");
        break;
    case 2:
        X = M;
        yMax = mMax;
        ui->widget->yAxis->setObjectName("Mass");
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

void MainWindow::calcValues()
{
    if (!ui->checkBox->isChecked())
    {
        for(x = 0; x < b + 0.0001; x += delta)
        {
            iterations();
        }
        buildTable();
    } else
    {
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::timer_fired);
        timer->start(1);
    }

    endBuild();
}

void MainWindow::buildTable()
{
    rows = T.size();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setHorizontalHeaderLabels(str);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < rows; ++j)
        {
            switch (i)
            {
            case 0:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(T[j])));
                break;
            case 1:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(V[j])));
                break;
            case 2:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(M[j])));
                break;
            case 3:
                ui->tableWidget->setItem(j, i, new QTableWidgetItem(QString::number(H[j])));
                break;
            }
        }
    ui->tableWidget->resizeColumnsToContents();

}

void MainWindow::timer_fired()
{
    iterations();
    x += delta;
    buildTable();
    endBuild();
    if (x > b)
        timer->stop();
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
    delta = position/100.0;
    buildGraph();
}

void MainWindow::on_actionSave_PNG_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/jana/untiteled.png", tr("Images (*.png)"));
    ui->widget->savePng(filePath, 0, 0, 3,-1, 96);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    buildGraph();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    buildGraph();
}

