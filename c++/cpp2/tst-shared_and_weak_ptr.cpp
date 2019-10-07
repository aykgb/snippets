#include <memory>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class StrBlobPtr;

class StrBlob {
    typedef std::vector<string>::size_type size_type;

    friend class StrBlobPtr;
public:
    StrBlob();
    //  ~StrBlob();

    StrBlob(std::initializer_list<std::string> il); // initializer_list 将允许使用c++11标准的列表初始化方法初始化类

    size_type size() const
    {
        return data->size();
    }
    bool empty() const
    {
        return data->empty();
    }

    void push_back(const std::string& t)
    {
        data->push_back(t);
    }
    void pop_back();

    std::string& front();
    std::string& back();

    StrBlobPtr begin();
    StrBlobPtr end();

private:
    void check(size_type i, const std::string& msg) const;

private:
    /* data is a smart pointer variable. */
    std::shared_ptr<std::vector<std::string>> data;
};

inline
StrBlob::StrBlob(): data(make_shared<vector<string>>()) { }

/* Use the standred c++11 list initializer */
StrBlob::StrBlob(std::initializer_list<string> il):
    data(make_shared<vector<string>>(il)) {}

inline
void StrBlob::check(size_type i, const std::string &msg) const
{
    if(i >= data->size()) throw out_of_range(msg);
}

inline
std::string& StrBlob::front()
{
    check(0, "front on empty StrBlob.");
    return data->front();
}

inline
std::string& StrBlob::back()
{
    check(0, "back on empty StrBlob.");
    return data->back();
}

inline
void StrBlob::pop_back()
{
    check(0, "pop_back on empty StrBlob.");
    data->pop_back();
}

class StrBlobPtr {
public:
    StrBlobPtr(): curr(0) { }
    StrBlobPtr(StrBlob &a, size_t sz = 0):
        wptr(a.data), curr(sz) { }
    std::string& deref() const;
    StrBlobPtr& incr(); // 前缀递增

private:
    /* 若检查成功，check返回一个指向vector的shared_ptr */
    std::shared_ptr<std::vector<std::string>>  check(std::size_t, const std::string&) const;

    /* 保存一个weak_ptr, 意味着底层vector可能会被销毁 */
    std::weak_ptr<std::vector<std::string>> wptr;
    std::size_t curr;
};

std::shared_ptr<std::vector<std::string>>
                                       StrBlobPtr::check(std::size_t i, const std::string &msg) const
{
    auto ret = wptr.lock();
    if(!ret)
        throw std::runtime_error("unbound StrBlobptr");
    if(i >= ret->size())
        throw std::out_of_range(msg);
    return ret;
}

std::string& StrBlobPtr::deref() const
{
    auto p = check(curr, "dereference past end");
    return (*p)[curr];
}

StrBlobPtr& StrBlobPtr::incr()
{
    check(curr, "increment past end of StrBlobptr");
    ++curr;
    return *this;
}

inline
StrBlobPtr StrBlob::begin()
{
    return StrBlobPtr(*this);
}

inline
StrBlobPtr StrBlob::end()
{
    auto ret =  StrBlobPtr(*this, data->size());
    return ret;
}

int main()
{
    StrBlob str_blob{ "101001", "abckd", "degbh", "mine", " " };
    StrBlob str_blob0 = str_blob;

    std::cout << str_blob0.front() << "\n";

    return 0;
}
