#ifndef LOADING_H
#define LOADING_H

#include <QString>

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
    bool copyDCIM = false;
};

struct LoadingProgress {
    int stepID;
    int stepNumber;
    int stepCount;
    struct index {
        int doneVideos;
        int totalVideos;
        int doneSegments;
        int totalSegments;
    } index;
    struct copy {
        int fileCount;
        int fileNumber;
        struct currentFile {
            QString name;
            qint64 bytesCount;
            qint64 bytesDone;
            float speed;   // In MB/s
        } currentFile;
    } copy;
};

#endif
