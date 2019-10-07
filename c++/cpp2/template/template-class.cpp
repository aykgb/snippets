#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <initializer_list>

using namespace std;

template <typename T> class Blob {
public:
    typedef T value_type;
    typedef typename std::vector<T>::size_type size_type;

    // Constructor
    Blob(): data(std::make_shared<std::vector<T>>()) { }
    Blob(std::initializer_list<T> il): data(std::make_shared<std::vector<T>>(il)) { }
    // Numbers of class Blob.
    size_type size() const
    {
        return data->size();
    }
    bool empty() const
    {
        return data->empty();
    }
    // Add/Remove element of the class.
    void push_back(const T& t)
    {
        data->push_back();
    }
    void push_back(T&& t)
    {
        data->push_back(std::move(t));
    }
    void pop_back();
    // Accessor
    T& back();
    T& operator[](size_type i);
private:
    std::shared_ptr<std::vector<T>> data;
    void check(size_type i, const std::string& msg) const;
};

template<typename T>
void Blob<T>::check(size_type i, const std::string& msg) const
{
    if(i >= data->size()) {
        throw std::out_of_range(msg);
    }
}

template<typename T>
T& Blob<T>::back()
{
    check(0, "back on empty Blob");
    return data->back();
}

template<typename T>
T& Blob<T>::operator[]( size_type i )
{
    check(i, "subscript out of range");
    return (*data)[i];
}

template<typename T>
void Blob<T>::pop_back()
{
    check(0, "pop_back on empty Blob");
    data->pop_back();
}

int main()
{
    Blob<int> tst{1, 2, 3, 4, 5};
    std::cout << tst.size() << std::endl;

    try {
        std::cout << tst[5] << std::endl;
    } catch (std::exception& e) {
        std::cout << e.what()  << std::endl;
    } catch ( ... ) {
        std::cout << "Something wrong!" << std::endl;
    }

    return 0;
}
