#include "Recorder.h"
#include "JsonMgr.h"
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/Dynamic/Var.h>
#include <unistd.h>

Recorder* Recorder::m_pRecorder = NULL;
std::recursive_mutex Recorder::_mtx;

Recorder* Recorder::getInstance()
{
    std::lock_guard<std::recursive_mutex> lock(_mtx);
    if (NULL == m_pRecorder) {
        m_pRecorder = new Recorder();
    }
    return m_pRecorder;
}

void Recorder::releaseInstance()
{
    std::lock_guard<std::recursive_mutex> lock(_mtx);
    RELEASE(m_pRecorder);
}

Recorder::Recorder() : m_pJsonMgr(NULL)
{
    this->setJsonFilePath(JSON_FILE);
}

Recorder::~Recorder()
{
    if (NULL != m_pJsonMgr) {
        delete m_pJsonMgr;
        m_pJsonMgr = NULL;
    }
}

void Recorder::setJsonFilePath(const string& strJsonPath)
{
    dLog(__FUNCTION__);
    string path = JSON_FILE_TEMPLATE;  // using template file if path doesnot exist
    if (0 == access(strJsonPath.c_str(), F_OK)) {
        path = strJsonPath;
    }

    if (NULL != m_pJsonMgr) {
        delete m_pJsonMgr;
    }
    m_pJsonMgr = new JsonMgr(path);
    if (NULL == m_pJsonMgr) {
        eLog("null pointer - m_pJsonMgr");
    }
}

void Recorder::outputRecords(const string& file_path)
{
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->writeFile(file_path);
}

void Recorder::recVersion(const string& version)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("Version", version);
}

void Recorder::recRoadsideAssistanceClickTime(const string & btnType, const string& timeStamp)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("RoadsideAssistanceClickTime");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }

        JSON::Array arr;
        if (pObj->has(btnType)) {
            Dynamic::Var val = pObj->get(btnType);
            arr = *val.extract<JSON::Array::Ptr>();
            arr.add(timeStamp);
        }
        else {
            arr.add(timeStamp);
        }

        pObj->set(btnType, arr);
        m_pJsonMgr->setValue("RoadsideAssistanceClickTime", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recLastMode(int mode)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    if (Recorder::LM_NAVI == mode) {
        m_pJsonMgr->setValue("NavigationLastMode", true);
    } else {
        m_pJsonMgr->setValue("NavigationLastMode", false);
        m_pJsonMgr->setValue("MediaLastMode", toString((_LAST_MODE)mode));
    }
}

void Recorder::recAppSelectedTimes(const string& appName)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("ClickCount");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        int nTimes = 1;
        if (pObj->has(appName)) {
            nTimes = pObj->getValue<int>(appName) + 1;
        }
        pObj->set(appName, nTimes);

        m_pJsonMgr->setValue("ClickCount", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recWiFiStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("QuickSettingsToggle");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("Wi-Fi", bEnabled);
        m_pJsonMgr->setValue("QuickSettingsToggle",
                             Dynamic::Var(*pObj));  // val ?= Dynamic::Var(*pObj), using Dynamic::Var(*pObj) will lead to crash
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recBtStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("QuickSettingsToggle");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();  // if using Dynamic::Var(*pObj) instead of val in line 121, it
                                                      // will crash here
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("Bluetooth", bEnabled);
        m_pJsonMgr->setValue("QuickSettingsToggle", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recMuteStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("QuickSettingsToggle");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("Mute", bEnabled);
        m_pJsonMgr->setValue("QuickSettingsToggle", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::rec4GStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("QuickSettingsToggle");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("4G", bEnabled);
        m_pJsonMgr->setValue("QuickSettingsToggle", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recOffModeStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("QuickSettingsToggle");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("OffMode", bEnabled);
        m_pJsonMgr->setValue("QuickSettingsToggle", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recVolume(int type, int volume)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("SoundVolumeMixer");
    try {
        // dLog(val.toString());
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set(toString((_VOLUME_TYPE)type), volume);
        // dLog(val.toString());

        m_pJsonMgr->setValue("SoundVolumeMixer", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recFader(int value)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("SoundAudioControl");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("Fader", value);

        m_pJsonMgr->setValue("SoundAudioControl", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recBalance(int value)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("SoundAudioControl");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("Balance", value);

        m_pJsonMgr->setValue("SoundAudioControl", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recBassBoostStatus(int mode)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SoundAudioControlBassBoost", toString((_BASS_BOOST)mode));
}

void Recorder::recEqualizer(int mode)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SoundEqualizer", toString((_EQ_MODE)mode));
}

void Recorder::recLoudnessStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SoundLoudness", bEnabled);
}

void Recorder::recHKeyBeepStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SoundHardKeySounds", bEnabled);
}

void Recorder::recTouchBeepStatusu(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SoundTouchSounds", bEnabled);
}

void Recorder::recDisplayMode(int mode)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("DisplayMode", toString((_DISPLAY_MODE)mode));
}

void Recorder::recDisplayBrightness(int value)  // 0 ~ 100%
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("DisplayBrightness", value);

    /* 设置状态类快捷标志 */
    Dynamic::Var val = m_pJsonMgr->getValue("QuickSettingsToggle");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set("BrightNess", value);
        m_pJsonMgr->setValue("QuickSettingsToggle", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }

}

void Recorder::recSDVC(int value)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SDVC", value);
}

void Recorder::recAudioLowering(int value)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("AudioLowering", value);
}

void Recorder::recVoiceLanguage(int langId)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("VoiceLanguage", toString((_LANGUAGE_ID)langId));
}

