#ifndef LOADING_H
#define LOADING_H

#define CREATE_PROJECT_FOLDER 0
#define CREATE_PROJECT_SD 1
#define LOAD_PROJECT 2

#include <QString>

struct LoadingInfo {
    int type = LOAD_PROJECT;
    QString rootProjectPath;    // Root folder of project. Example: /Users/username/Documents
    QString projectPath;        // Base folder of project. Example: /Users/username/Documents/Project1
    QString dcimPath;
    QString dcimFrontPath, dcimBackPath;
    QString projectName;
    bool copyDCIM = false;
};

struct LoadingProgress {
    QString stepMessage;
    double stepProgress;  // 0-100
    int stepNumber;
    int stepCount;
};

#endif
