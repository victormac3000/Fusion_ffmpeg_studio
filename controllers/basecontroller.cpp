#include "basecontroller.h"
#include "utils/exceptions/customexception.h"

#include <QThreadPool>
#include <QPointer>

BaseController::BaseController(QObject *parent)
    : QObject(parent)
{
}

MainController *BaseController::appController() const
{
    return m_appController;
}

void BaseController::setAppController(MainController *appController)
{
    if (m_appController == appController)
        return;

    m_appController = appController;

    emit appControllerChanged();
}

void BaseController::backgroundTask(const QJSValue &args, const QJSValue &outputCallback,
                                    const QJSValue &errorCallback,
                                    BackgroundTaskFunction workFunction,
                                    BackgroundTaskDoneFunction doneFunction)
{
    if (!args.isObject()) {
        qWarning() << "Tried to run a background task without passing an arguments object";
        return;
    }

    if (!outputCallback.isCallable()) {
        qWarning() << "Tried to run a background task without passing a callable output callback";
        return;
    }

    if (!errorCallback.isCallable()) {
        qWarning() << "Tried to run a background task without passing a callable error callback";
        return;
    }

    if (!workFunction) {
        qWarning() << "Tried to run a background task without a work function";
        return;
    }

    if (!doneFunction) {
        qWarning() << "Tried to run a background task without a completion function";
        return;
    }

    const QVariantMap arguments = args.toVariant().toMap();
    const QJSValue output = outputCallback;
    const QJSValue error = errorCallback;
    const auto done = doneFunction;

    QPointer<BaseController> controller(this);

    QThreadPool::globalInstance()->start(
        [controller, arguments, output, error, workFunction, done]() mutable {
            QVariantMap results;
            QString userError;

            try {
                results = workFunction(arguments);
            } catch (const CustomException& e) {
                userError = QString::fromStdString(e.userMessage());
            } catch (const std::exception& e) {
                userError = QStringLiteral("Unknown error");
                qWarning() << "An unknwon exception was thrown:" << e.what();
            } catch (...) {
                userError = QStringLiteral("Unknown error");
                qWarning() << "An unknwon throwable was thrown";
            }

            if (!controller) {
                qWarning() << "Controller not found";
                return;
            }

            QMetaObject::invokeMethod(controller,
                [controller, results, userError, output, error, done]() mutable {

                    if (!controller) {
                        qWarning() << "Controller not found";
                        return;
                    }

                    done(results, userError, output, error);
                },
                Qt::QueuedConnection
            );
        }
    );
}

void BaseController::task(const QJSValue &args,
                          const QJSValue &outputCallback,
                          const QJSValue &errorCallback,
                          BackgroundTaskFunction workFunction,
                          TaskDoneFunction doneFunction)
{
    if (!args.isObject()) {
        qWarning()
        << "Tried to run a task without passing "
           "an arguments object";
        return;
    }

    if (!outputCallback.isCallable()) {
        qWarning()
        << "Tried to run a task without passing "
           "a callable output callback";
        return;
    }

    if (!errorCallback.isCallable()) {
        qWarning()
        << "Tried to run a task without passing "
           "a callable error callback";
        return;
    }

    if (!workFunction) {
        qWarning()
        << "Tried to run a task without a work function";
        return;
    }

    if (!doneFunction) {
        qWarning()
        << "Tried to run a task without a completion function";
        return;
    }

    const QVariantMap arguments = args.toVariant().toMap();
    QVariantMap results;
    QString userError;

    try {
        results = workFunction(arguments);
    } catch (const CustomException& e) {
        userError = QString::fromStdString(e.userMessage());
    } catch (const std::exception& e) {
        userError = QStringLiteral("Unknown error");
        qWarning()
            << "An unknown exception was thrown:"
            << e.what();
    } catch (...) {
        userError = QStringLiteral("Unknown error");
        qWarning()
            << "An unknown throwable was thrown";
    }

    if (!userError.isEmpty()) {
        errorCallback.call({
            QJSValue(userError)
        });
        return;
    }

    doneFunction(results, outputCallback);
}