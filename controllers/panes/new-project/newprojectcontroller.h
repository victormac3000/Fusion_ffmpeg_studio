#ifndef NEWPROJECTCONTROLLER_H
#define NEWPROJECTCONTROLLER_H

#include "controllers/basecontroller.h"

#include <QObject>


class NewProjectController : public BaseController
{
    Q_OBJECT
public:
    explicit NewProjectController(QObject *parent = nullptr);

    Q_INVOKABLE void importSDClicked();
    Q_INVOKABLE void importDCIMFolderClicked();

signals:
};

#endif // NEWPROJECTCONTROLLER_H
