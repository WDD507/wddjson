#ifndef WDDJSON_HPP
#define WDDJSON_HPP //"wddjson.hpp"

#include <cstdio>
#include <iomanip>
//#include <ios>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>
#include <map>
#include <variant>
#include <cstdint>
#include <cstddef>
//#include <cerrno>
//#include <exception>

namespace WDDWXY {
    class basic_json {
    public:
        // 类型别名定义
        using null_t     = std::nullptr_t;
        // TRAE
        using string_t   = std::string;
        using object_t   = std::map<string_t, basic_json>;
        using array_t    = std::vector<basic_json>;
        using boolean_t      = bool;
        using number_integer32_t  = std::int32_t;
        using number_unsigned32_t = std::uint32_t;
        using number_integer64_t  = std::int64_t;
        using number_unsigned64_t = std::uint64_t;
        using number_float_t      = double;
        // ----

        /*union m_data_t {
            null_t null_pointer;
            object_t object;
            array_t array;
            string_t str;
            boolean_t bln;
            number_integer_t numi64;
            number_unsigned_t numui64;
            number_float_t numf64;
        };*/
        using m_data_t = std::variant<null_t, boolean_t, number_integer64_t,
                                      number_unsigned64_t, number_float_t,
                                      string_t, array_t, object_t>;

        // 类型判断枚举
        enum class value_t : int {
            null = 0,
            boolean = 1,
            number_integer = 2,
            number_unsigned = 3,
            number_float = 4,
            string = 5,
            array = 6,
            object_t = 7
        };
    private:
        //std::variant<std::nullptr_t> m_data;
        //string_t key;
        m_data_t m_data;
    public:
        static int deflt_indent,max_floats;
        static string_t escape_string(const string_t &s) { // 转义
            string_t result="";
            static char unicode_esc[1024]="\\u00XX";
            //result += "\"";
            for (const unsigned char &c : s) {
                switch (c) {
                    case '\"':
                        result += "\\\"";
                        break;
                    case '\\':
                        result += "\\\\";
                        break;
                    case '\b':
                        result += "\\b";
                        break;
                    case '\f':
                        result += "\\f";
                        break;
                    case '\n':
                        result += "\\n";
                        break;
                    case '\r':
                        result += "\\r";
                        break;
                    case '\t':
                        result += "\\t";
                        break;
                    default:
                        if (c < 0x20) {
                            sprintf(unicode_esc, "\\u00%02x", (int)c);
                            result += unicode_esc;
                        }else{
                            result += c;
                        }
                        break;
                }
            }
            //result += "\"";
            return result;
        }

        // 构造函数
        //basic_json();
        basic_json(const basic_json &_other) = default;
        basic_json(basic_json &&_other) = default;
        basic_json(const null_t &_other=nullptr) : m_data(_other) {}
        basic_json(const boolean_t &_other) : m_data(_other) {}
        basic_json(const number_integer64_t &_other) : m_data(_other) {}
        basic_json(const number_unsigned64_t &_other) : m_data(_other) {}
        basic_json(const number_float_t &_other) : m_data(_other) {}
        basic_json(const string_t &_other) : m_data(_other) {}
        basic_json(const object_t &_other) : m_data(_other) {}
        basic_json(const array_t &_other) : m_data(_other) {}
        basic_json(char _other) : m_data(number_integer64_t(_other)) {}
        basic_json(unsigned char _other)
            : m_data(number_unsigned64_t(_other)) {}
        basic_json(short _other) : m_data(number_integer64_t(_other)) {}
        basic_json(unsigned short _other)
            : m_data(number_unsigned64_t(_other)) {}
        basic_json(int _other) : m_data(number_integer64_t(_other)) {}
        basic_json(unsigned int _other)
            : m_data(number_unsigned64_t(_other)) {}
        /*basic_json(long long _other) : m_data(number_integer64_t(_other)) {}
        basic_json(unsigned long long _other)
            : m_data(number_unsigned64_t(_other)) {}*/
        basic_json(const char _other[]) : m_data(string_t(_other)) {}
        explicit basic_json(const m_data_t &_other) : m_data(_other) {}

        // 赋值函数
        basic_json &operator=(const basic_json &_other) = default;
        basic_json &operator=(basic_json &&_other) = default;
        basic_json &operator=(const null_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const boolean_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const number_integer64_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const number_unsigned64_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const number_float_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const string_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const object_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const array_t &_other) {
            return (*this) = basic_json(_other);
        }
        basic_json &operator=(const char _other[]) {
            return (*this) = basic_json(_other);
        }

        // 以下这些函数都是判断类型
        bool is_null() const {
            return std::holds_alternative<null_t>(this->m_data);
        }
        bool is_boolean() const {
            return std::holds_alternative<boolean_t>(this->m_data);
        }
        bool is_number_integer() const {
            return std::holds_alternative<number_integer64_t>(this->m_data);
        }
        bool is_number_unsigned() const {
            return std::holds_alternative<number_unsigned64_t>(this->m_data);
        }
        bool is_number_float() const {
            return std::holds_alternative<number_float_t>(this->m_data);
        }
        bool is_number() const {
            return (
                is_number_integer() ||
                is_number_unsigned() ||
                is_number_float()
            );
        }
        bool is_string() const {
            return std::holds_alternative<string_t>(this->m_data);
        }
        bool is_array() const {
            return std::holds_alternative<array_t>(this->m_data);
        }
        bool is_object() const {
            return std::holds_alternative<object_t>(this->m_data);
        }
        value_t type() {
            return value_t(this->m_data.index());
        }

