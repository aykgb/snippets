#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

/* 这个类将屏幕的内容看作是一行字符 */
class Window_mgr;
class Screen {
public:
    //  typedef std::string::size_type pos;
    using pos = std::string::size_type;
    /* 指定Window_mgr是Screen的友元类 */
    friend class Window_mgr;
    /* c++ 默认构造函数 */
    Screen() = default;
    /* 使用给定的字符初始化屏幕的内容 */
    Screen(pos ht, pos wd, char c): height(ht), width(wd), contents(ht * wd, c)
    {
        std::cout << "Screen(pos ht, pos wd, char c)" << std::endl;
    }
    /* 使用空白字符初始化屏幕的内容 */
    Screen(pos ht, pos wd): height(ht), width(wd), contents(ht * wd,  ' ')
    {
        std::cout << "Screen(pos ht, pos wd)" << std::endl;
    }

    /* 隐式内联-获取当前光标所在位置的字符 */
    char get() const
    {
        return contents[cursor];
    }
    /* 显式内联-获取特定位置的字符 */
    inline char get(pos ht, pos wd) const;
    Screen &move(pos r, pos c);
    Screen &set(char);
    Screen &set(pos, pos , char);
    /* 根据对象是否是const重载display函数
     * 对于常量对象将只能调用常量函数
     # 对于非常量对象默认调用非常量版本函数，亦可调用常量版本的函数
     */
    Screen &display(std::ostream &os)
    {
        do_display(os);
        return *this;
    }
    const Screen &display(std::ostream &os) const
    {
        do_display(os);
        return *this;
    }
    pos size() const;

private:
    void do_display(std::ostream &os) const
    {
        os << contents << std::endl;
    }

private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
};

inline                      // 可在函数定义处指定inline
Screen &Screen::move(pos r, pos c)
{
    pos row = r * width;      // 计算行的位置
    cursor = row + c;         // 在行内将光标移动到指定的列
    return *this;             // 以左值的形式返回对象
}

char Screen::get(pos r, pos c) const
{
    pos row = r * width;      // 计算行的位置
    return contents[row + c]; // 返回给定位置的字符
}

inline
Screen &Screen::set(char c)
{
    contents[cursor] = c;
    return *this;
}

inline
Screen &Screen::set(pos r, pos c, char ch)
{
    contents[r * width + c] = ch;
    return *this;
}

Screen::pos Screen::size() const
{
    return height * width;
}

class Window_mgr {
public:
    using ScreenIdx = std::vector<Screen>::size_type;
    /* 按照编号将指定的Screen重置为空白 */
    void clear(ScreenIdx);
    ScreenIdx addScreen(const Screen&);

private:
    std::vector<Screen> screens{Screen(24, 80, ' ')};
};

void Window_mgr::clear(ScreenIdx i)
{
    Screen &s = screens[i];
    /* 重置指定的Screen-这里可以直接访问Screen类对象的private成员 */
    s.contents = string(s.height * s.width, ' ');
}

Window_mgr::ScreenIdx
Window_mgr::addScreen(const Screen &s)
{
    screens.push_back(s);
    return screens.size() - 1;
}

int main()
{
    std::cout << "####" << std::endl;
    Screen myscreen(10, 10, '.');
    char ch = myscreen.get();
    std::cout << ch << std::endl;
    ch = myscreen.get(0, 0);
    std::cout << ch << std::endl;
    myscreen.move(4, 0);
    std::cout << myscreen.get(4, 0) << std::endl;
    myscreen.set('#').display(std::cout);

    const Screen blank(5, 3);
    blank.display(std::cout);

    std::cout << "####" << std::endl;
    Screen::pos ht = 24, wd = 80;
    Screen scr(ht, wd, ' ');
    Screen *p = &scr;
    char c = scr.get();
    c = p->get();
    std::cout << c << std::endl;

    std::cout << "####" << std::endl;


    return 0;
}
