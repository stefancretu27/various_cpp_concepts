#include "headers.hpp"

using namespace std;

int main()
{
	cout<<"----------------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl<<"Insights on std::chrono"<<endl;
    cout<<"     The chrono library contains mechanisms for tracking time with varying degrees of precision. Nonetheless, C-style mechanisms used for"<<endl;
    cout<<"     time and date tracking are included in other C++ libraries, such as <ctime> that embeds time_t and clock_t, amongst others."<<endl;
    cout<<"     The chrono library defines 3 main types that serve for the time tracking purpose. "<<endl;
    cout<<"      1. clock"<<endl;
    cout<<"      2. time point"<<endl;
    cout<<"      3. duration"<<endl;
    cout<<"     Each of these comes along with classes defining objects with corresponding behaviors. These classes are available in the namespace chrono"<<endl;

    cout<<"-------------------------------------------clock--------------------------------------------------------------"<<endl;
    clock_insights();
    cout<<"-------------------------------------------time_point---------------------------------------------------------"<<endl;
    //time_point_insights();
    cout<<"-------------------------------------------duration-----------------------------------------------------------"<<endl;
    //duration_insights();
    
    return 0;
}
