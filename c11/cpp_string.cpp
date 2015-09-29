#include <iostream>
#include <string>
#include <cctype>
#include <sstream>

using namespace std;


int main()
{
    string str("some thing");
    for (auto &c : str)
    {
        //cout << toupper(c) << endl;
        //cout << c << endl;
        c = toupper(c);
    }
    const char * s = str.c_str();
    cout << str << endl;
    cout << s << endl;
    istringstream istr(str);
    cout << istr.str() << endl;
    string aaa;
    istr >> aaa;
    cout << aaa << endl;
    cout << str.find_first_of("ANCM") << endl;
    cout << str.substr(str.find_first_of("ANCM")) << endl;
}
