#include "SVPSomeIP.h"

#include <vsomeip/vsomeip.hpp>

#include "SVPLog.h"

#ifdef SVP_LOG_TAG
    #undef SVP_LOG_TAG
#endif
#define SVP_LOG_TAG        "libbasic|someip"

void SVPSomeIP::SVP_PrintClientID(const std::string & connection)
{
    thread_local auto p_runtime = vsomeip::runtime::get();
    if (nullptr == p_runtime) return;
    thread_local auto p_application = p_runtime->get_application(connection);
    if (nullptr == p_application) return;
    SVP_INFO("Connection: %s, ClientID: %04x", connection.c_str(), p_application->get_client());
}
