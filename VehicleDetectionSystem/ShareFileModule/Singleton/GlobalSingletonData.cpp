#include "GlobalSingletonData.h"

GlobalSingletonData::GlobalSingletonData(QObject *parent) : QObject(parent)
{

}

GlobalSingletonData::~GlobalSingletonData()
{
        qDebug("我被析构了~");
}

void GlobalSingletonData::setEid(QString eid)
{
    m_eid = eid;
}

QString GlobalSingletonData::getEid()
{
    return m_eid;
}

void GlobalSingletonData::setAgentid(QString agentid)
{
    m_agentid = agentid;
}

QString GlobalSingletonData::getAgentid()
{
    return m_agentid;
}

void GlobalSingletonData::setSecret(QString secret)
{
    m_secret = secret;
}

QString GlobalSingletonData::getSecret()
{
    return m_secret;
}
