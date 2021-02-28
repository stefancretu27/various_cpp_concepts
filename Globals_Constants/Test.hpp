class Test
{
	private:
	//error: non-static data member ‘mI’ declared ‘constexpr’ => constexpr must be static and must have initializer (in class initialization only)
	static constexpr int mI{5};
	
	public:
	constexpr int getmI(){ return mI;};
	
};

