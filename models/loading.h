#ifndef LOADING_H
#define LOADING_H

#include <QString>
#include <QtCore/qobject.h>

enum {
    CREATE_PROJECT_FOLDER,
    CREATE_PROJECT_SD,
    LOAD_PROJECT
};

enum {
    CHECK_SOURCE_FOLDERS,
    CREATE_PROJECT_DIRS,
    COPY_DCIM_FOLDER,
    INDEX_VIDEOS
};

struct LoadingInfo {
    int type = -1;
    QString rootProjectPath;    // Root folder of project. Example: /Users/username/Documents
    QString projectPath;        // Base folder of project. Example: /Users/username/Documents/Project1
    QString projectName;        // Only used on create type
    QString dcimPath;
    QString dcimFrontPath, dcimBackPath;
    QString frontVolumePath, backVolumePath;
    bool copyDCIM = false;
};

struct LoadingProgress {
    int stepID = -1;
    int stepNumber = -1;
    int stepCount = -1;
    struct index {
        int doneVideos = 0;
        int totalVideos = 0;
        int doneSegments = 0;
        int totalSegments = 0;
    } index;
    struct copy {
        int fileCount = 0;
        int fileNumber = 0;
        struct currentFile {
            QString name;
            qint64 bytesCount = 0;
            qint64 bytesDone = 0;
            float speed = 0.0;   // In MB/s
        } currentFile;
    } copy;
};

struct LoadingError {
    QString message;
    LoadingProgress progress;
};

#endif
