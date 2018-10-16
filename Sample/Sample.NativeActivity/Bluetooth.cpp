#include "Bluetooth.h"

////////////////////////////////////////////////////////////////////////////////
CBluetooth::CBluetooth(JavaVM* pVm) : m_pVm(pVm), m_env(NULL), m_classBta(NULL), m_fnGetDefaultAdapter(NULL), m_fnGetName(NULL), m_fnIsEnabled(NULL)
{
    if (pVm)
    {
        pVm->AttachCurrentThread(&m_env, NULL);
        if (m_env)
        {
            m_classBta = m_env->FindClass("android/bluetooth/BluetoothAdapter");
            if (m_classBta)
            {
                m_fnGetDefaultAdapter = m_env->GetStaticMethodID(m_classBta, "getDefaultAdapter", "()Landroid/bluetooth/BluetoothAdapter;");
                m_fnGetName = m_env->GetMethodID(m_classBta, "getName", "()Ljava/lang/String;");
                m_fnIsEnabled = m_env->GetMethodID(m_classBta, "isEnabled", "()Z");
            }
        }

    }
}

////////////////////////////////////////////////////////////////////////////////
CBluetooth::~CBluetooth()
{
    if (m_pVm)
    {
        if (m_env)
        {
            if (m_classBta)
            {
                m_env->DeleteLocalRef(m_classBta);
                m_classBta = NULL;
            }


            m_pVm->DetachCurrentThread();
            m_env = NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
bool CBluetooth::IsEnabled()
{
    bool bRet = false;
    if (m_fnIsEnabled)
    {
        jboolean jbRet = (jboolean)m_env->CallBooleanMethod(m_classBta, m_fnIsEnabled); //TODO: class loader is missing
        bRet = (bool)jbRet;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
std::string CBluetooth::GetAdapterName()
{
    std::string strRet;
    if (m_classBta && m_fnGetDefaultAdapter && m_fnGetName)
    {
        jobject objBta = (jobject)m_env->CallStaticObjectMethod(m_classBta, m_fnGetDefaultAdapter);
        if (objBta)
        {
            jstring strName = (jstring)m_env->CallObjectMethod(objBta, m_fnGetName);
            const char* pszName =m_env->GetStringUTFChars(strName, 0);
            strRet = pszName;

            m_env->ReleaseStringUTFChars(strName, pszName);
            m_env->DeleteLocalRef(strName);
            m_env->DeleteLocalRef(objBta);
        }
    }

    return strRet;
}
