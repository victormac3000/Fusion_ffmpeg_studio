#ifndef LOADING_H
#define LOADING_H

#define CREATE_PROJECT_FOLDER 0
#define CREATE_PROJECT_SD 1
#define LOAD_PROJECT 2

#define CHECK_SOURCE_FOLDERS 0
#define CREATE_PROJECT_DIRS 1
#define COPY_DCIM_FOLDER 2
#define INDEX_VIDEOS 3


#include <QString>

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
