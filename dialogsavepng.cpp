#include "dialogsavepng.h"
#include "ui_dialogsavepng.h"
#include <QFileDialog>

DialogSavePNG::DialogSavePNG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSavePNG)
{
    ui->setupUi(this);
    connect(ui->spinBox_res_h, &QSpinBox::valueChanged, this, [&](){
        hRes = ui->spinBox_res_h->value();
    });
    connect(ui->spinBox_res_v, &QSpinBox::valueChanged, this, [&](){
        vRes = ui->spinBox_res_v->value();
    });

    ui->buttonBox->buttons()[0]->setDisabled(ui->lineEdit->text() == "");

}

DialogSavePNG::~DialogSavePNG()
{
    delete ui;
}

void DialogSavePNG::on_lineEdit_textChanged(const QString &arg1)
{
    ui->buttonBox->buttons()[0]->setDisabled(arg1 == "" || !isLegalFilePath(arg1));
    path = arg1;
}


void DialogSavePNG::on_pushButton_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/jana/untiteled.png", tr("Images (*.png)"));
    ui->lineEdit->setText(filePath);
}

bool DialogSavePNG::isLegalFilePath(QString path)
{
    // Anything following the raw filename prefix should be legal.
    if (path.left(4)=="\\\\?\\")
        return true;

    // Windows filenames are not case sensitive.
    path = path.toUpper();

    // Trim the drive letter off
    if (path[1]==':' && (path[0]>='A' && path[0]<='Z'))
        path = path.right(path.length()-2);

    QString illegal="<>:\"|?*";

    foreach (const QChar& c, path)
    {
        // Check for control characters
         if (c.toLatin1() > 0 && c.toLatin1() < 32)
            return false;

        // Check for illegal characters
        if (illegal.contains(c))
            return false;
    }

    // Check for device names in filenames
    static QStringList devices;

    if (!devices.count())
        devices << "CON" << "PRN" << "AUX" << "NUL" << "COM0" << "COM1" << "COM2"
                << "COM3" << "COM4" << "COM5" << "COM6" << "COM7" << "COM8" << "COM9" << "LPT0"
                << "LPT1" << "LPT2" << "LPT3" << "LPT4" << "LPT5" << "LPT6" << "LPT7" << "LPT8"
                << "LPT9";

    const QFileInfo fi(path);
    const QString basename = fi.baseName();

    foreach (const QString& s, devices)
        if (basename == s)
            return false;

    // Check for trailing periods or spaces
    if (path.right(1)=="." || path.right(1)==" ")
        return false;

    // Check for pathnames that are too long (disregarding raw pathnames)
    if (path.length()>260)
        return false;

    // Exclude raw device names
    if (path.left(4)=="\\\\.\\")
        return false;

    // Since we are checking for a filename, it mustn't be a directory
    if (path.right(1)=="\\")
        return false;

    return true;
}