        // 尝试获取内部值
        template<typename T>
        T *get_ptr() {
            return std::get_if<T>(&(this->m_data));
        }
        template<typename T>
        T &get_ref() {
            /*try {
                //
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
                throw e;
            }*/
            /*if (T *p=std::get_if<T>(&(this->m_data))) {
                return *p;
            }else{
                throw ;
            }*/
            return std::get<T>(this->m_data);
        }
        template<typename T>
        T get() const {
            return std::get<T>(this->m_data);
        }

        /*string_t dump(int indent=-1) const {
            string_t result="";
            const bool &have_ind=(indent>=0);
            result += "{";
            std::visit([](auto &&val){
                using T = std::decay_t<decltype(val)>;
            }, this->m_data);
            result += "}";
            return result;
        }*/
        string_t dump(int indent=-1) const { // 整个作为字符串输出。
            return dump_impl(indent, 0);
        }

        basic_json &operator[](const string_t &index);
        basic_json &operator[](const char index[]);
        basic_json &operator[](const size_t &index);
        basic_json *at_ptr(const string_t &index);
        basic_json *at_ptr(const char index[]);
        basic_json *at_ptr(const size_t &index);
        basic_json &at(const string_t &index);
        basic_json &at(const char index[]);
        basic_json &at(const size_t &index);
        boolean_t contains(const string_t &key) const;
        boolean_t contains(const char key[]) const;
        size_t size() const;
        template<typename T>
        void push_back(const T &val);
        void erase(const string_t &key);
        void erase(const char key[]);
        void erase(const size_t &index);

        // TODO: parse

        class iterator;
        iterator begin();
        iterator end();

        friend bool operator==(const basic_json &a, const basic_json &b);

        friend std::istream &operator>>(
            std::istream &inputer, basic_json &j
        );
        friend std::ostream &operator<<(
            std::ostream &outputer, const basic_json &j
        ) {
            outputer << j.dump(deflt_indent);
            return outputer;
        }

    private:
        // For dump()
        string_t dump_impl(int indent=-1, int depth=0) const { // TRAE
            return std::visit([&](auto&& val) -> string_t {
                using T = std::decay_t<decltype(val)>;
                string_t result="";
                if constexpr (std::is_same_v<T, null_t>) {
                    result = "null";
                } else if constexpr (std::is_same_v<T, boolean_t>) {
                    result = (val ? "true" : "false");
                } else if constexpr (std::is_same_v<T, number_integer64_t> ||
                                    std::is_same_v<T, number_unsigned64_t>) {
                    result = std::to_string(val);
                } else if constexpr (std::is_same_v<T, number_float_t>) {
                    std::stringstream ss;
                    ss << std::setprecision(max_floats) << val;
                    result = ss.str();
                } else if constexpr (std::is_same_v<T, string_t>) {
                    result = "\"" + escape_string(val) + "\"";
                } else if constexpr (std::is_same_v<T, array_t>) {
                    const array_t &rval=val;
                    if (rval.empty()) {
                        result = "[]";
                    }else{
                        string_t out = "[";
                        for (size_t i = 0; i < rval.size(); ++i) {
                            if (i) out += ",";
                            if (indent >= 0) {
                                out += "\n";
                                out += string_t(
                                    (depth + 1) * indent, ' '
                                );
                            }
                            out += rval[i].dump_impl(
                                indent, depth + 1
                            ); // 递归！
                        }
                        if (indent >= 0) {
                            out += "\n";
                            out += string_t(
                                depth * indent, ' '
                            );
                        }
                        result = out + "]";
                    }
                } else if constexpr (std::is_same_v<T, object_t>) {
                    const object_t &rval=val;
                    if (rval.empty()) {
                        result = "{}";
                    }else{
                        string_t out = "{";
                        bool first = true;
                        for (auto& [
                            key,
                            value
                        ] : rval) {
                            if (!first) out += ",";
                            first = false;
                            if (indent >= 0) {
                                out += "\n";
                                out += string_t(
                                    (depth + 1) * indent, ' '
                                );
                            }
                            out += "\"" + key + "\":" + (
                                (indent >= 0) ? " " : ""
                            ) + value.dump_impl(
                                indent, depth + 1
                            ); // 递归！
                        }
                        if (indent >= 0) {
                            out += "\n";
                            out += string_t(
                                depth * indent, ' '
                            );
                        }
                        result = out + "}";
                    }
                }else{
                    result = "{}";
                }
                return result;
            }, this->m_data);
        }
    };
    inline int basic_json::deflt_indent=-1;
    inline int basic_json::max_floats=15;
    using json = basic_json;
}

#endif /* WDDJSON_HPP */
