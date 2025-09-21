# C++ Tip Modification Inside const Function

## Introduction

There are times where modification inside `const` member function must be done (for example, to allow for caching or memoization). The `mutable` keyword and `const_cast` are well-known in the C++ circles to work around these. The 3<sup>rd</sup> way is to use a `const` pointer; we cannot modify the `const` pointer but we can modify the contents it pointed to. Let&#39;s dive in and discover for ourselves.

## mutable Keyword

We will do a quick recap on the `mutable` keyword. A lock have to be acquired on mutex. So by declaring the `mutex` `mutable`, it can be used inside a `const` function.

```Cpp
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

int GetValue(const MyClass1& c)
{
    return c.GetValue();
}
```

## Cast Away Constness

Next example, we will declare the member function as non-`const` and do a `const_cast()`. Typically C++ developers who do not know the `mutable` keyword&nbsp;will use this technique.

```Cpp
class MyClass2
{
public:
    MyClass2(int value) : m_Value(value) {}

    int GetValue() // not const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_Value;
    }
private:
    int m_Value;
    std::mutex m_Mutex;
};
int GetValue(const MyClass2& c)
{
    MyClass2& c2 = const_cast<MyClass2&>(c); // cast away the constness
    return c2.GetValue();
}
```

## Const Pointer to Member

As advertised, the data member can be modified though the member pointer.

```Cpp
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
int GetValue(const MyClass3& c)
{
    return c.GetValue();
}
```

## Local Pointer to Member Does Not Compile

If local pointer is used, compiler will complain "`error C2440: &#39;initializing&#39;: cannot convert from &#39;const std::mutex *&#39; to &#39;std::mutex *`&#39;".

```Cpp
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
int GetValue(const MyClass4& c)
{
    return c.GetValue();
}
```

## unique_ptr

Instead of raw pointer, using `unique_ptr` works as well.

```Cpp
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
int GetValue(const MyClass5& c)
{
    return c.GetValue();
}
```
