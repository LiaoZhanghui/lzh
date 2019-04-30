/******************************************************************************
*
* Company       : Huizhou Desay SV Automotive Co., Ltd.
*
* Division      : Automotive Electronics, Desay Corporation
*
* Business Unit : Central Technology
*
* Department    : Advanced Development (Huizhou)
*
******************************************************************************/

#include "SVPTime.h"

#include <time.h>

#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG     "libbasic|time"

#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)

#else
    #include <unistd.h>
    #include <sys/time.h>
    #include <netinet/in.h>
	#include "sys/shm.h"
#endif

static uint16_t g_wSaveYear = 2017;
static uint16_t g_wCurrentYear = 2017;

static uint8_t* g_pShmAddress = NULL;
static int32_t g_shmID = -1;

#define SHARE_MEM_SIZE 16
void SVPTime::getSystemTime(SVPTypeSystemTime& sys_time) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    ::GetSystemTime(&sys_time);
#else
    time_t curtm;
    time(&curtm);
    struct tm* ptm_sys = gmtime(&curtm);

    if (NULL == ptm_sys) {
        SVP_ERROR("getSystemTime() fail!");
        return;
    }

    sys_time.wYear          = ptm_sys->tm_year + 1900;
    sys_time.wMonth         = ptm_sys->tm_mon + 1;
    sys_time.wDayOfWeek     = ptm_sys->tm_wday;
    sys_time.wDay           = ptm_sys->tm_mday;
    sys_time.wHour          = ptm_sys->tm_hour;
    sys_time.wMinute        = ptm_sys->tm_min;
    sys_time.wSecond        = ptm_sys->tm_sec;
    sys_time.wMilliseconds  = 0;
    //printf("GET SYS: %s\n", asctime(ptm_sys));
#endif
}

void SVPTime::setSystemTime(const SVPTypeSystemTime& sys_time) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    ::SetSystemTime(&sys_time);
#else
    struct tm tm_sys;
    tm_sys.tm_year          = sys_time.wYear - 1900;
    tm_sys.tm_mon           = sys_time.wMonth - 1;
    tm_sys.tm_mday          = sys_time.wDay;
    tm_sys.tm_wday          = sys_time.wDayOfWeek;
    tm_sys.tm_hour          = sys_time.wHour;
    tm_sys.tm_min           = sys_time.wMinute;
    tm_sys.tm_sec           = sys_time.wSecond;
    // Correct any day of week and day of year etc. fields
    tm_sys.tm_isdst         = -1;
    //printf("SET SYS: %s\n", asctime(&tm_sys));
    time_t tt_sys = mktime(&tm_sys);

    if (tt_sys == -1) {
        SVP_ERROR("setSystemTime() - mktime() fail!");
        return;
    }

    tm* ptm_loc = gmtime(&tt_sys);

    if (NULL == ptm_loc) {
        SVP_ERROR("setSystemTime() - gmtime() fail!");
        return;
    }

    time_t tt_loc = (tt_sys - mktime(ptm_loc)) + tt_sys; // timzzone dist + sys time

    //SVP_DEBUG("SET LOC: %s", asctime(localtime(&tt_loc)));

    if (!stime(&tt_loc)) {
        // OK
        SVP_DEBUG("setSystemTime() - %04d-%02d-%02d %02d:%02d:%02d", sys_time.wYear, sys_time.wMonth, sys_time.wDay,
            sys_time.wHour, sys_time.wMinute, sys_time.wSecond );
    } else {
        // Failed
        SVP_ERROR("setSystemTime() - stime() fail!");
    }

#endif
}

