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

    connect(m_appController, &MainController::loadProjectError,
            this, &LoadingController::onLoadProjectError);

    this->task(
        args, outputCallback, errorCallback,
        [](const QVariantMap &args) {
            QString operation = args.value("operation").toString();
            int operationConv = -1;

            if (operation == "CREATE_PROJECT_SD") operationConv = CREATE_PROJECT_SD;
            if (operation == "CREATE_PROJECT_FOLDER") operationConv = CREATE_PROJECT_FOLDER;
            if (operation == "LOAD_PROJECT") operationConv = LOAD_PROJECT;

            if (operationConv == -1) {
                throw LoadingException(
                    ("operation (" + operation + ") is not supported").toStdString(),
                    "Error interno"
                );
            }

            QVariantMap result = args;
            result.insert("operation", operationConv);

            return result;
        },
        [this](const QVariantMap &result,
               const QJSValue &outputCallback) {

            QString operation = "create";
            LoadingInfo info;
            info.type = result.value("operation").toInt();


            if (info.type == CREATE_PROJECT_SD ||
                info.type == CREATE_PROJECT_FOLDER) {
                info.projectName = result.value("projectName").toString();
                info.projectPath = result.value("projectPath").toString();
                info.frontVolumePath = result.value("frontPath").toString();
                info.backVolumePath = result.value("backPath").toString();
                info.dcimPath = result.value("dcimPath").toString();
                info.copyDCIM = result.value("dcimCopy").toBool();
            }

            if (info.type == LOAD_PROJECT) {
                info.projectPath = result.value("projectPath").toString();
                operation = "load";
            }

            QMetaObject::invokeMethod(
                project,
                operation.toUtf8().constData(),
                Qt::QueuedConnection,
                Q_ARG(LoadingInfo, info)
            );

            outputCallback.call({});
        }
    );
}

void LoadingController::onLoadProjectError(LoadingError error)
{
    qDebug() << "ERROR" << error.message;
}