void Recorder::recTextLanguage(int langId)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("TextLanguage", toString((_LANGUAGE_ID)langId));
}

void Recorder::recInputMethod(int option, bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("KeyboardLanguage");
    try {
        JSON::Object::Ptr pObj;
        if (!val.isEmpty()) {
            pObj = val.extract<JSON::Object::Ptr>();
        } else {
            pObj = new JSON::Object();
        }
        pObj->set(toString((_IM_OPTION)option), bEnabled);

        m_pJsonMgr->setValue("KeyboardLanguage", Dynamic::Var(*pObj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recHandWriteLanguage(int langId)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("KeyboardHandwritingLanguage", toString((_LANGUAGE_ID)langId));
}

void Recorder::recHandWriteAutoComposeTime(int second)  // 0~5, 0: never; 1~5: 1~5s
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("KeyboardHandwritingAutoCompose", second);
}

void Recorder::recGpsSyncStatus(bool bEnabled)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("AutomaticTime", bEnabled);
}

void Recorder::recAdjustedTime(const string& time)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("SetTime", time);
}

void Recorder::recTimeFormat(int format)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("TimeFormat", toString((_TIME_FORMAT)format));
}

void Recorder::rerDistanceUnit(int unit)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("Units", toString((_DISTANCE_UNIT)unit));
}

void Recorder::recCoordinateFormat(int format)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("CoordinateFormat", toString((_COORDINATE_FORMAT)format));
}

void Recorder::recGeodeticDatum(int type)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("Datum", toString((_GEODETIC_DATUM)type));
}

void Recorder::recWidgetStatus(_WIDGET_ALLTYPE type, const widget_stat_t& state)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    // key
    string widget_name = toString((_WIDGET_ALLTYPE)type);
    // value
    JSON::Object objWidget;
    objWidget.set("screen", state.screen);
    objWidget.set("x", state.x);
    objWidget.set("y", state.y);

    Dynamic::Var val = m_pJsonMgr->getValue("Widgets");
    try {
        JSON::Object Obj;
        if (!val.isEmpty()) {
            Obj = *(val.extract<JSON::Object::Ptr>());
        }

        JSON::Array arr;
        if (Obj.has(widget_name)) {
            arr = *(Obj.get(widget_name).extract<JSON::Array::Ptr>());
        }
        arr.add(objWidget);
        Obj.set(widget_name, Dynamic::Var(arr));

        m_pJsonMgr->setValue("Widgets", Dynamic::Var(Obj));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recDid(const string & strDid)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("did", strDid);
}

void Recorder::recDevice(const string & strDev)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);
    m_pJsonMgr->setValue("Device", strDev);
}

void Recorder::recParkingClickInfo(const string & time, int lat, int lon)
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    Dynamic::Var val = m_pJsonMgr->getValue("ParkingClickInfo");
    try {
        JSON::Array arr;
        if (!val.isEmpty()) {
            arr = *val.extract<JSON::Array::Ptr>();
        }

        JSON::Object objPark;
        objPark.set("time", time);
        objPark.set("lat", lat);
        objPark.set("lon", lon);

        arr.add(objPark);
        m_pJsonMgr->setValue("ParkingClickInfo", Dynamic::Var(arr));
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recReset()
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    // reset ParkingClickInfo
    Dynamic::Var val = m_pJsonMgr->getValue("ParkingClickInfo");
    try {
        if (!val.isEmpty()) {
            JSON::Array arr;
            m_pJsonMgr->setValue("ParkingClickInfo", Dynamic::Var(arr));
        }
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }

    // reset RoadsideAssistanceClickTime
    val = m_pJsonMgr->getValue("RoadsideAssistanceClickTime");
    try {
        if (!val.isEmpty()) {
            JSON::Object obj;
            m_pJsonMgr->setValue("RoadsideAssistanceClickTime", Dynamic::Var(obj));
        }
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }

    // reset ClickCount
    val = m_pJsonMgr->getValue("ClickCount");
    try {
        if (!val.isEmpty()) {
            JSON::Object obj;
            m_pJsonMgr->setValue("ClickCount", Dynamic::Var(obj));
        }
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

void Recorder::recResetWidget()
{
    dLog(__FUNCTION__);
    NULL_CHECK(m_pJsonMgr);

    // reset Widgets
    Dynamic::Var val = m_pJsonMgr->getValue("Widgets");
    try {
        if (!val.isEmpty()) {
            JSON::Object obj;
            m_pJsonMgr->setValue("Widgets", Dynamic::Var(obj));
        }
    } catch (JSON::JSONException& exc) {
        eLog(exc.displayText());
    }
}

string Recorder::GetJsonString()
{
    if (!m_pJsonMgr) {
        return "";
    }

    return string(m_pJsonMgr->getBuffer());
}