bool SVPTime::getLocalTime(SVPTypeSystemTime& loc_time)
{
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    ::GetLocalTime(&loc_time);
#else
    time_t curtm;
    time(&curtm);
    //#if 0 // OS fixed time zone issue.
    //  curtm += 28800; //8*3600; 东8区
    //#endif
    tm* ptm_loc = localtime(&curtm);

    if (NULL == ptm_loc)
    {
        SVP_ERROR("getLocalTime() fail!");
        return false;
	}
	
	if(NULL == g_pShmAddress)
	{
		g_shmID = shmget(20160927, SHARE_MEM_SIZE, IPC_CREAT);
		if (-1 != g_shmID)
		{
			g_pShmAddress = (uint8_t*)shmat(g_shmID, NULL, 0);
			if (-1 != (int)g_pShmAddress)
			{
				//memset(g_pShmAddress, 0x0, SHARE_MEM_SIZE);
				memmove(&g_wSaveYear, g_pShmAddress, sizeof(g_wSaveYear));
				memmove(&g_wCurrentYear, g_pShmAddress+4, sizeof(g_wCurrentYear));
				if(0 == g_wSaveYear)
				{
					g_wSaveYear = 2017;
					memmove(g_pShmAddress, &g_wSaveYear, sizeof(g_wSaveYear));
				}
				if(0 == g_wCurrentYear)
				{
					g_wCurrentYear = 2017;
					memmove(g_pShmAddress+4, &g_wCurrentYear, sizeof(g_wCurrentYear));
				}
			}
			else
			{
				SVP_ERROR("SVPTime::GetLocalTime - g_pShmAddress = -1!!!!.");
                return false;
			}
		}
		else
		{
			SVP_ERROR("SVPTime::GetLocalTime - g_shmID = -1!!!!.");
            return false;
		}
	}
	else
	{
		memmove(&g_wSaveYear, g_pShmAddress, sizeof(g_wSaveYear));
		memmove(&g_wCurrentYear, g_pShmAddress+4, sizeof(g_wCurrentYear));
	}

    //If the system time has crossed the saved year,
    //then g_wSaveYear and g_wCurrentYear need to be corrected.
	if((ptm_loc->tm_year + 1900) > g_wSaveYear) 
	{
		//g_wSaveYear = ptm_loc->tm_year + 1900;
		g_wCurrentYear++;
        //fix the case:while OS time is 2036, but current year is not leap year.
        if (g_wCurrentYear > 2037)
        {
            if (((g_wCurrentYear%4 == 0) && (g_wCurrentYear%100 != 0)) || (g_wCurrentYear%400 == 0)) 
            {
                g_wSaveYear = 2036;
            }
            else
            {
                g_wSaveYear = 2035;
            }
            //save the corrected year to system
            struct tm tm_loc;
            tm_loc.tm_year          = g_wSaveYear - 1900;
            //tm_loc.tm_year          = loc_time.wYear - 1900;
            tm_loc.tm_mon           = ptm_loc->tm_mon;
            tm_loc.tm_mday          = ptm_loc->tm_mday;
            tm_loc.tm_wday          = ptm_loc->tm_wday;
            tm_loc.tm_hour          = ptm_loc->tm_hour;
            tm_loc.tm_min           = ptm_loc->tm_min;
            tm_loc.tm_sec           = ptm_loc->tm_sec;
            tm_loc.tm_isdst         = -1;
            time_t tt_loc = mktime(&tm_loc);
            stime(&tt_loc);
        }
        else
        {
            g_wSaveYear = ptm_loc->tm_year + 1900;
        }
        memmove(g_pShmAddress, &g_wSaveYear, sizeof(g_wSaveYear));
        memmove(g_pShmAddress+4, &g_wCurrentYear, sizeof(g_wCurrentYear));
	}

	loc_time.wYear = g_wCurrentYear;
    loc_time.wMonth         = ptm_loc->tm_mon + 1;
    loc_time.wDayOfWeek     = ptm_loc->tm_wday;
    loc_time.wDay           = ptm_loc->tm_mday;
    loc_time.wHour          = ptm_loc->tm_hour;
    loc_time.wMinute        = ptm_loc->tm_min;
    loc_time.wSecond        = ptm_loc->tm_sec;
    loc_time.wMilliseconds  = 0;

    return true;
#endif
}

