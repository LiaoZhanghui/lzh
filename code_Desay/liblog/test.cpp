/*******************************************************************
 > Copyright © 2014 Fang Tao. All rights reserved.
 > FileName   : test.c
 > Description:
 > Author     : Fang Tao <Tao.Fang@desay-svautomotive.com>
 > Version    : v1.0 - Wed 21 Mar 2018 07:49:10 PM HKT
 *******************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "SVPLog.h"
#include "JsonMgr.h"
#include "Recorder.h"
#include <string>
using namespace std;

#ifdef SVP_LOG_TAG
#undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG "test"

#define TEST_STRING "+++++++++++++++++++++++++++++++++++++++++this is a test log+++++++++++++++++++++++++++++++++++++++++"

void loop_log(int count)
{
    clock_t start_time = clock();
    for (int i = 0; i < count; ++i) {
        SVP_INFO(TEST_STRING);
    }
    clock_t  end_time = clock();
    long int dur      = end_time - start_time;
    printf("time elapsed: %ld.%ld s\n", dur / CLOCKS_PER_SEC, dur % CLOCKS_PER_SEC * 1000 / CLOCKS_PER_SEC);
}

int main()
{
    #ifdef PC_TEST
    cout << "pc test mode" << endl;
    #else
    cout << "target mode" << endl;
    #endif
    // test for JsonMgr
    JsonMgr jm(JSON_FILE);
    cout << "write file: " << jm.writeFile("data1.json") << endl;

    // bool ret = JsonMgr::getInstance()->isKeyExist("Version");
    cout << "Version: " << jm.isKeyExist("Version") << endl;
    cout << "Fader: " << jm.isKeyExist("Fader") << endl;
    cout << "sizeof string: " << sizeof(string) << endl;

    if (0 == fork()) {
        // test for Recorder
        Recorder::getInstance()->setJsonFilePath("data.json");
        Recorder::getInstance()->recVersion("1.1");
        Recorder::getInstance()->recRoadsideAssistanceClickTime("20181029090000");
        Recorder::getInstance()->recLastMode(1);
        Recorder::getInstance()->recAppSelectedTimes("test");
        Recorder::getInstance()->recAppSelectedTimes("test");
        Recorder::getInstance()->recWiFiStatus(true);
        Recorder::getInstance()->recBtStatus(true);
        Recorder::getInstance()->recMuteStatus(true);
        Recorder::getInstance()->rec4GStatus(true);
        Recorder::getInstance()->recOffModeStatus(true);
        Recorder::getInstance()->recVolume(1, 100);
        Recorder::getInstance()->recVolume(2, 100);
        Recorder::getInstance()->recFader(100);
        Recorder::getInstance()->recBalance(-100);
        Recorder::getInstance()->recBassBoostStatus(2);
        Recorder::getInstance()->recEqualizer(2);
        Recorder::getInstance()->recLoudnessStatus(true);
        Recorder::getInstance()->recHKeyBeepStatus(true);
        Recorder::getInstance()->recTouchBeepStatusu(true);
        Recorder::getInstance()->outputRecords("data2.json");
        // exit(0);
    }
    Recorder::getInstance()->recAppSelectedTimes("test");
    Recorder::getInstance()->recDisplayMode(2);
    Recorder::getInstance()->recDisplayBrightness(100);
    Recorder::getInstance()->recVoiceLanguage(2);
    Recorder::getInstance()->recTextLanguage(2);
    Recorder::getInstance()->recInputMethod(0, true);
    Recorder::getInstance()->recInputMethod(1, true);
    Recorder::getInstance()->recInputMethod(2, true);
    Recorder::getInstance()->recInputMethod(3, true);
    Recorder::getInstance()->recHandWriteLanguage(2);
    Recorder::getInstance()->recHandWriteAutoComposeTime(2);
    Recorder::getInstance()->recGpsSyncStatus(true);
    Recorder::getInstance()->recAdjustedTime("20180101000000");
    Recorder::getInstance()->recTimeFormat(1);
    Recorder::getInstance()->rerDistanceUnit(1);
    Recorder::getInstance()->recCoordinateFormat(2);
    Recorder::getInstance()->recGeodeticDatum(2);
    // Recorder::widget_stat_t s{1,1,1};
    Recorder::getInstance()->recWidgetStatus(2, { 1, 1, 1 });
    Recorder::getInstance()->outputRecords("data3.json");
    Recorder::releaseInstance();

    int   s;
    pid_t cid = wait(&s);
    if ((cid > 0) && (WIFEXITED(s))) {
        cout << "cid: " << cid << " returned " << WEXITSTATUS(s) << endl;
    }

    return 0;

    // test for log
    log_init();
    // test of log interface with level
    SVP_DEBUG(TEST_STRING);
    SVP_INFO(TEST_STRING);
    SVP_WARN(TEST_STRING);
    SVP_ERROR(TEST_STRING);
    SVP_FATAL(TEST_STRING);

    // performance test of single process, how much time does it take for 10000 records
    loop_log(10000);

    // modify log level
    set_log_level(3);
    sleep(2);
    SVP_INFO("after log level changed");
    SVP_ERROR("after log level changed");  // add -DDEV when compiling

    // test of log export
    // export_all_logs("backup");

    log_exit();
    /*
        pid_t pid = fork();
        if (pid != 0)
        {
            printf("pid = %u\n", pid);
            return 0;// parent process
        }
        // child process
        printf("starting logservice\n");
        close(0);
        close(1);
        close(2);
        umask(0);
        // chdir("/");
        setsid();

        log_init();
        int i = 0;
        SVP_DEBUG("i = %d\n", i);

        char p[12] = "";
        SVP_DEBUG("size = %d\n", sizeof(p));

        char q[12] = "abcd";
        SVP_DEBUG("size = %d\n", sizeof(q));
        for (; i<50; ++i)
        {
            SVP_DEBUG(TEST_STRING);
            SVP_INFO(TEST_STRING);
            SVP_WARN(TEST_STRING);
            SVP_ERROR(TEST_STRING);
            SVP_FATAL(TEST_STRING);
            sleep(1);
        }
        log_exit();*/
    // sleep(1);
    return 0;
}
