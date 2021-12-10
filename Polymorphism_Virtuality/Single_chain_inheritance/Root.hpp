#pragma once

class Root
{
	public:
	Root();
	virtual ~Root();
	virtual void pureVirtualMethod() = 0;
	virtual void virtualMethod();
	void method();
};

