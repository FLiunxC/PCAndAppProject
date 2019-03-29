#ifndef SINGLETON_H
#define SINGLETON_H
/**
* @brief: 单例模板类
* @author: Fu_Lin
* @date:2018年8月21日
* @description: 单例的用法为：先初始化，后面取值，如果中途销毁单例的话，需要重新取值。如果没有初始化就取值或者重复初始化则会抛出一个异常。
*                       两种用法，1、直接继承class::get方式，必须先init
*                                       2、通过Singleton<class>::get方式调用，必须先init
*/
#include <QString>
#include <QDebug>

template <typename T>
class Singleton
{
public:
    template<typename... Args>
    //初始化单例
    static T * Init(Args&&... args)
    {
        if(m_pInstance==nullptr)
        {
            m_pInstance = new T(std::forward<Args>(args)...);
        }
        else
        {
            const char * className = typeid(T).name();
            QString fatal = "Singleton qFatal:<<"+QString(QLatin1String(className))+">>the instance is repeat init";
            QByteArray fatalInfo = fatal.toLocal8Bit();
            qFatal(fatalInfo.data());
        }
        return m_pInstance;
    }
    //获取单例
    static T * Get()
    {
        if (m_pInstance == nullptr)
        {
            const char * className = typeid(T).name();
            QString fatal = "Singleton qFatal:<<"+QString(QLatin1String(className))+">>the instance is not init, please initialize the instance first";
               QByteArray fatalInfo = fatal.toLocal8Bit();
             qFatal(fatalInfo.data());
         //   throw std::logic_error("the instance is not init, please initialize the instance first");
        }

        return m_pInstance;
        }

    //析构new出来的单例
    static void DestroyInstance()
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }

private:
    Singleton(void);
    virtual ~Singleton(void){}
    Singleton(const Singleton&){}
    Singleton& operator = (const Singleton&){}
private:
    static T* m_pInstance;
};

template <class T> T*  Singleton<T>::m_pInstance = nullptr;
#endif // SINGLETON_H
