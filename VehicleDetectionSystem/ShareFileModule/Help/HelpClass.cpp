#include "HelpClass.h"
#include <QDateTime>
#include <QCryptographicHash>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QDebug>
#include <QSettings>
#include <QVariant>
#include <QApplication>
#include <QFontMetrics>
#include "ToolTip.h"
#include <QClipboard>
#include "QNGraphicsEffect.h"

QJsonObject HelpClass::m_jsonObject;
QLabel* HelpClass::m_label = nullptr;

HelpClass::HelpClass(QObject *parent) : QObject(parent)
{

}

QByteArray HelpClass::GetEncrypt(const QByteArray plaintextStr, QByteArray key)
{
    QByteArray encryptText = encryption(plaintextStr);
    if(key.isEmpty())
    {
        key = DefaultKEY;
    }
    return getXorEncryptDecrypt(encryptText, key);
}

QByteArray HelpClass::GetEncrypt(const QString plaintextStr, QByteArray key)
{
    QByteArray encryptText = encryption(plaintextStr);
    if(key.isEmpty())
    {
        key = DefaultKEY;
    }
    return getXorEncryptDecrypt(encryptText, key);
}

QByteArray HelpClass::GetDecrypt(const QByteArray ciphertext, QByteArray key)
{
    if(key.isEmpty())
    {
        key = DefaultKEY;
    }

    QByteArray plaintextByte = getXorEncryptDecrypt(ciphertext, key);

    return Deciphering(plaintextByte);
}

bool HelpClass::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
        qDebug() << "dir exists" << fullPath;
        return true;
    }
    else
    {
        qDebug() << "create path fullPath";
        bool ok = dir.mkpath(fullPath);//创建多级目录
        return ok;
    }
}

QString HelpClass::bytesToKB(qint64 size)
{
    qint64 rest = 0.00;
    if(size < 1024)
    {
        return QString::number(size, 'f', 2) + "B";
    }
    else
    {
        size /= 1024;
    }
    if(size < 1024)
    {
        return QString::number(size, 'f', 2) + "KB";
    }
    else
    {
        rest = size % 1024;
        size /= 1024;
    }
    if(size < 1024)
    {
        size = size * 100;
        return QString::number((size / 100), 'f', 2) + "." + QString::number((rest * 100 / 1024 % 100), 'f', 2) + "MB";
    }
    else
    {
        size = size * 100 / 1024;
        return QString::number((size / 100), 'f', 2) + "." + QString::number((size % 100), 'f', 2) + "GB";
    }
}

bool HelpClass::removeFile(QString filePath)
{
    if(filePath.isEmpty())
    {
        filePath = getCurrentTempDataDir()+"/"+AgencyFileName;
    }
    QFile * file = new QFile(filePath);
    if(file->exists())
    {
        if(file->remove())
        {
            delete file;
            return true;
        }
        else
        {
            delete file;
            return false;
        }
    }
    else
    {
        qCritical("File '%s' does not exist!", qUtf8Printable(filePath));
    }
    delete file;
    return false;
}

QString HelpClass::getTimestamp()
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t(); //获取时间戳

    return QString::number(timeT);
}

QString HelpClass::md5Encrypt(QString plaintextStr)
{
    if(plaintextStr.isEmpty())
    {
        return "";
    }
    QByteArray plaintextByArray = plaintextStr.toLocal8Bit();
    QByteArray byteMd5 = QCryptographicHash::hash(plaintextByArray, QCryptographicHash::Md5);

    QString strPwdMd5 = byteMd5.toHex();

    return strPwdMd5;
}

QString HelpClass::getCurrentTime(const QString &format)
{
    QTime  currenttime = QTime::currentTime();
    if(format.isEmpty())
    {
        return currenttime.toString();
    }

    return currenttime.toString(format);
}

QString HelpClass::getCurrentDateTime(const QString &format)
{
    QDateTime date =  QDateTime::currentDateTime();

    if(format.isEmpty())
    {
        return date.toString();
    }

    return date.toString(format);
}

QString HelpClass::msecsTo(const QString &timeStr)
{
    QTime time = QTime::fromString(timeStr);
    int mesecs = time.msecsTo(QTime::currentTime());

    return QString::number(mesecs);
}

