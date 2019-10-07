/***********************************BEGIN***************************************
 **                      CONFIDENTIAL AND PROPRIETARY
 **         Copyright (C) 2016 Coriant, All Rights Reserved
 *******************************************************************************
 *
 * FILE:        PmcG4SdkApi.h
 * SUBSYSTEM:   Common driver
 * TARGET:      Coriant OS
 * DESCRIPTION:
 * @author:
 * @Version:    1.0
 * @Since:      1.0
 *******************************************************************************/
#include <iostream>
#include <bitset>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

#include <boost/thread>

typedef char uint8;
typedef unsigned int uint32;

const int maxNumTraceItems = 32;

class SdkTrace {
public:
    SdkTrace(unsigned int traceItems, unsigned int logLevels, std::string sdkName) :
        myNumTraceItems(traceItems),
        myNumLogLevels(logLevels),
        mySdkName(sdkName)
    {
        std::cout << "SdkTrace()" <<  std::endl;
    }

    SdkTrace(const SdkTrace& obj)
    {
        (void(obj));
    }

    virtual ~SdkTrace()
    {
        std::cout << "~SdkTrace()" << std::endl;
    }

    bool resetALL()
    {
        myTraceWord.reset();
    }

    virtual void sdkLogLevelSet(unsigned int logLevel)
    {
        if(logLevel < myNumLogLevels)

        {
            myLogLevel = logLevel;
        } else {

        }
    }

    virtual unsigned int sdkLogLevelGet()
    {
        return myLogLevel;
    }

    virtual void traceEnableSet(unsigned int item, bool enable)
    {
        if(item < myNumTraceItems) {
            enable ? myTraceWord.set(item) : myTraceWord.reset(item);
        } else {
            std::cout << "Invalid TraceItem" << std::endl;
            //FC_THROW_ERROR(FC_ValueOutOfRangeError, "Invalid TraceItem");
        }
    }

    virtual bool traceEnableGet(unsigned int item)
    {
        if(item < myNumTraceItems) {
            return myTraceWord.test(item);
        } else {
            std::cout << "Invalid TraceItem" << std::endl;
        }
    }

    virtual void tracePrint(unsigned int item, const char *format, ...)
    {
        char gOutputBuffer[255] = {0};
        if(item < myNumTraceItems) {
            va_list ap;
            va_start(ap, format);
            vsprintf(gOutputBuffer, format, ap);
            va_end(ap);
            std::cout << gOutputBuffer << std::endl;
        } else {
            //FC_THROW_ERROR(FC_ValueOutOfRangeError, "Invalid TraceItem");
        }
    }

    virtual void printTraceWord()
    {
        std::cout << myTraceWord << std::endl;
    }

    virtual void mySdkMutexEnter()
    {
        //pthread_mutex_lock(&mySdkLock);
    }

    virtual void mySdkMutexExit()
    {
        //pthread_mutex_unlock(&mySdkLock);
    }

private:
    std::string  mySdkName;
    std::bitset<maxNumTraceItems> myTraceWord;
    unsigned int myLogLevel;
    unsigned int myNumLogLevels;
    unsigned int myNumTraceItems;

    pthread_mutex_t mySdkLock;
};


