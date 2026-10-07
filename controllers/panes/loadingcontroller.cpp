#include "loadingcontroller.h"
#include "utils/exceptions/loadingexception.h"

LoadingController::LoadingController(QObject *parent)
    : BaseController{parent}
{
    qDebug() << "LoadingController thread" << QThread::currentThreadId();
}

void LoadingController::startLoading(const QJSValue& args,
                                     const QJSValue& outputCallback,
                                     const QJSValue& errorCallback)
{
    project = m_appController->getProject();

    connect(m_appController, &MainController::loadProjectUpdate,
            this, &LoadingController::onLoadProjectUpdate);
    connect(m_appController, &MainController::loadProjectDone,
            this, &LoadingController::onLoadProjectDone);
    connect(m_appController, &MainController::loadProjectError,
            this, &LoadingController::onLoadProjectError);

    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap& args) {
            return args;
        },
        [this](const QVariantMap &result,
               const QJSValue &outputCallback) {
            LoadingInfo info {
                .type = result.value("operation").toInt(),
                .projectPath = result.value("projectPath").toString(),
                .projectName = result.value("projectName").toString(),
                .dcimPath = result.value("dcimPath").toString(),
                .frontVolumePath = result.value("frontPath").toString(),
                .backVolumePath = result.value("backPath").toString(),
                .copyDCIM = result.value("dcimCopy").toBool()
            };

            QMetaObject::invokeMethod(
                project,
                "create",
                Qt::QueuedConnection,
                Q_ARG(LoadingInfo, info)
            );

            outputCallback.call({});
        }
    );
}

void LoadingController::onLoadProjectUpdate(LoadingProgress progressData)
{
    QVariantMap data;
    QString operation;

    switch (progressData.stepID) {
        case GENERATE_PROJECT_DIRS:
            operation = "Generating project files";
            break;
        case COPY_DCIM_FOLDER:
            operation = "Copying video files";
            break;
        case INDEX_VIDEOS:
            operation = "Importing video files";
            break;
        default:
            operation = "Loading";
    }

    data = progressData.toQML();
    data.insert("operation", operation);

    emit loadProjectUpdate(data);
}

void LoadingController::onLoadProjectDone(LoadingDone done)
{
    QVariantMap map;
    map.insert("badVideos", done.badVideos);
    map.insert("numVideos", done.numVideos);
    emit loadProjectDone(map);
}

void LoadingController::onLoadProjectError(LoadingError error)
{
    QVariantMap data;
    QString title = "Error loading project";

    switch (error.progress.stepID) {
        case CREATE_PROJECT_FOLDER:
        case CREATE_PROJECT_SD:
            title = "Error creating project";
    }

    data["title"] = title;
    data["message"] = error.message;


    emit loadProjectError(data);
}