bool SVPTime::setLocalTime(const SVPTypeSystemTime& loc_time) {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    ::SetLocalTime(&loc_time);
#else
    struct tm tm_loc;
	g_wCurrentYear = loc_time.wYear;
	
	if(loc_time.wYear > 2037) 
	{
		if(((loc_time.wYear%4 == 0) && (loc_time.wYear%100 != 0)) || (loc_time.wYear%400 == 0)) 
		{
			g_wSaveYear = 2036;
		}
		else 
		{
			g_wSaveYear = 2035;
		}
	}
	else 
	{
		g_wSaveYear = loc_time.wYear;
	}
	
	tm_loc.tm_year			= g_wSaveYear - 1900;
    //tm_loc.tm_year          = loc_time.wYear - 1900;
    tm_loc.tm_mon           = loc_time.wMonth - 1;
    tm_loc.tm_mday          = loc_time.wDay;
    tm_loc.tm_wday          = loc_time.wDayOfWeek;
    tm_loc.tm_hour          = loc_time.wHour;
    tm_loc.tm_min           = loc_time.wMinute;
    tm_loc.tm_sec           = loc_time.wSecond;
    // Correct any day of week and day of year etc. fields
    tm_loc.tm_isdst         = -1;
    time_t tt_loc = mktime(&tm_loc);

    if (tt_loc == -1)
    {
        SVP_ERROR("setLocalTime() - mktime() fail!");
        return false;
    }
    //  tt_loc -= 28800; //8*3600; 东8区

    if (!stime(&tt_loc))
    {
        // OK
        SVP_DEBUG("setLocalTime() - %04d-%02d-%02d %02d:%02d:%02d", loc_time.wYear, loc_time.wMonth, loc_time.wDay,
            loc_time.wHour, loc_time.wMinute, loc_time.wSecond );
			
		if(NULL == g_pShmAddress)
		{
			g_shmID = shmget(20160927, SHARE_MEM_SIZE, IPC_CREAT);
			if (-1 != g_shmID)
			{
				g_pShmAddress = (uint8_t*)shmat(g_shmID, NULL, 0);
				if (-1 != (int)g_pShmAddress)
				{
					memmove(g_pShmAddress, &g_wSaveYear, sizeof(g_wSaveYear));
					memmove(g_pShmAddress+4, &g_wCurrentYear, sizeof(g_wCurrentYear));
				}
				else
				{
					SVP_ERROR("SVPTime::SetLocalTime - g_pShmAddress = -1!!!!.");
                    return false;
				}
			}
			else
			{
				SVP_ERROR("SVPTime::SetLocalTime - g_shmID = -1!!!!.");
                return false;
			}
		}
		else
		{
			memmove(g_pShmAddress, &g_wSaveYear, sizeof(g_wSaveYear));
			memmove(g_pShmAddress+4, &g_wCurrentYear, sizeof(g_wCurrentYear));
		}
		
        return true;
    }
    else
    {
        // Failed
        SVP_ERROR("setLocalTime() - stime() fail!");
        return false;
    }

#endif
}

/**
 * \brief Set system (UTC) time and save time to share memory
 * \param[in] gpsTime:
 */
