#include "../wddjson.hpp"
#include <iostream>
#include <ostream>
#include <string>
#include <cstddef>
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
    cout << arr.at((size_t)0) << "\n"
         << obj.at("key2").dump(3) << endl;
    json j=nullptr;
    j["name"] = "WDDWXY";
    j["age"] = 20;
    j["height"] = 1.7;
    j["hobbies"].push_back("reading");
    j["hobbies"].push_back("programming");
    j["others"]["said"] = "Hi!";
    j["others"]["data"] = json::object_t{
        {"X", "Y"},
        {"Z", false}
    };
    j["others"]["data"]["Z"] = true;
    json &rj1=j["others"]["data"];
    rj1["A"] = "B";
    rj1["C"] = json::array_t{
        1, 2, 3.0, "A", "B", "C", arr, obj
    };
    rj1["D"] = obj;
    rj1["E"]["F"] = special_s;
    /*json &&rvaluej=json::object_t{{"I", "J"}};
    rj1["E"]["G"]["H"] = (json&&)rvaluej;*/
    cout << j.dump(4) << endl;
    const json &cj1="X",&cj2=j;
    cout << *(cj1.get_ptr<string>()) << "\n"
         << cj2["age"].get_ref<json::number_integer64_t>() << endl;
    json ea = json::array_t{json(1), json(2), json(3)};
    ea.erase((size_t)1);
    cout << ea.dump() << endl;              // 期望 [1,3]
    json eo = json::object_t{{"a\"b", "c\\d"}};
    cout << eo.dump() << endl;              // 期望 {"a\"b":"c\\d"}（键被转义）
    json ei = json::array_t{json(1)};
    ei[(size_t)3] = 9;
    cout << ei.dump() << endl;              // 期望 [1,null,null,9]
    return 0;
}
/* g++ -std=c++17 test_json.cpp -o test_json.exe -lm -O2 && test_json.exe */
