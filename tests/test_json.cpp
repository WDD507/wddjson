#include "../wddjson.hpp"
#include <iostream>
#include <ostream>
using namespace std;

int main() {
    using WDDWXY::json;
    json a = 42;
    json b = 3.14;
    json c = "hello";
    json d = true;
    json arr = json::array_t{
        json(1),
        json(2.5),
        json("hi"),
        json(false)
    };
    json obj=json::object_t{
        {"key1", "value1"},
        {"key2", 2},
        {"key3", 3.0},
        {"key4", true},
        {"key5", json::array_t{
            arr, json::array_t{}
        }},
        {"key6", json::object_t{}}
    };
    json special_s = "a\"b\\c\nd";
    cout << a.dump() << "\n" << b.dump() << "\n"
              << c.dump() << "\n" << d.dump() << "\n"
              << arr.dump() << "\n" << arr.dump(2) << endl;
    d = false;
    cout << d.dump() << endl;
    cout << obj.dump(4) << "\n"
         << special_s.dump() << endl;
    return 0;
}
/* g++ -std=c++17 test_json.cpp -o test_json.exe -lm -O2 && test_json.exe */
