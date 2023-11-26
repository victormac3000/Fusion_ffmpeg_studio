#include "videoitem.h"

VideoItem::VideoItem(QObject *parent) :
    QObject(parent)
{
    this->m_selected = false;
    this->m_idString = "VIDEO N/A";
    this->m_imagePath = "Icons/VideoPlayer/no_video.png";
    this->m_recorded = "DD/MM/YYYY";
}

bool VideoItem::selected()
{
    return m_selected;
}

QString VideoItem::idString()
{
    return m_idString;
}

QString VideoItem::imagePath()
{
    return m_imagePath;
}

QString VideoItem::recorded()
{
    return m_recorded;
}

void VideoItem::setSelected(bool selected)
{
    this->m_selected = selected;
    emit selectedChanged();
}

void VideoItem::setIdString(QString idString)
{
    this->m_idString = idString;
    emit idStringChanged();
}

void VideoItem::setImagePath(QString imagePath)
{
    this->m_imagePath = imagePath;
    emit imagePathChanged();
}

void VideoItem::setRecorded(QString recorded)
{
    this->m_recorded = recorded;
    emit recordedChanged();
}
