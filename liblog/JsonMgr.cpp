#include "JsonMgr.h"
#include "log.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/mman.h>
using std::ifstream;
using std::ios;
using std::ofstream;
using std::ostringstream;

#define RECORDER_MAP_BUF_SIZE (64 * 1024U)
extern stLogCfg g_cfg;

JsonMgr::JsonMgr(const string& strJsonFilePath) : m_strJsonPath(strJsonFilePath), m_pMapBuffer(NULL)
{
    if (!synchronizer_init()) {
        eLog("synchronizer_init failed");
    }

    JSON::ParseHandler::Ptr handle = new JSON::ParseHandler(true);
    if (NULL != handle) {
        m_jsonParser.setHandler(handle);

        if (0 == access(JSON_FILE_TEMP, F_OK)) {
            dLog("tmp file exist")
        }
        else {
            dLog(strJsonFilePath);
            copy_file(strJsonFilePath.c_str(), JSON_FILE_TEMP);
        }

        if (0 == access(JSON_FILE_TEMP, F_OK)) {
            m_pMapBuffer = (char*)shareFile(JSON_FILE_TEMP);
        }

        if (NULL == m_pMapBuffer) {
            eLog("failed in sharing recorder file");
        }
    } else {
        eLog("nullptr of parser handler")
    }
}

JsonMgr::~JsonMgr()
{
    if (NULL != m_pMapBuffer) {
        munmap(m_pMapBuffer, RECORDER_MAP_BUF_SIZE);
        m_pMapBuffer = NULL;
    }
}

void JsonMgr::loadFile(const string& strPath)
{
    // no need to call this interface any more, leave it here for compatibility with old version
}

void* JsonMgr::shareFile(const string& strPath)
{
    dLog(__FUNCTION__);
    ifstream ifs(strPath.c_str(), ios::in);
    if (!ifs.is_open()) {
        eLog("failed to load file");
        return NULL;
    }

    ifs.close();
    ifs.clear();

    return map_existing_file(strPath.c_str(), 0, RECORDER_MAP_BUF_SIZE);
}

bool JsonMgr::writeFile(const string& strPath)
{
    dLog(__FUNCTION__);
    if (NULL == m_pMapBuffer || 0 == strlen(m_pMapBuffer)) return false;

    ofstream ofs;
    ofs.open(strPath.c_str(), ios::trunc);
    if (!ofs.is_open()) {
        eLog("failed to open file");
        return false;
    }

    try {
        lock(&g_cfg._meta_data->_lock);
        Dynamic::Var js            = m_jsonParser.parse(m_pMapBuffer);
        JSON::Object::Ptr pJsonObj = js.extract<JSON::Object::Ptr>();
        pJsonObj->stringify(ofs, 2, 2);
        unlock(&g_cfg._meta_data->_lock);
    } catch (JSON::JSONException& exc) {
        unlock(&g_cfg._meta_data->_lock);
        eLog(exc.displayText());
        ofs.close();
        return false;
    }

    if (!ofs.good()) {
        eLog("failed to write file");
        ofs.close();
        return false;
    }
    ofs.close();

    system("sync");
    return true;
}

bool JsonMgr::isKeyExist(const string& key)
{
    bool bRet = false;
    if (NULL == m_pMapBuffer || 0 == strlen(m_pMapBuffer)) return bRet;
    try {
        lock(&g_cfg._meta_data->_lock);
        Dynamic::Var js            = m_jsonParser.parse(m_pMapBuffer);
        JSON::Object::Ptr pJsonObj = js.extract<JSON::Object::Ptr>();
        bRet                       = pJsonObj->has(key);
        unlock(&g_cfg._meta_data->_lock);
    } catch (JSON::JSONException& exc) {
        unlock(&g_cfg._meta_data->_lock);
        eLog(exc.displayText());
    }
    return bRet;
}

Dynamic::Var JsonMgr::getValue(const string& key)
{
    dLog(__FUNCTION__);
    Dynamic::Var varRet;
    if (NULL == m_pMapBuffer || 0 == strlen(m_pMapBuffer)) return varRet;
    try {
        lock(&g_cfg._meta_data->_lock);
        Dynamic::Var js            = m_jsonParser.parse(m_pMapBuffer);
        JSON::Object::Ptr pJsonObj = js.extract<JSON::Object::Ptr>();
        varRet                     = pJsonObj->get(key);
        unlock(&g_cfg._meta_data->_lock);
    } catch (JSON::JSONException& exc) {
        unlock(&g_cfg._meta_data->_lock);
        eLog(exc.displayText());
    }
    return varRet;
}

bool JsonMgr::setValue(const string& key, const Dynamic::Var& value)
{
    if (NULL == m_pMapBuffer) {
        eLog("pMapBuffer is nullptr");
        return false;
    }

    if (0 == strlen(m_pMapBuffer)) {
        dLog("pMapBuffer is empty");
        snprintf(m_pMapBuffer, RECORDER_MAP_BUF_SIZE - 1, "{}");
    }

    try {
        lock(&g_cfg._meta_data->_lock);
        Dynamic::Var js            = m_jsonParser.parse(m_pMapBuffer);
        JSON::Object::Ptr pJsonObj = js.extract<JSON::Object::Ptr>();
        if (pJsonObj) pJsonObj->set(key, value);

        if (strlen(js.toString().c_str()) < RECORDER_MAP_BUF_SIZE)
            snprintf(m_pMapBuffer, RECORDER_MAP_BUF_SIZE - 1, "%s", js.toString().c_str());
        else
            eLog("Json length out of the range");

        unlock(&g_cfg._meta_data->_lock);
    } catch (JSON::JSONException& exc) {
        unlock(&g_cfg._meta_data->_lock);
        eLog(exc.displayText());
        return false;
    }
    return true;
}
