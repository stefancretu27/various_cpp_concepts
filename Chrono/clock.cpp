#include "headers.hpp"

#include <thread>

void clock_insights()
{
    using namespace std;

    cout<<endl<<"Insights on std::chrono::clock"<<endl;
    cout<<"     1. A clock comprises of a starting point, also referred to as epoch, and a tick rate. In the chrono library are defined multiples types"<<endl;
    cout<<"     of clocks, with the following 3 being available in c++11, whilst the others being introduced in c++20."<<endl;
    cout<<"      1. system_clock."<<endl;
    cout<<"      2. steady_clock"<<endl;
    cout<<"      3. high_resolution_clock"<<endl;
    cout<<"     The above listed types of clocks meet the requirements of TrivialClock."<<endl<<endl;

    cout<<"     2. system_clock represents the system real time clock and it is the only c++ clock that can map its time points to C-style time."<<endl;
    cout<<"     However, it is not a monotonic clock, as the time can be adjusted at any moment. In detail, this clock can be the subject of variations"<<endl;
    cout<<"     thus it can jump backward or forward. For instance, this can happen when it is synchronized with other clocks (via NTP: Network Time Protocol)"<<endl;
    cout<<"     or when DST is set on/off. The epoch of a system_clock usually is set to Unix time: 1st of January 1970."<<endl;
    cout<<"     The class implementing the system_clock has several static methods:"<<endl;
    cout<<"         - now(): returns a time_point representing the current point in time"<<endl;
    cout<<"         - to_time_t(): converts system_clock to time_t, that is C style clock"<<endl;
    cout<<"         - from_time_t(): converts from time_t to system_clock"<<endl<<endl;

    
    chrono::system_clock sysClock;
    cout<<"is system clock steady (static constexpr) "<<sysClock.is_steady<<endl;

    //get a time_point
    chrono::time_point<chrono::system_clock> start_time_point = chrono::system_clock::now();

    //mimic some work
    this_thread::sleep_for(250ms);

    //get a time_point
    auto end_time_point = chrono::system_clock::now();

    //difference between 2 time points is a of type duration
    chrono::duration<int64_t, nano> intervalIntNanoseconds = end_time_point - start_time_point;
    chrono::duration<double, milli> intervalDoubleMilliseconds = end_time_point - start_time_point;

    cout<<"time interval elapsed: "<<intervalIntNanoseconds.count()<<" "<<intervalDoubleMilliseconds.count()<<endl;

    cout<<"use c style time_t as argument for from_time_t to convert to a time point, then get time since epoch"<<endl;
    //initialize time_t struct instance
    time_t cStyleTime = time(nullptr);
    //localtime converts into calendar time, expressed in local time, the time since epoch given as argument
    tm* calendarTime = localtime(&cStyleTime);
    //asctime converts the given calendar time tm* to a textual representation of form: www mmm dd hh:mm:ss yyyy
    cout<<asctime(calendarTime)<<endl;

    chrono::time_point<chrono::system_clock> fromTimeT = chrono::system_clock::from_time_t(cStyleTime);
    //get time since unix epoch
    cout<<chrono::duration_cast<chrono::hours>(fromTimeT.time_since_epoch()).count()<<endl;

    cout<<"use time_point as argument for to_time_t to convert to C style time_t and print the result"<<endl;
    cStyleTime = chrono::system_clock::to_time_t(start_time_point);
    cout<<asctime(localtime(&cStyleTime))<<endl;

    cout<<"     2. steady_clock represents a monotonic clock that measures physical time. This means the measured time always moves forward, as the "<<endl;
    cout<<"     time points cannot decrease. In addition, the time between ticks is constant, hence the monotony. That said, this clock is more suitable"<<endl;
    cout<<"     for measuring time intervals, as it is more accurate and it is not bound to an epoch becuase is not related to a wall clock."<<endl;
    cout<<"     Nevertheless, it can suffer adjustments if it detects that the local quartz is ticking slower/faster then the server's (via NTP) and"<<endl;
    cout<<"     consequently it adjusts the time intervals (bigger/smaller) but it does not entail jumps back in time."<<endl;
    cout<<"     The steady clock can be related to some system time, in the sense that it can represent the time elapse since boot, for instance."<<endl;
    cout<<"     The class implementing the steady_clock has only one static method: now. It is used to retrieve the current time point"<<endl<<endl;

    chrono::steady_clock steadyClk;
    cout<<"is steady clock steady (static constexpr) "<<steadyClk.is_steady<<endl;

    chrono::time_point<chrono::steady_clock> startSteady {chrono::steady_clock::now()};
    this_thread::sleep_for(100ms);
    auto endSteady {chrono::steady_clock::now()};
    //by default, duration<double> computes seconds
    chrono::duration<double, milli> elapsedSteady {endSteady - startSteady};

    cout<<" steady clock measured interval "<<elapsedSteady.count()<<endl;

    cout<<"     3. high_resolution_clock represents the clock with the smallest tick available on the given implementation. In many SDK's/libraries it"<<endl;
    cout<<"     is a alias for either steady_clock or system_clock. Rarely, it comes with a separate implementation. Therefore, this type of clock "<<endl;
    cout<<"     is often avoided and is preferred to use the clock implementation (system/steady) that is appropriate for the given case, not the alias."<<endl;
    cout<<"     This is done in order to avoid ambiguous situations, such as a program using high_resolution_clock used on a configuration that makes "<<endl;
    cout<<"     use of system_clock is then executed on another machine with other configuration that uses setady_clock for aliasing it."<<endl;
    cout<<"     The class implementing the high_resolution_clock has only one static method: now. It is used to retrieve the current time point"<<endl<<endl;

    cout<<"is high_resolution_clock steady: "<<chrono::high_resolution_clock::is_steady<<" False => On this config it is aliased to system_clock"<<endl;

    cout<<"     4. As (private) members, both system_clock and steady_clock non-template classes encapsulate the follwing:"<<endl;
    cout<<"      - rep = which is the number of ticks in the clock's duration"<<endl;
    cout<<"      - period = it is a std::ratio type, representing the tick period of the clock, in seconds (interval between consecutive ticks)"<<endl;
    cout<<"      - duration = chrono::duration<rep, period>, capable of representing negative duration in case of system_clock"<<endl;
    cout<<"      - time_point = chrono::time_point<chrono::system_clock> which stores the measurement retruned by now()"<<endl<<endl;

}