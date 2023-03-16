#include "TimeNowFunction.h"

Time timeNow() {
	return boost::posix_time::microsec_clock::local_time().time_of_day();
}
