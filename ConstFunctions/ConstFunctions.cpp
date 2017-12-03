// ConstFunctions.cpp : Defines the entry point for the console application.
//

#include<mutex>
#include<iostream>
#include <memory>

class MyClass1
{
public:
	MyClass1(int value) : m_Value(value) {}

	int GetValue() const
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_Value;
	}
private:
	int m_Value;
	mutable std::mutex m_Mutex;
};

class MyClass2
{
public:
	MyClass2(int value) : m_Value(value) {}

	int GetValue() 
	{
		std::lock_guard<std::mutex> lock(m_Mutex);
		return m_Value;
	}
private:
	int m_Value;
	std::mutex m_Mutex;
};

class MyClass3
{
public:
	MyClass3(int value) : m_Value(value), m_pMutex(&m_Mutex) {}

	int GetValue() const
	{
		std::lock_guard<std::mutex> lock(*m_pMutex);
		return m_Value;
	}
private:
	int m_Value;
	std::mutex m_Mutex;
	std::mutex* m_pMutex;
};

/*
// cannot work.
class MyClass4
{
public:
	MyClass4(int value) : m_Value(value) {}

	int GetValue() const
	{
		std::mutex* pMutex = &m_Mutex; // compiler complain
		std::lock_guard<std::mutex> lock(*pMutex);
		return m_Value;
	}
private:
	int m_Value;
	std::mutex m_Mutex;
};
*/

class MyClass5
{
public:
	MyClass5(int value) : m_Value(value), m_pMutex(std::make_unique<std::mutex>()) {}

	int GetValue() const
	{
		std::lock_guard<std::mutex> lock(*m_pMutex);
		return m_Value;
	}
private:
	int m_Value;
	std::unique_ptr<std::mutex> m_pMutex;
};

int GetValue(const MyClass1& c)
{
	return c.GetValue();
}

int GetValue(const MyClass2& c)
{
	MyClass2& c2 = const_cast<MyClass2&>(c);
	return c2.GetValue();
}

int GetValue(const MyClass3& c)
{
	return c.GetValue();
}

/*
int GetValue(const MyClass4& c)
{
	return c.GetValue();
}
*/

int GetValue(const MyClass5& c)
{
	return c.GetValue();
}

int main()
{
	const MyClass1 c1(10);
	std::cout << "MyClass1 object value:" << GetValue(c1) << std::endl;
	const MyClass2 c2(20);
	std::cout << "MyClass2 object value:" << GetValue(c2) << std::endl;
	const MyClass3 c3(30);
	std::cout << "MyClass3 object value:" << GetValue(c3) << std::endl;
	//const MyClass4 c4(40);
	//std::cout << "MyClass4 object value:" << GetValue(c4) << std::endl;
	const MyClass5 c5(50);
	std::cout << "MyClass4 object value:" << GetValue(c5) << std::endl;
	return 0;
}

