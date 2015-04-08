#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWidget>
#include <QVector>
#include <QSettings>
#include <QDebug>
#include <QDir>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <string>

using namespace std;

struct Preset {
    string name;
    string imgPath;
    string musicPath;
    string color1;
    string color2;
    string color3;
    string color4;
    
    const char* colors[5];
};

class SettingsManager //: public QGraphicsItem
{
public:
    SettingsManager();
    ~SettingsManager();
    void readConfigFile();
    const QList<Preset> getPresetArray();
private:
    QList<Preset> presetArray;
    void parseAndStore(const QJsonObject &json);
};

/*
class Preset //: public QGraphicsItem
{
public:
    SettingsManager();
    ~SettingsManager();
private:
    
};
*/


#endif // SETTINGSMANAGER_H
