#include<iostream>
#include<iterator>
#include<algorithm>
#include<fstream>
#include<string>
#include<vector>

using namespace std;

int main()
{
    //  cout << __FILE__ << "\n";
    //  cout << __FUNCTION__ << "\n";
    istream_iterator<int> is_it(cin), eof;
    vector<int> ivec(is_it, eof);
    //  while(is_it != eof)
    //    ivec.push_back(*is_it++);

    sort(ivec.begin(), ivec.end());

    ostream_iterator<int> os_it(cout, " | ");
    copy(ivec.begin(), ivec.end(), os_it);
    cout << endl;

    return 0;
}
