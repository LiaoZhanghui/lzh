#ifndef __RECORDER_H__
#define __RECORDER_H__

#include <Poco/Dynamic/Var.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>

using std::map;
using std::string;
using std::vector;

#define RELEASE(p) \
    if (p) {       \
        delete p;  \
        p = NULL;  \
    }
#define NULL_CHECK(p) \
    if (!p) {         \
        eLog("null pointer: "#p); \
        return;       \
    }
#ifdef PC_TEST
#define JSON_FILE "g5r2.data.json"
#define JSON_FILE_TEMPLATE "g5r2.data.json"
#else
#define JSON_FILE "/storage/data/data/log/g5r2.data.json"
#define JSON_FILE_TEMPLATE "/svp/etc/g5r2.data.json"
#endif

class JsonMgr;
class Recorder
{
public:
    enum _LAST_MODE
    {
        LM_UNKNOWN = 0,
        LM_TUNER,
        LM_USB_AUDIO,
        LM_USB_PICTURE,
        LM_USB_VIDEO,
        LM_IPOD,
        LM_BT,
        LM_RSE,
        LM_HDTV,
        LM_NAVI,
        LM_WIDGET
    };

    static inline const string toString(_LAST_MODE mode)
    {
        string ret = "unknown";
        switch (mode) {
        case LM_TUNER:       ret = "Tuner";       break;
        case LM_USB_AUDIO:   ret = "USB Audio";   break;
        case LM_USB_PICTURE: ret = "USB Picture"; break;
        case LM_USB_VIDEO:   ret = "USB Video";   break;
        case LM_IPOD:        ret = "iPod";        break;
        case LM_BT:          ret = "Bluetooth";   break;
        case LM_RSE:         ret = "RSE";         break;
        case LM_HDTV:        ret = "HDTV";        break;
        case LM_NAVI:        ret = "Navigation";  break;
        case LM_WIDGET:      ret = "Widget";      break;
        default: break;
        }
        return ret;
    }

    enum _VOLUME_TYPE
    {
        VT_UNKNOWN = 0,
        VT_PHONE,
        VT_NAVIANDVR,
        VT_MEDIA,
        VT_APPNAVI,
        VT_APPVR
    };

    static inline const string toString(_VOLUME_TYPE type)
    {
        string ret = "unknown";
        switch (type) {
        case VT_PHONE:          ret = "Phone";              break;
        case VT_NAVIANDVR:      ret = "NavigationAndVR";    break;
        case VT_MEDIA:          ret = "Media";              break;
        case VT_APPNAVI:        ret = "APPNavigation";      break;
        case VT_APPVR:          ret = "APPVR";              break;
        default: break;
        }
        return ret;
    }

    enum _BASS_BOOST
    {
        BB_OFF = 0,
        BB_STEP_1,
        BB_STEP_2
    };

    static inline const string toString(_BASS_BOOST mode)
    {
        string ret = "OFF";
        switch (mode) {
        case BB_STEP_1: ret = "Step 1"; break;
        case BB_STEP_2: ret = "Step 2"; break;
        default: break;
        }
        return ret;
    }

    enum _EQ_MODE
    {
        EM_UNKNOWN = 0,
        EM_DEFINE,
        EM_ROCK,
        EM_POP,
        EM_CLASSIC,
        EM_JAZZ,
        EM_SPEECH
    };

    static inline const string toString(_EQ_MODE mode)
    {
        string ret = "unknown";
        switch (mode) {
        case EM_DEFINE:     ret = "Define";     break;
        case EM_ROCK:       ret = "Rock";       break;
        case EM_POP:        ret = "Pop";        break;
        case EM_CLASSIC:    ret = "Classic";    break;
        case EM_JAZZ:       ret = "Jazz";       break;
        case EM_SPEECH:     ret = "Speech";     break;
        default: break;
        }
        return ret;
    }

    enum _DISPLAY_MODE
    {
        DM_AUTO = 0,
        DM_DAY,
        DM_NIGHT
    };

    static inline const string toString(_DISPLAY_MODE mode)
    {
        string ret = "Auto";
        switch (mode) {
        case DM_AUTO:  ret = "Auto";  break;
        case DM_DAY:   ret = "Day";   break;
        case DM_NIGHT: ret = "Night"; break;
        default: break;
        }
        return ret;
    }

    enum _LANGUAGE_ID
    {
        LID_EN_US = 0,
        LID_ZH_CN,
        LID_ZH_TW,
        LID_ID_ID,
        LID_MS_MY
    };

    static inline const string toString(_LANGUAGE_ID id)
    {
        string ret = "unknown";
        switch (id) {
        case LID_EN_US: ret = "American English";    break;
        case LID_ZH_CN: ret = "Simplified Chinese";  break;
        case LID_ZH_TW: ret = "Traditional Chinese"; break;
        case LID_ID_ID: ret = "Bahasa Indonesia";    break;
        case LID_MS_MY: ret = "Malay";               break;
        default: break;
        }
        return ret;
    }

    enum _IM_OPTION
    {
        IO_EN_US = 0,
        IO_ZH_HANDWRITING,
        IO_ZH_STROKE,
        IO_ZH_ZHUYIN
    };

    static inline const string toString(_IM_OPTION opt)
    {
        string ret = "unknown";
        switch (opt) {
        case IO_EN_US:          ret = "American English";    break;
        case IO_ZH_HANDWRITING: ret = "Chinese Handwriting"; break;
        case IO_ZH_STROKE:      ret = "Chinese Stroke";      break;
        case IO_ZH_ZHUYIN:      ret = "Chinese Zhuyin";      break;
        default: break;
        }
        return ret;
    }

    enum _TIME_FORMAT
    {
        TF_12 = 0,
        TF_24
    };

    static inline const string toString(_TIME_FORMAT id)
    {
        string ret = "24-Hour";
        switch (id) {
        case TF_12: ret = "12-Hour"; break;
        case TF_24: ret = "24-Hour"; break;
        default: break;
        }
        return ret;
    }

    enum _DISTANCE_UNIT
    {
        DU_MILES = 0,
        DU_KILOMETERS
    };

    static inline const string toString(_DISTANCE_UNIT id)
    {
        string ret = "Miles";
        switch (id) {
        case DU_MILES:      ret = "Miles";      break;
        case DU_KILOMETERS: ret = "Kilometers"; break;
        default: break;
        }
        return ret;
    }

    enum _COORDINATE_FORMAT
    {
        CF_DEGREE,               // h ddd.ddddd°
        CF_DEGREE_MINUTE,        // h ddd° mm.mmm'
        CF_DEGREE_MINUTE_SECOND  // h ddd° mm' ss.s"
    };

    static inline const string toString(_COORDINATE_FORMAT format)
    {
        string ret = "unknown";
        switch (format) {
        case CF_DEGREE:               ret = "h ddd.ddddd°";      break;
        case CF_DEGREE_MINUTE:        ret = "h ddd° mm.mmm'";    break;
        case CF_DEGREE_MINUTE_SECOND: ret = "h ddd° mm' ss.s"""; break;
        default: break;
        }
        return ret;
    }

    enum _GEODETIC_DATUM
    {
        GD_WGS_84 = 0,
        GD_EUROPEAN_1950,
        GD_FINLAND_HAYFRD,
        GD_ORDNANCE_SURVEY_GRID,
        GD_ROME_1940
    };

    static inline const string toString(_GEODETIC_DATUM id)
    {
        string ret = "WGS-84";
        switch (id) {
        case GD_EUROPEAN_1950:        ret = "European 1950";        break;
        case GD_FINLAND_HAYFRD:       ret = "Finland Hayfrd";       break;
        case GD_ORDNANCE_SURVEY_GRID: ret = "Ordnance Survey Grid"; break;
        case GD_ROME_1940:            ret = "Rome 1940";            break;
        default: break;
        }
        return ret;
    }

    enum _WIDGET_ALLTYPE : int {
        /*widget 1*1 TYPE enum declaration*/
        Widget_1x1 = 0,
        Preset_1x1,
        Clock_1x1,
        Mute_1x1,
        BT_1x1,
        WiFi_1x1,
        OffLine_1x1,
        Widget4G_1x1,
        Equlizer_1x1,
        WhereTo_1x1,
        GasStation_1x1,
        GoHome_1x1,
        RecentPOI_1x1,
        SavedPOI_1x1,
        StopNavi_1x1,
        ScreenOff_1x1,
        CarPlay_1x1,
        AndroidAuto_1x1,
        AndroidLink_1x1,
        VoiceCmd_1x1,
        VehicleSecretary_1x1,
        HightwayTraffic_1x1,
        Tutorial_1x1,
        InfoCenter_1x1,

        /*widget 1*2 TYPE enum declaration*/
        Widget_1x2 = 30,
        OilPriceMsg_1x2,

        /*widget 2*1 TYPE enum declaration*/
        Widget_2x1 = 40,
        Tuner_2x1,
        Media_2x1,
        Preset01_2x1,
        Preset02_2x1,
        RecentCall_2x1,
        Clock_2x1,
        Navi_2x1,
        DigitalClock_2x1,

        /*widget 2*2 TYPE enum declaration*/
        Widget_2x2 = 50,
        Tuner_2x2,
        Media_2x2,
        RecentCall_2x2,
        Clock_2x2,
        Navi_2x2,
        WhereTo_2x2,
        WeatherInfo_2x2,
    };

    static inline const string toString(_WIDGET_ALLTYPE id)
    {
        string ret = "";
        switch (id) {
        case Widget_1x1:            ret = "Widget_1x1";             break;
        case Preset_1x1:            ret = "ContactPreset_1x1";      break;
        case Clock_1x1:             ret = "Analog_Clock_1x1";       break;
        case Mute_1x1:              ret = "Mute_1x1";               break;
        case BT_1x1:                ret = "Bluetooth_1x1";          break;
        case WiFi_1x1:              ret = "WiFi_1x1";               break;
        case OffLine_1x1:           ret = "Offline_Mode_1x1";       break;
        case Widget4G_1x1:          ret = "4G_Settings_1x1";        break;
        case Equlizer_1x1:          ret = "Equalizer_Settings_1x1"; break;
        case WhereTo_1x1:           ret = "Where_To_1x1";           break;
        case GasStation_1x1:        ret = "Gas_Station_1x1";        break;
        case GoHome_1x1:            ret = "Go_Home_1x1";            break;
        case RecentPOI_1x1:         ret = "Recent_POI_1x1";         break;
        case SavedPOI_1x1:          ret = "Saved_POI_1x1";          break;
        case StopNavi_1x1:          ret = "Stop_Navigation_1x1";    break;
        case ScreenOff_1x1:         ret = "Screen_Off_Setting_1x1"; break;
        case CarPlay_1x1:           ret = "CarPlay";                break;
        case AndroidAuto_1x1:       ret = "AndroidAuto";            break;
        case AndroidLink_1x1:       ret = "AndroidLink_1x1";        break;
        case VoiceCmd_1x1:          ret = "Voice_Command";          break;
        case VehicleSecretary_1x1:  ret = "VehicleSecretary_1x1";   break;
        case HightwayTraffic_1x1:   ret = "Freeway_Traffic";        break;
        case Tutorial_1x1:          ret = "Tutorial";               break;
        case InfoCenter_1x1:        ret = "Message_Center";         break;
        case Widget_1x2:            ret = "Widget_1x2";             break;
        case OilPriceMsg_1x2:       ret = "Fuel_Price_1x2";         break;
        case Widget_2x1:            ret = "Widget_2x1";             break;
        case Tuner_2x1:             ret = "Media_Widget_2x1";       break;
        case Media_2x1:             ret = "Media_Widget_2x1";       break;
        case Preset01_2x1:          ret = "ContactPreset_2x1";      break;
        case Preset02_2x1:          ret = "ContactPreset_2x1";      break;
        case RecentCall_2x1:        ret = "RecentCalls_2x1";        break;
        case Clock_2x1:             ret = "Analog_clock_2x1";       break;
        case Navi_2x1:              ret = "Navigation_Widget_2x1";  break;
        case DigitalClock_2x1:      ret = "DigitalClock_2x1,";      break;
        case Widget_2x2:            ret = "Widget_2x2";             break;
        case Tuner_2x2:             ret = "Media_Widget_2x2";       break;
        case Media_2x2:             ret = "Media_Widget_2x2";       break;
        case RecentCall_2x2:        ret = "RecentCalls_2x2";        break;
        case Clock_2x2:             ret = "Analog_Clock_2x2";       break;
        case Navi_2x2:              ret = "Navigation_Widget_2x2";  break;
        case WhereTo_2x2:           ret = "Where_To_2x2";           break;
        case WeatherInfo_2x2:       ret = "Weather_2x2";            break;
        default: break;
        }
        return ret;
    }

    typedef struct _WIDGET_STATE
    {
        int screen;
        int x;
        int y;
    } widget_stat_t;

private:
    Recorder();
    virtual ~Recorder();

public:
    static Recorder* getInstance();
    static void releaseInstance();

    /**
     * @brief allow user specify the Json File Path, should be called before recording
     *
     * @param strJsonPath
     */
    void setJsonFilePath(const string& strJsonPath);
    void outputRecords(const string& file_path = JSON_FILE);

    /**
     * @brief record the json file version
     *
     * @param version, e.g., 1.1
     */
    void recVersion(const string& version);

    /**
     * @brief record the time for each roadside assistance request
     * @param btnType e.g., RAButtonClick/AgreeButtonClick/BluetoothSettingsClick/CancelClick/DialClick
     * @param timeStamp e.g., 20180101100000
     */
    void recRoadsideAssistanceClickTime(const string & btnType, const string& timeStamp);

    /**
     * @brief record the last mode of media, navi, widget
     *
     * @param mode - 10 types in total
     */
    void recLastMode(int mode);

    /**
     * @brief record the click times of each app
     *
     * @param appName
     */
    void recAppSelectedTimes(const string& appName);

    /**
     * @brief quick setting toggles
     *
     * @param bEnabled
     */
    void recWiFiStatus(bool bEnabled);

    /**
    * @brief quick setting toggles
    *
    * @param bEnabled
    */
    void recBtStatus(bool bEnabled);

    /**
    * @brief quick setting toggles
    *
    * @param bEnabled
    */
    void recMuteStatus(bool bEnabled);

    /**
    * @brief quick setting toggles
    *
    * @param bEnabled
    */
    void rec4GStatus(bool bEnabled);

    /**
    * @brief quick setting toggles
    *
    * @param bEnabled
    */
    void recOffModeStatus(bool bEnabled);

    /**
     * @brief record the volume of each source
     *
     * @param type - 1-media; 2-navi-asr; 3-phone; 4-notification; 5-touch-feedback
     * @param volume
     */
    void recVolume(int type, int volume);

    /**
     * @brief record the fader and balance
     *
     * @param value
     */
    void recFader(int value);
    void recBalance(int value);

    /**
     * @brief record the bass boost status
     *
     * @param mode - 0-off; 1-step_1; 2-step_2
     */
    void recBassBoostStatus(int mode);

    /**
     * @brief eq mode
     *
     * @param mode - 0-default; 1-classic; 2-electronic; 3-hippop; 4-jazz; 5-pop; 6-rock; 7-customized
     */
    void recEqualizer(int mode);

    /**
     * @brief loudness
     *
     * @param bEnabled
     */
    void recLoudnessStatus(bool bEnabled);
    void recHKeyBeepStatus(bool bEnabled);
    void recTouchBeepStatusu(bool bEnabled);

    /**
     * @brief day/night mode
     *
     * @param mode - 0-auto; 1-day; 2-night
     */
    void recDisplayMode(int mode);

    // 0 ~ 100%
    void recDisplayBrightness(int value);

    /**
     * @brief The volume with the speed adjustment
     *
     * @param value - 0 ~ 7
     */
    void recSDVC(int value);

    /**
     * @brief Entertainment System Volume suppression
     *
     * @param value - 1 ~ 3
     */
    void recAudioLowering(int value);

    /**
     * @brief language setting
     *
     * @param langId - 0-en_US; 1-zh_CN; 2-zh_TW; 3-id_ID; 4-ms_MY
     */
    void recVoiceLanguage(int langId);
    void recTextLanguage(int langId);

    /**
     * @brief
     *
     * @param option - en_US, handwriting, chinese stroke, chinese zhuyin
     * @param bEnabled
     */
    void recInputMethod(int option, bool bEnabled);
    void recHandWriteLanguage(int langId);
    void recHandWriteAutoComposeTime(int second);  // how log does it take to generate characters after hands off the touch pad, 0~5, 0: never; 1~5: 1~5s
    void recGpsSyncStatus(bool bEnabled);          // ?= automatic time
    void recAdjustedTime(const string& time);

    /**
     * @brief time format setting
     *
     * @param format - 0-12h; 1-24h
     */
    void recTimeFormat(int format);

    /**
     * @brief distance unit setting
     *
     * @param unit - 0-miles; 1-km
     */
    void rerDistanceUnit(int unit);

    /**
     * @brief how to display the coordinates
     *
     * @param format - 0-degree; 1-degree and minute; 2-degree, minute and second
     */
    void recCoordinateFormat(int format);

    /**
     * @brief
     *
     * @param type - 0-WGS-84; 1-European-1950; 2-Finland Hayfrd; 3-Ordance Survey grid; 4-Rome-1940
     */
    void recGeodeticDatum(int type);

    /**
     * @brief
     *
     * @param type - refer to g_vecWidget
     * @param state - includes screen, x, y
     */
    void recWidgetStatus(_WIDGET_ALLTYPE type, const widget_stat_t& state);  // FIXME: crash

    // Device id,MAC address, example: 104E89F36842
    void recDid(const string & strDid);

    // string, Device name. exapmle: TR6399-06-DA,TR6399-06-EB
    void recDevice(const string & strDev);

    void recParkingClickInfo(const string &time, int lat, int lon);

    void recReset();
    void recResetWidget();
    string GetJsonString();

private:
    static Recorder* m_pRecorder;
    JsonMgr* m_pJsonMgr;
    static std::recursive_mutex _mtx;
};

#endif  // !__RECORDER_H__
