#pragma once
#include <string>

class CBluetooth
{
public:

    CBluetooth(JavaVM* pVm);

    ~CBluetooth();


    bool IsEnabled();

    std::string GetAdapterName();

protected:

    JavaVM* m_pVm;
    JNIEnv* m_env;
    jclass m_classBta;
    jmethodID m_fnGetDefaultAdapter;
    jmethodID m_fnGetName;
    jmethodID m_fnIsEnabled;

};

