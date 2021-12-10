#include "headers.hpp"

void time_point_insights()
{
    using namespace std;
    
    cout<<endl<<"Insights on std::chrono::time_point"<<endl;
    cout<<"     1. time_point is a template class whose implementation represents a point in time. Its template type parameters are the Clock type"<<endl;
    cout<<"     (steady, system etc) and the Duration. The time_point class stores internally a Duration value that represents the time elapsed since the"<<endl;
    cout<<"     clock's epoch. Therefore, when substracting 2 time points, a duration value is obtained, representing the difference in time between"<<endl;
    cout<<"     the 2 time points, regradless of the clock type (and it's apoch, implicitly)."<<endl<<endl;

    cout<<"     2. The class encapsulates the following private members:"<<endl;
    cout<<"      - clock: the template parameter designating on which clock the time is measured. Also, used to retrieve the corresponding epoch"<<endl;
    cout<<"      - duration: stores the time elapsed since epoch. Duration type is retrieved from the 2nd template parameter and depends on the clock type"<<endl;
    cout<<"     the 2 time points, regradless of the clock type (and it's apoch, implicitly)."<<endl;
    cout<<"      - rep = which is the number of ticks in the clock's duration, it is a data type such as int64_t or double"<<endl;
    cout<<"      - period: std::ratio representing the clock's tick period (the interval between consecutive ticks)"<<endl<<endl;

    cout<<"     3. The class encapsulates the following methods:"<<endl;
    cout<<"      - c-tor"<<endl;
    cout<<"      - time_since_epoch(): returns duration elpased since the start of the epoch of the clock the time_point is associated with"<<endl;
    cout<<"      - min/max: static methods returning a time point associated with the smallest/largest possible duration"<<endl;
    cout<<"      - operator +=/-=: modifying the value of the time point by the given duration"<<endl<<endl;

    //specify explicitly duration type with the afferent rep (data type) as 2nd template parameter, as well as the ratio. 
    chrono::time_point<chrono::system_clock, chrono::duration<double, milli>> timePointSystemClk;
    //add an offset to the time point consisting of a duration interval
    timePointSystemClk += chrono::duration<double>(111s);
    timePointSystemClk -= chrono::duration<double>(1.1ms);
    cout<<(timePointSystemClk.time_since_epoch()).count()<<endl;

    //by default, the duration rep is long int (int64_t)
    auto minTimePoint = chrono::time_point<chrono::steady_clock>::min();
    cout<<(chrono::steady_clock::now() - minTimePoint).count()<<endl;
    chrono::time_point<chrono::steady_clock> timePointSteadyClk; 
    cout<<timePointSteadyClk.time_since_epoch().count()<<endl;
    auto currTimePointSteadyClk = chrono::steady_clock::now();
    cout<<(currTimePointSteadyClk - timePointSteadyClk).count()<<endl;

}