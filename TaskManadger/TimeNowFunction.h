#pragma once

#include <boost/date_time/posix_time/posix_time.hpp>

using Time = boost::posix_time::millisec_posix_time_system_config::time_duration_type;

Time timeNow();