/*The 32-bit OS system does not support setting a year greater than 2038, 
so special measures are required to process years greater than 2038.*/
bool SVPTime::setZoneTime(const SVPTypeSystemTime& gpsTime)
{
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    ::SetSystemTime(&gpsTime);
#else
    struct tm tm_sys;
    g_wCurrentYear = gpsTime.wYear;
    
    //The year of more than 2037 will be converted, 
    //the leap year will be converted into 2036, 
    //the non-leap year will be converted to 2035, and the year not exceeding 2037 will remain the same.
    //Equal to 2037 is also converted, 
    //in order to prevent the occurrence of the number of hours on December 31, 2037 plus 8 becoming to 2038, 
    //resulting in failure when acquiring time seconds.
    if (gpsTime.wYear >= 2037)
    {
        if(((gpsTime.wYear%4 == 0) && (gpsTime.wYear%100 != 0)) || (gpsTime.wYear%400 == 0)) 
        {
            g_wSaveYear = 2036;
        }
        else 
        {
            g_wSaveYear = 2035;
        }
    }
    else 
    {
        g_wSaveYear = gpsTime.wYear;
    }

    tm_sys.tm_year          = g_wSaveYear - 1900;
    tm_sys.tm_mon           = gpsTime.wMonth - 1;
    tm_sys.tm_mday          = gpsTime.wDay;
    tm_sys.tm_wday          = gpsTime.wDayOfWeek;
    tm_sys.tm_hour          = gpsTime.wHour;
    tm_sys.tm_min           = gpsTime.wMinute;
    tm_sys.tm_sec           = gpsTime.wSecond;
    tm_sys.tm_isdst         = -1;

    //get the seconds since 1970.1.1 00:00:00
    time_t tt_sys = mktime(&tm_sys);

    if (tt_sys == -1)
    {
        SVP_ERROR("setZoneTime() - mktime() fail!");
        return false;
    }

    //get the time of UTC
    tm* ptm_loc = gmtime(&tt_sys);

    if (NULL == ptm_loc)
    {
        SVP_ERROR("setZoneTime() - gmtime() fail!");
        return false;
    }

    //Get the time of the current time zone after calculation.
    time_t tt_loc = (tt_sys - mktime(ptm_loc)) + tt_sys; // timzzone dist + sys 
    //time(&curtm);
    //get the current time convert from tt_loc
    tm* locZoneTime = localtime(&tt_loc);
    //If the system time after converted has crossed the saved year,
    //then g_wSaveYear and g_wCurrentYear need to be corrected.
    if ((ptm_loc->tm_year + 1900) > g_wSaveYear)
    {
        g_wCurrentYear++;
        //fix the case:while gps time is 2037.12.31 -> zone time 2038.1.1, and correct g_wSaveYear
        if (g_wCurrentYear > 2037)
        {
            if (((g_wCurrentYear%4 == 0) && (g_wCurrentYear%100 != 0)) || (g_wCurrentYear%400 == 0)) 
            {
                g_wSaveYear = 2036;
            }
            else
            {
                g_wSaveYear = 2035;
            }
        }
        else
        {
            g_wSaveYear = locZoneTime->tm_year+1900;
        }
    }
    SVP_DEBUG("setZoneTime g_wSaveYear - %d", g_wSaveYear);
    //update zone time year
    locZoneTime->tm_year = g_wSaveYear - 1900;
    //change zone time to seconds
    time_t tZoneTime = mktime(locZoneTime);

    //set the time to system
    if (!stime(&tZoneTime))
    {
        SVP_DEBUG("setZoneTime() - %04d-%02d-%02d %02d:%02d:%02d", g_wCurrentYear, locZoneTime->tm_mon+1, locZoneTime->tm_mday,
            locZoneTime->tm_hour, locZoneTime->tm_min, locZoneTime->tm_sec);
        //save g_wSaveYear and g_wCurrentYear to share memory
        if (NULL == g_pShmAddress)
        {
            g_shmID = shmget(20160927, SHARE_MEM_SIZE, IPC_CREAT);
            if (-1 != g_shmID)
            {
                g_pShmAddress = (uint8_t*)shmat(g_shmID, NULL, 0);
                if (-1 != (int)g_pShmAddress)
                {
                    memmove(g_pShmAddress, &g_wSaveYear, sizeof(g_wSaveYear));
                    memmove(g_pShmAddress+4, &g_wCurrentYear, sizeof(g_wCurrentYear));
                }
                else
                {
                    SVP_ERROR("SVPTime::SetLocalTime - g_pShmAddress = -1!!!!.");
                    return false;
                }
            }
            else
            {
                SVP_ERROR("SVPTime::SetLocalTime - g_shmID = -1!!!!.");
                return false;
            }
        }
        else
        {
            memmove(g_pShmAddress, &g_wSaveYear, sizeof(g_wSaveYear));
            memmove(g_pShmAddress+4, &g_wCurrentYear, sizeof(g_wCurrentYear));
        }

        return true;
    }
    else
    {
        // Failed
        SVP_ERROR("setZoneTime() - stime() fail!");
        return false;
    }

#endif
}

int64_t SVPTime::getTickCount() {
#if defined(SVP_WIN_32) || defined(SVP_WIN_CE)
    return ::GetTickCount();
#else
    //  struct timeval current;
    //  gettimeofday(&current, NULL);
    //  return (current.tv_sec * 1000 + current.tv_usec/1000);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((int64_t)ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000));
#endif
}
