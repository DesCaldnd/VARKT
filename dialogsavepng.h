#ifndef DIALOGSAVEPNG_H
#define DIALOGSAVEPNG_H

#include <QDialog>

namespace Ui {
class DialogSavePNG;
}

class DialogSavePNG : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSavePNG(QWidget *parent = nullptr);
    ~DialogSavePNG();

    int hRes = 1920, vRes = 1080;
    QString path = "";

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::DialogSavePNG *ui;
    bool isLegalFilePath(QString path);
};

#endif // DIALOGSAVEPNG_H
