#ifndef __JSON_MGR_H__
#define __JSON_MGR_H__

#include <Poco/JSON/Object.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/ParseHandler.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Array.h>
#include <Poco/SharedPtr.h>
#include <Poco/Dynamic/Var.h>
#include <string>
using std::string;
using namespace Poco;

// #define NO_DEBUG_MSG

#ifdef dLog
#undef dLog
#endif

#ifdef NO_DEBUG_MSG
#define dLog(msg)
#else
#define dLog(msg)                                                                            \
    do {                                                                                     \
        std::cout << "(" << getpid() << "|" << pthread_self() << ")" \
        << "[DEBUG] " << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl; \
    } while (0);
#endif

#ifdef eLog
#undef eLog
#endif

#ifdef NO_ERROR_MSG
#define eLog(msg)
#else
#define eLog(msg)                                                                            \
    do {                                                                                     \
        std::cerr << "(" << getpid() << "|" << pthread_self() << ")" \
        << "[ERROR] " << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl; \
    } while (0);
#endif

#define JSON_FILE_TEMP "/tmp/g5r2.data.json"

class JsonMgr
{
private:
    string            m_strJsonPath;
    JSON::Parser      m_jsonParser;
    char*             m_pMapBuffer;

public:
    JsonMgr(const string& strJsonFilePath);
    virtual ~JsonMgr();

    void loadFile(const string& strPath);  // reserve for old version compatibility

    bool writeFile(const string& strPath);

    bool isKeyExist(const string& key);

    Dynamic::Var getValue(const string& key);

    bool setValue(const string& key, const Dynamic::Var& value);

    char * getBuffer() {return m_pMapBuffer;}

private:
    JsonMgr(void);
    void* shareFile(const string& strPath);
};

#endif  // !__JSON_MGR_H__
