#ifndef HELPERCLASS
#define HELPERCLASS

#include <iostream>

class HelperClass
{
    public:
    HelperClass() = default;
    ~HelperClass()
    {
        std::cout<<"HelperClass d-tor"<<std::endl;
    }
};

class Garbage
{
	private:
	double d{};
	
	public:
	//default c-tor useful for atomic<Garbage> declaration
	Garbage() = default;
	//Garbage():d{0.0f}{}      //not accepted as a default c-tor when declaring atomic<Garbage>, as it is user provided and not implicit or default
	explicit Garbage(const double& in):d{in}
	{
		std::cout<<__func__<<" non default c-tor"<<std::endl;
	}
	~Garbage() = default;
	
	double getD() const{return d;};
	void setD(const double& in){d = in;};
	
	/* non trivial dtor, copy ctor and move ctor => if they are provided, by the user, the class won't be suitable for usage with atomic<T>, as it is non trivial
	 * Garbage(const Garbage& obj)
	 * {
	 * 		d = obj.d;
	 * }
	 * 
	 * Garbage(Garbage&& obj)
	 * {
	 * 		d = move(obj.d);
	 * 		obj.d = 0.0f;
	 * }
	 * 
	 * virtual ~Garbage(){};
	 */
	
	
};

#endif
