#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <initializer_list>
#include <algorithm>

using namespace std;

class StrVec {
public:
    StrVec():
        elements(nullptr), first_free(nullptr), cap(nullptr) { }
    StrVec(const StrVec&);
    StrVec(StrVec&&) noexcept;
    StrVec(std::initializer_list<string> strlist);
    StrVec& operator=(const StrVec&);
    StrVec& operator=(std::initializer_list<std::string>);
    ~StrVec();

    void push_back(const std::string&);
    size_t size() const
    {
        return first_free - elements;
    }
    size_t capacity() const
    {
        return cap - elements;
    }
    std::string *begin() const
    {
        return elements;
    }
    std::string *end() const
    {
        return first_free;
    }

    void reserve(size_t);
    void resize(size_t, const std::string&);

    void print()
    {
        std::cout << "contents: ";
        for(auto p = elements; p != first_free;)
            std::cout << *p++ << " ";
        std::cout << "\n";
        std::cout << "    size: " << size() << "\n";
        std::cout << "capacity: " << capacity() << "\n";
        std::cout << std::endl;
    }

private:
    void reallocate();
    void free();
    void check_n_alloc()
    {
        if (size() == capacity()) reallocate();
    }
    std::pair<std::string*, std::string*> alloc_n_copy
    (const std::string*, const std::string*);

    static std::allocator<std::string> alloc; // 分配元素
    std::string *elements;   // 指向数组首元素
    std::string *first_free; // 指向数组第一个空闲元素的指针
    std::string *cap;        // 指向数组尾后位置的指针
};

std::allocator<std::string> StrVec::alloc;

void StrVec::push_back(const string& s)
{
    check_n_alloc();
    alloc.construct(first_free++, s);
}

std::pair<string*, string*>
StrVec::alloc_n_copy(const string *b, const std::string *e)
{
    /* 分配空间保存给定范围的元素 */
    auto data = alloc.allocate(e - b);
    /* 初始化并返回一个pair, 该pair由data和uninitialized_copy的返回值构成 */
    return {data, uninitialized_copy(b, e, data)}; // 将b e之间的数据copy到data中
}

void StrVec::free()
{
    if(elements) {
        // for (auto p = first_free; p != elements;)
        //   alloc.destroy(--p);
        for_each(elements, first_free, [&](std::string &s) {
            alloc.destroy(&s);
        });

        alloc.deallocate(elements, cap - elements);

    }
}

StrVec::StrVec(const StrVec &s)
{
    auto newdata = alloc_n_copy(s.begin(), s.end());
    elements = newdata.first;
    first_free = cap = newdata.second;
}

StrVec::StrVec(StrVec &&s) noexcept
    : elements(s.elements), first_free(s.first_free), cap(s.cap)
{
    // 令s进入这样的状态——对其运行析构函数是安全的
    s.elements = s.first_free = s.cap = nullptr;
}

StrVec::StrVec(std::initializer_list<string> strlist):
    elements(nullptr), first_free(nullptr), cap(nullptr)
{
    for(auto str: strlist) {
        push_back(str);
    }
}

StrVec &StrVec::operator=(std::initializer_list<string> il)
{
    // alloc_n_copy分配内存空间并从给定范围内copy元素
    auto data = alloc_n_copy(il.begin(), il.end());
    free();
}

StrVec::~StrVec()
{
    free();
}

StrVec& StrVec::operator=(const StrVec &rhs)
{
    auto data = alloc_n_copy(rhs.begin(), rhs.end());
    free();
    elements = data.first;
    first_free = cap = data.second;
    return *this;
}

void StrVec::reallocate()
{
    auto newcapacity = size() ? 2 * size() : 1;
    auto newdata = alloc.allocate(newcapacity);
    auto dest = newdata;  // 指向新数组下一个空闲的位置
    auto elem = elements; // 指向旧数组中的下一个位置
    for(size_t i = 0; i != size(); ++i) {
        alloc.construct(dest++, std::move(*elem++));
    }
    free();
    elements = newdata;
    first_free = dest;
    cap = elements + newcapacity;
}

void StrVec::reserve(size_t n)
{
    // 检查capacity
    if(n > capacity()) {
        auto newdata = alloc.allocate(n);
        auto dest = newdata;
        auto elem = elements;
        for(size_t i = 0; i != size(); ++i) {
            alloc.construct(dest++, std::move(*elem++));
        }
        free();
        elements = newdata;
        first_free = dest;
        cap = elements + n;
    }
}

void StrVec::resize(size_t n, const string& s = "")
{
    if(n == size()) return;
    if(n < size()) {
        auto elem = elements + n;
        //    alloc.deallocate(elem, first_free - elem);
        while(first_free != elem) {
            alloc.destroy(--first_free);
        }
    } else if(n > size() && n < capacity()) {
        while(first_free < first_free + (n - size())) {
            alloc.construct(first_free++, s);
        }
    } else {
        reserve(n);
        while(first_free != cap) {
            alloc.construct(first_free++, s);
        }
        check_n_alloc(); // handle the n == capacity() case.
    }
}

int main()
{
    std::cout << "list initialize.\n";
    StrVec str_vec{"Wang", "Cheng", "lin"};
    str_vec.print();
    std::cout << "reserve(10)\n";
    str_vec.reserve(10);
    str_vec.print();
    std::cout << "resize(10, \"rep\")\n";
    str_vec.resize(10, "rep");
    str_vec.print();
    std::cout << "resize(5)\n";
    str_vec.resize(5);
    str_vec.print();

    return 0;
}
