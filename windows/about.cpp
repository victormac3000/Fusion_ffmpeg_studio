#include "about.h"
#include "QtQuick/qquickitem.h"
#include "ui_about.h"
#include "utils/settings.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    QQuickItem* root = ui->quickWidget->rootObject();

    if (root == nullptr) {
        qWarning() << "About could not found quickWidget element root QML object";
        return;
    }

    QQuickItem* versionText = root->findChild<QQuickItem*>("versionText");
    QQuickItem* licenseTextArea = root->findChild<QQuickItem*>("licenseTextArea");
    librariesInfoGrid = root->findChild<QQuickItem*>("librariesInfoGrid");
    QQuickItem* buildInfoGrid = root->findChild<QQuickItem*>("buildInfoGrid");


    if (versionText == nullptr || licenseTextArea == nullptr || librariesInfoGrid == nullptr ||
        buildInfoGrid == nullptr) {
        qWarning() << "Could not find QML objects";
        return;
    }

    QString buildDatetime = QStringLiteral(__DATE__) + " " + QStringLiteral(__TIME__);
    QString compilationType = "Release";
    #ifdef QT_DEBUG
    compilationType = "Debug";
    #endif

    QString ffmpegPath = Settings::getFFmpegPath();
    QString ffprobePath = Settings::getFFprobePath();

    connect(&ffmpegProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(ffmpegVersionDone(int,QProcess::ExitStatus)));
    connect(&ffprobeProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(ffprobeVersionDone(int,QProcess::ExitStatus)));

    ffmpegProc.start(ffmpegPath, {"-version"});
    ffprobeProc.start(ffprobePath, {"-version"});

    versionText->setProperty("text", QCoreApplication::applicationVersion());
    licenseTextArea->setProperty("areaText", getLicenseText());
    librariesInfoGrid->setProperty("qtVersion", QT_VERSION_STR);
    librariesInfoGrid->setProperty("ffmpegVersion", "N/A");
    librariesInfoGrid->setProperty("ffprobeVersion", "N/A");
    buildInfoGrid->setProperty("compilationDate", buildDatetime);
    buildInfoGrid->setProperty("compilationType", compilationType);
    buildInfoGrid->setProperty("compilationOSName", QSysInfo::productType());
    buildInfoGrid->setProperty("compilationOSVersion", QSysInfo::productVersion());

    init = true;
}

About::~About()
{
    delete ui;
}

bool About::getInit()
{
    return init;
}

void About::ffmpegVersionDone(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode > 0) {
        qWarning() << "Could not get ffmpeg version. Exit code was" << exitCode;
        return;
    }

    if (exitStatus == QProcess::CrashExit) {
        qWarning() << "Could not get ffmpeg version. Process crashed";
        return;
    }

    QString consoleOut = ffmpegProc.readAllStandardOutput();
    QString consoleErrorOut = ffmpegProc.readAllStandardError();

    static const QRegularExpression regex("^ffmpeg version ([A-Za-z0-9\\-]+)");
    QRegularExpressionMatch match = regex.match(consoleOut);

    if (!match.hasMatch()) {
        qWarning() << "FFmpeg process out could not find version"
                   << consoleOut << consoleErrorOut;
        return;
    }

    QString version = match.captured(1);
    librariesInfoGrid->setProperty("ffmpegVersion", version);
}

void About::ffprobeVersionDone(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode > 0) {
        qWarning() << "Could not get ffprobe version. Exit code was" << exitCode;
        return;
    }

    if (exitStatus == QProcess::CrashExit) {
        qWarning() << "Could not get ffprobe version. Process crashed";
        return;
    }

    QString consoleOut = ffprobeProc.readAllStandardOutput();
    QString consoleErrorOut = ffprobeProc.readAllStandardError();

    static const QRegularExpression regex("^ffprobe version ([A-Za-z0-9\\-]+)");
    QRegularExpressionMatch match = regex.match(consoleOut);

    if (!match.hasMatch()) {
        qWarning() << "FFprobe process out could not find version"
                   << consoleOut << consoleErrorOut;
        return;
    }

    QString version = match.captured(1);
    librariesInfoGrid->setProperty("ffprobeVersion", version);
}

QString About::getLicenseText(int license)
{
    QString licenseText = "Could not find license contents";
    QString licensePath = "";

    switch (license) {
        case LICENSE_MAIN:
            licensePath = ":/Documents/Txt/LICENSE";
            break;
        case LICENSE_LIBRARIES:
            licensePath = ":/Documents/Txt/LICENSE_LIBS";
            break;
        default:
            return licenseText;
            break;
    }

    QFile licenseFile(licensePath);

    if (!licenseFile.open(QFile::ReadOnly)) {
        qWarning() << "Could not open main license file";
        return licenseText;
    }

    if (licenseFile.size() < 1) {
        qWarning() << "The license file is empty";
        return licenseText;
    }

    licenseText = licenseFile.readAll();
    licenseFile.close();

    return licenseText;
}
