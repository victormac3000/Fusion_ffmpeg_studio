#ifndef LOADINGINFO_H
#define LOADINGINFO_H

#define CREATE_PROJECT_FOLDER 0
#define CREATE_PROJECT_SD 1
#define LOAD_PROJECT 2

#include <QString>

struct LoadingInfo {
    int type = LOAD_PROJECT;
    QString projectPath;
    QString dcimPath;
    QString projectName;
    bool copyDCIM = false;
};

#endif
