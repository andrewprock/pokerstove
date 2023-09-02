#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>

namespace pokerstove
{

/**
 * This is to get around the worse interface presented
 * by the boost timer code. timer.elapsed() used to return
 * the number of seconds, but no there is no native interface
 * to get the number of seconds from a timer as a double.
 *
 * input: boost timer
 * output: number of elapsed wall seconds as a double
 */
double elapsed(boost::timer::cpu_timer timer) {
    boost::timer::nanosecond_type elapsed = timer.elapsed().wall;
    double nanos = static_cast<double>(boost::chrono::nanoseconds(elapsed).count());
    return nanos * boost::chrono::nanoseconds::period::num / boost::chrono::nanoseconds::period::den;
}

}
