#ifndef GLOBALSINGLETONDATA_H
#define GLOBALSINGLETONDATA_H

/**
* @brief: 存储公共数据，全局唯一
* @author: Fu_Lin
* @date:
* @description: 主要用于数据的读写操作，可全局共享
*/
#include <QObject>
#include "Singleton.h"

class GlobalSingletonData : public QObject
{
    Q_OBJECT
public:
    explicit GlobalSingletonData(QObject *parent = nullptr);

    ~GlobalSingletonData();
    //读写坐席id
    void setEid(QString eid);
    QString getEid();

    //读写客服id
    void setAgentid(QString agentid);
    QString getAgentid();

    //读写密钥secret,相当于token
    void setSecret(QString secret);
    QString getSecret();
signals:

public slots:
private:
    //以下是接口需要用到的数据
    QString m_eid;  //坐席id
    QString m_agentid; //客服id，登录账号
    QString m_secret; //相当于token
};

#endif // GLOBALSINGLETONDATA_H