void HelpClass::jsonCreate(const QString &key, const QJsonValue &value, bool isClean)
{
    if(isClean)
    {
        QStringList keyList = m_jsonObject.keys();
        for(QString key : keyList)
        {
            m_jsonObject.remove(key);
        }
    }
    if(key.isEmpty())
    {
        return;
    }
    m_jsonObject.insert(key, value);
}

QJsonObject HelpClass::getJsonObject()
{
    return m_jsonObject;
}

QString HelpClass::tojsonArray(QJsonArray jsonArray)
{
    QJsonDocument document;
    document.setArray(jsonArray);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);

    return strJson;
}

bool HelpClass::generalJsonParse(QJsonDocument jsonDocument, QMap<QString, QString> &jsonkeyMap, QString headKey, int headValue, bool flage)
{
    if(jsonDocument.isObject())
    {
        QJsonObject object = jsonDocument.object();

        if(object.contains(headKey))
        {
            int code = object.value(headKey).toInt();
            if(flage || code== headValue)
            {
                QStringList keyList = jsonkeyMap.keys();

                for(QString key : keyList)
                {
                    if(object.contains(key))
                    {
                        QJsonValue value = object.value(key);
                        QVariant varVariant = value.toVariant();
                        jsonkeyMap[key] = varVariant.toString();
                    }
                }
                return true;
            }
            else
            {
                QStringList keyList = jsonkeyMap.keys();
                for(QString key : keyList)
                {
                    if(object.contains(key))
                    {
                        QJsonValue value = object.value(key);
                        QVariant varVariant = value.toVariant();
                        jsonkeyMap[key] = varVariant.toString();
                    }
                }
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    if(jsonDocument.isArray())
    {
        QJsonArray array = jsonDocument.array();
        for(int i = 0; i < array.size(); i++)
        {
            QJsonValue value =  array.at(i);

            if(value.isObject())
            {
                QJsonObject object = array.at(i).toObject();
                if(flage || object.contains(headKey))
                {
                    if(flage  || object.take(headKey)== headValue)
                    {
                        QStringList keyList = jsonkeyMap.keys();

                        for(QString key : keyList)
                        {
                            if(object.contains(key))
                            {
                                QJsonValue value = object.value(key);
                                QVariant varVariant = value.toVariant();
                                jsonkeyMap[key] = varVariant.toString();
                            }
                        }
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }

        }
    }

    return false;
}

bool HelpClass::generalArrayJsonParse(QJsonDocument jsonDocument, QList<QMapString> &listJsonkepMap, QMapString jsonkey, QString arrayKey, QString headKey, int headValue)
{
    //只判断是对象的情况，数组情况目前不做判断
    if(jsonDocument.isObject())
    {
        QJsonObject jsonObject = jsonDocument.object();

        if(jsonObject.contains(headKey))
        {
            if(jsonObject.take(headKey).toInt() == headValue)
            {
                QJsonValue jsonValue = jsonObject.value(arrayKey);

                //进行判断，是否是数组
                if(jsonValue.isArray())
                {
                    QJsonArray array = jsonValue.toArray();

                    for(int i = 0; i < array.size(); i++)
                    {
                        QJsonValue value =  array.at(i);

                        if(value.isObject())
                        {
                            QJsonObject object = array.at(i).toObject();

                            QStringList keyList = jsonkey.keys();
                            QMapString tempMap;
                            for(QString key : keyList)
                            {
                                if(object.contains(key))
                                {
                                    QJsonValue value = object.value(key);
                                    QVariant varVariant = value.toVariant();

                                    tempMap[key] = varVariant.toString();

                                }
                            }
                           listJsonkepMap.append(tempMap);
                        }
                        else if(value.isString())
                        {
                            QMapString tempMap;

                            QJsonValue JsonValue = array.at(i).toString();
                            tempMap["text"] = JsonValue.toString();

                            listJsonkepMap.append(tempMap);
                        }
                    }

                    return true;
                }
                else if(jsonValue.isObject())
                {
                    //特殊情况，可能名称相同，先对象后数组，如果再不是，直接返回false
                        QJsonObject jsonobject = jsonValue.toObject();
                        //再取一次
                        if(jsonobject.contains(arrayKey))
                        {
                              QJsonValue jsonvalue = jsonobject.value(arrayKey);
                              //进行判断，是否是数组
                              if(jsonvalue.isArray())
                              {
                                  QJsonArray array = jsonvalue.toArray();

                                  for(int i = 0; i < array.size(); i++)
                                  {
                                      QJsonValue value =  array.at(i);

                                      if(value.isObject())
                                      {
                                          QJsonObject object = array.at(i).toObject();

                                          QStringList keyList = jsonkey.keys();

                                          QMapString tempMap;
                                          for(QString key : keyList)
                                          {
                                              if(object.contains(key))
                                              {
                                                  QJsonValue value = object.value(key);
                                                  QVariant varVariant = value.toVariant();

                                                  tempMap[key] = varVariant.toString();
                                              }
                                          }

                                          listJsonkepMap.append(tempMap);
                                      }
                                  }
                              }
                        }

                        return true;
                }
                return false;
            }
        }
    }

    return false;
}

QString HelpClass::getUuid()
{
    return  QUuid::createUuid().toString();
}

QString HelpClass::getCurrentTempDataDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
}

bool HelpClass::writeDataToFile(QList<QMap<QString, QStringList> > mapList, QString Path)
{
    if(Path.isEmpty())
    {
        Path = getCurrentTempDataDir()+"/Data";
    }

    if(!isDirExist(Path))
    {
        return false;
    }

    Path = Path +"/"+AgencyFileName;

    //保险起见先把原先的文件删除再重写
    removeFile(Path);

    QFile * file = new QFile(Path);

    //默认只写，不会追加前面已有的数据，而是覆盖前面的数据
    if(!file->open(QIODevice::WriteOnly)){
        qInfo()<<QString("%1 file WriteOnly open failed~").arg(Path);
        return false;
    }
    QDataStream  dataStream(file);
    dataStream.setVersion(QDataStream::Qt_5_9);
    QList<QMap<QString, QStringList> >::ConstIterator iter = mapList.constBegin();

    while(iter != mapList.constEnd())
    {
        dataStream<<(*iter);

        iter++;
    }

    file->close();

    delete file;


    return true;
}

QList<QMap<QString, QStringList> > HelpClass::ReadDataFromFile(QString Path)
{
    isDirExist(getCurrentTempDataDir()+"/Data/");
    if(Path.isEmpty())
    {
        Path = getCurrentTempDataDir()+"/Data/"+AgencyFileName;
    }

    QFile * file = new QFile(Path);
    QList<QMap<QString, QStringList> > mapList;
    //默认只写，不会追加前面已有的数据，而是覆盖前面的数据
    if(!file->open(QIODevice::ReadOnly)){
        qCritical()<<QString("%1 file ReadOnly open failed~").arg(Path);
        return mapList;
    }

    QDataStream  dataStream(file);
    dataStream.setVersion(QDataStream::Qt_5_9);

    while(!dataStream.atEnd())
    {
        QMap<QString, QStringList>  mapData;
        dataStream>>mapData;

        mapList.append(mapData);
    }
    file->close();

    delete file;

    return mapList;
}

QByteArray HelpClass::encryption(const QByteArray plaintextStr)
{
    QByteArray ciphertext = plaintextStr.toBase64();

    return ciphertext;
}

QByteArray HelpClass::encryption(const QString plaintextStr)
{
    QByteArray plaintextByte = plaintextStr.toLocal8Bit();

    return encryption(plaintextByte);
}

QByteArray HelpClass::Deciphering(const QByteArray ciphertext)
{
    QByteArray plaintext = QByteArray::fromBase64(ciphertext);
    return plaintext;
}

QByteArray HelpClass::getXorEncryptDecrypt(const QByteArray &str, const QByteArray &key)
{
    QByteArray bs = str;

    for(int i=0; i<bs.size(); i++){
        for(int j  = 0; j < key.size(); j++)
            bs[i] = bs[i] ^ key[j];
    }

    return bs;
}

void HelpClass::writeSettingFile(QMap<QString, QString> mapSettingData, QString groupName, QString filePath, bool isEncrypt)
{
    QSettings settings(filePath, QSettings::IniFormat);

    QStringList mapList = mapSettingData.keys();

    settings.beginGroup(groupName);
    for(QString key : mapList)
    {
        QString value = mapSettingData.value(key);
        if(isEncrypt)
        {
            QByteArray valueData = GetEncrypt(value);
            settings.setValue(key, valueData);
        }
        else
        {
            settings.setValue(key, value);
        }
    }

    settings.endGroup();
}

QMap<QString, QString> HelpClass::readSettingFile(QString groupName, QString filePath, bool isDecrypt)
{
    QMap<QString, QString> map;
    QSettings settings(filePath,QSettings::IniFormat);

    settings.beginGroup(groupName);
    QStringList keyList = settings.allKeys();

    for(QString key :keyList)
    {
        QString value;
        if(isDecrypt)
        {
            QByteArray valueData = settings.value(key).toByteArray();
            valueData = GetDecrypt(valueData);

            value = QString(valueData);
        }
        else
        {
            value = settings.value(key).toString();
        }
        map.insert(key, value);
    }
    return map;
}

void HelpClass::ToolTipmessage(QWidget *parent, QString message, QPoint point)
{
    ToolTip tip(parent);

    //显示时间为1.5秒
    tip.setToolTipDelay(1500);

    QEventLoop loop;

    if(!point.isNull())
    {
        tip.showToolMessage(point, message);
    }
    else
    {
        int toolWidtg = tip.getToolTipWidth(message);
        tip.setAnimationPopupPosition(QPoint((parent->width() - toolWidtg)/2, -60), QPoint((parent->width()- toolWidtg)/2, 15));

        tip.showToolMessage(message);
    }


    connect(&tip, &ToolTip::sighide, &loop, &QEventLoop::quit);

    loop.exec();
}

int HelpClass::getFontWidth(QString text)
{
    QFontMetrics fontMetrics(QFont("Microsoft Yahei",12));
    return fontMetrics.width(text);
}

void HelpClass::showDialogText(QWidget *parent, QString showText, bool free)
{
    //释放label指针
    if(free)
    {
        if(m_label)
        {
            //如果有父对象走Qt释放如下
            if(m_label->parent())
            {
                m_label->deleteLater();
            }
            else
            {
                delete m_label;
            }
            m_label = nullptr;
        }
        return;
    }

    if(m_label == nullptr)
    {
        m_label = new QLabel(parent);
        int fontwidget = getFontWidth(showText)+10;

        int labelX = (parent->width() - fontwidget)/2;
        int labelY = (parent->height()/2)+10;

        m_label->setGeometry(labelX, labelY, fontwidget, 50);
        m_label->setStyleSheet("   background-color:#FFFFFF; \
                                  border-radius:2px 0px 0px 2px; \
                                  border-bottom:0px solid rgba(234,234,234,1); \
                                  font-size:15px; \
                                 font-family:Microsoft YaHei;  \
                                 color:#333333;");
        m_label->setText(showText);
        m_label->raise();
        m_label->show();
    }
}

void HelpClass::setGraphicsEffect(QWidget *parent)
{
    QNGraphicsEffect * bodyShadow = new QNGraphicsEffect(parent);
    bodyShadow->setBlurRadius(10.0);
    bodyShadow->setDistance(4.0);
    bodyShadow->setColor(QColor(0, 0, 0, 20));
    parent->setGraphicsEffect(bodyShadow);

}

void HelpClass::copyText(QString text)
{
    QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
    clipboard->setText(text);	 //设置剪贴板内容
}

//加载qss文件
QString HelpClass::loaderQSSFile(const QString & qssFileName)
{
    QString openfileName = qssFileName;
    if(!qssFileName.startsWith(":/"))
    {
        openfileName = QString(":/qss/%1.qss").arg(qssFileName);
    }
    QFile file(openfileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qCritical("QSS File '%s' does not open!", qUtf8Printable(qssFileName));
        return "";
    }

    QString styleSheet = QLatin1String(file.readAll());
    file.close();

    return styleSheet;
}

void HelpClass::setGlobalStyleSheet(const QString &qssFileName)
{
    QString qssFile = loaderQSSFile(qssFileName);
    qApp->setStyleSheet(qssFile);

    return;
}
