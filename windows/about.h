#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QFile>
#include <QSysInfo>

#define LICENSE_MAIN 0
#define LICENSE_LIBRARIES 1

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();
    bool getInit();

private:
    Ui::About *ui;
    QString getLicenseText(int name = LICENSE_MAIN);
    bool init = false;
};

#endif // ABOUT_H
