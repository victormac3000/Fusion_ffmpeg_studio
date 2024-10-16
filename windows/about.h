#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QFile>
#include <QSysInfo>
#include <QQuickItem>
#include <QProcess>

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

private slots:
    void ffmpegVersionDone(int exitCode, QProcess::ExitStatus exitStatus);
    void ffprobeVersionDone(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::About *ui;
    QQuickItem* librariesInfoGrid;
    QString getLicenseText(int name = LICENSE_MAIN);
    QProcess ffmpegProc, ffprobeProc;
    bool init = false;
};

#endif // ABOUT_H