namespace Devices {
namespace Pm5990 {
namespace PmcLogSev {
enum Enum {
    PMC_LOG_SEV_DISABLE = 0,
    PMC_LOG_SEV_HIGHEST,
    PMC_LOG_SEV_FATAL,
    PMC_LOG_SEV_HIGH,
    PMC_LOG_SEV_MEDIUM,
    PMC_LOG_SEV_LOW,
    PMC_LOG_SEV_LOWEST,
    PMC_LOG_SEV_NUM
};
}

namespace TraceItem {
enum Enum {
    PORT,
    ODUXC,
    CPB,
    MUX,
    SDK_PARAM,
    AMPM,
    AMPM_CFG,
    AMPM_GET,
    AMPM_TRG,
    AMPM_SET,
    INT_AMPM_SET,
    SDK_TRAFFIC_ERR,
    SDK_AMPM_ERR,
    SDK_AMPM_SET_ERR,
    SAVE_BLOCK,
    INT,
    GCC,
    TCM,
    TCMCFG,
    CYCLE_BLOCK,
    SDK_BLK,
    SDK_BLK_NO_DEVID,
    SDK_BLK_AMPM,
    SDK_BLK_AMPM_CFG,
    SDK_BLK_AMPM_GET,
    SDK_BLK_AMPM_TRG,
    SDK_BLK_AMPM_SET,
    SDK_BLK_INT_AMPM_SET,
    SDK_BLK_TCM,
    SDK_PMON,
    ALL,
    UNKNOWN
};
}

pthread_mutex_t trace_lock;

class Pm5990SdkTrace : public SdkTrace {
private:
    Pm5990SdkTrace() :
        SdkTrace(TraceItem::ALL, PmcLogSev::PMC_LOG_SEV_NUM, "Pm5990Sdk")
    {
        std::cout << "Pm5990SdkTrace()" <<  std::endl;
        pthread_mutex_init(&trace_lock, NULL);
    }

    Pm5990SdkTrace& operator=(Pm5990SdkTrace const&) { }
    Pm5990SdkTrace(const Pm5990SdkTrace& obj) :
        SdkTrace(obj)
    { }

private:
    static Pm5990SdkTrace *m_pInstance;

public:
    virtual ~Pm5990SdkTrace()
    {
        std::cout << "~Pm5990SdkTrace()" << std::endl;
        m_pInstance = NULL;
    }

    static Pm5990SdkTrace *getInstance()
    {
        std::cout << "getInstance()" << std::endl;
        if(m_pInstance == NULL) {
            std::cout << "m_pInstance == NULL first time" << std::endl;
            pthread_mutex_lock(&trace_lock);
            std::cout << "locked." << std::endl;
            if(m_pInstance == NULL) {
                std::cout << "m_pInstance == NULL second time" << std::endl;
                m_pInstance = new Pm5990SdkTrace();
            }
            std::cout << "ready unlock..." << std::endl;
            pthread_mutex_unlock(&trace_lock);
        }
        return m_pInstance;
    }

    void traceEnableSet(unsigned int item, bool enable)
    {
        std::cout << "Pm5990SdkTrace.traceEnableSet()" << std::endl;
        if(item == TraceItem::ALL) {
            if(enable == true) {
                SdkTrace::traceEnableSet(TraceItem::CYCLE_BLOCK, true);
                SdkTrace::traceEnableSet(TraceItem::SAVE_BLOCK, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_PMON, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_NO_DEVID, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_AMPM, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_AMPM_SET, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_AMPM_CFG, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_AMPM_GET, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_AMPM_TRG, true);
                SdkTrace::traceEnableSet(TraceItem::SDK_BLK_INT_AMPM_SET, true);
            } else {
                resetALL();
            }
        } else {
            SdkTrace::traceEnableSet(item, true);
        }
    }

    void printTraceWord()
    {
        SdkTrace::printTraceWord();
    }

    bool traceEnableGet(unsigned int item)
    {
        return SdkTrace::traceEnableGet(item);
    }

};

Pm5990SdkTrace *Pm5990SdkTrace::m_pInstance = NULL;
}
}


using namespace Devices;
using namespace Pm5990;

Pm5990SdkTrace *myInstance = NULL;

void *thread_func0(void * )
{
    std::cout << "thread1" << std::endl;
    myInstance = Pm5990SdkTrace::getInstance();
    myInstance->traceEnableSet(0, true);
}

void *thread_func1(void *)
{
    std::cout << "thread2" << std::endl;
    myInstance = Pm5990SdkTrace::getInstance();
    myInstance->traceEnableSet(1, true);
}

int main()
{
    pthread_t thread1, thread2;
    {
        pthread_create(&thread1, NULL, &thread_func0, NULL);
        pthread_create(&thread2, NULL, &thread_func1, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        myInstance->printTraceWord();
    }

    return 0;
}

