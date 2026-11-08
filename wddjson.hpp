#ifndef WDDJSON_HPP
#define WDDJSON_HPP //"wddjson.hpp"

#include <cstdio>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <iomanip>
//#include <ios>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>
#include <variant>
#include <cstdint>
#include <cstddef>
//#include <cerrno>
//#include <exception>
#include <stdexcept>

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
        static_assert(std::variant_size_v<m_data_t> == 8, "");
    private:
        // TRAE 判据：算术类型且排除 bool（bool 是算术类型！）
        template<typename T>
        using is_number_t = std::integral_constant<bool,
            std::is_arithmetic<T>::value && !std::is_same<T, bool>::value
        >;
        //std::variant<std::nullptr_t> m_data;
        //string_t key;
        m_data_t m_data;
    public:
        static int deflt_indent,max_floats;
        static string_t escape_string(const string_t &s) { // 转义
            string_t result="";
            char unicode_esc[8]="\\u00XX";
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
        // 一个顶九个：char/uchar/short/ushort/int/uint/int64/uint64/float 全覆盖
        template<typename T,
                typename std::enable_if_t<is_number_t<T>::value, int> = 0>
        basic_json(T _other) { // TRAE
            if constexpr (std::is_floating_point<T>::value) {
                this->m_data = number_float_t(_other);
            } else if constexpr (std::is_signed<T>::value) {
                this->m_data = number_integer64_t(_other);
            } else {
                this->m_data = number_unsigned64_t(_other);
            }
        }
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
        // operator= 同样一个顶 N 个类
        template<typename T,
                typename std::enable_if_t<is_number_t<T>::value, int> = 0>
        basic_json &operator=(T _other) {
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
        value_t type() const {
            return value_t(this->m_data.index());
        }

        // 尝试获取内部值
        template<typename T>
        T *get_ptr() {
            return std::get_if<T>(&(this->m_data));
        }
        template<typename T>
        const T *get_ptr() const {
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
        const T &get_ref() const {
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

        // TRAE：const 版下标只能是只读严格访问（等价 at，查不到就抛）——const 承诺不修改，不可能有插入语义
        const basic_json &operator[](const string_t &index) const {
            return this->at(index);
        }
        const basic_json &operator[](const char index[]) const {
            return (*this)[string_t(index)];
        }
        const basic_json &operator[](const size_t &index) const {
            return this->at(index);
        }
        basic_json &operator[](const string_t &index) {
            if (this->is_null()) {
                this->m_data = object_t{};
            }
            object_t *obj=std::get_if<object_t>(&this->m_data);
            if (!(obj)) {
                throw std::runtime_error(
                    "This is NOT an object! (operator[])"
                );
            }
            return (*obj)[index];
        }
        basic_json &operator[](const char index[]) {
            return (*this)[string_t(index)];
        }
        basic_json &operator[](const size_t &index) {
            if (this->is_null()) {
                this->m_data = array_t{};
            }
            array_t *arr=std::get_if<array_t>(&this->m_data);
            if (!(arr)) {
                throw std::runtime_error(
                    "This is not an array! (operator[])"
                );
            }
            if (index >= arr->size()) { // Don't use: j[pow(2, 64) - 1]!!!
                arr->resize(index + 1, basic_json(nullptr));
            }
            return (*arr)[index];
        }
        // TRAE：const 版，照搬非 const 真身（const 自动传播，get_if 得到 const 指针）
        const basic_json *at_ptr(const string_t &index) const {
            const object_t *obj=std::get_if<object_t>(&(this->m_data));
            if (!(obj)) {
                throw std::runtime_error("This is NOT an object! (at_ptr)");
            }
            return (
                (obj->find(index) == obj->end()) ?
                nullptr : &(obj->at(index))
            );
        }
        const basic_json *at_ptr(const char index[]) const {
            return this->at_ptr(string_t(index));
        }
        const basic_json *at_ptr(const size_t &index) const {
            const array_t *arr=std::get_if<array_t>(&(this->m_data));
            if (!(arr)) {
                throw std::runtime_error("This is NOT an array! (at_ptr)");
            }
            return (
                (index < arr->size()) ?
                &(*arr)[index] : nullptr
            );
        }
        basic_json *at_ptr(const string_t &index) {
            /*TRAE 第三层：非 const 版不再重复实现，改为委托 const 真身。
               std::as_const 给 *this 加 const → 走 const 重载 → const_cast 脱掉 const。
               方向必须是"非 const 调 const"，反过来在真正 const 的对象上是 UB。
            object_t *obj=std::get_if<object_t>(&this->m_data);
            if (!(obj)) {
                throw std::runtime_error("This is NOT an object! (at_ptr)");
            }
            return (
                (obj->find(index) == obj->end()) ?
                nullptr : &(obj->at(index))
            );*/
            return const_cast<basic_json *>(
                std::as_const(*this).at_ptr(index)
            );
        }
        basic_json *at_ptr(const char index[]) {
            return this->at_ptr(string_t(index));
        }
        basic_json *at_ptr(const size_t &index) {
            /*TRAE 第三层：委托 const 真身（唯一实现留在下面 const 版）。
            array_t *arr=std::get_if<array_t>(&this->m_data);
            if (!(arr)) {
                //throw std::runtime_error("This is NOT an array! (at)");
                throw std::runtime_error("This is NOT an array! (at_ptr)");
            }
            TRAE 修复：三目条件写反了——越界时反而取元素（UB），合法时反而 nullptr
            return (
                (index >= arr->size()) ?
                &(*arr)[index] : nullptr
            );
            return (
                (index < arr->size()) ?
                &(*arr)[index] : nullptr
            );*/
            return const_cast<basic_json *>(
                std::as_const(*this).at_ptr(index)
            );
        }
        // TRAE：const 版，照搬非 const 真身（map/vector::at 自带 const 重载，返回 const 引用）
        const basic_json &at(const string_t &index) const {
            const object_t *obj=std::get_if<object_t>(&(this->m_data));
            if (!(obj)) {
                throw std::runtime_error("This is NOT an object! (at)");
            }
            if (obj->find(index) == obj->end()) {
                std::stringstream errss;
                errss << "Key \"" << index << "\" not found! ";
                throw std::out_of_range(errss.str());
            }
            return obj->at(index);
        }
        const basic_json &at(const char index[]) const {
            return this->at(string_t(index));
        }
        const basic_json &at(const size_t &index) const {
            const array_t *arr=std::get_if<array_t>(&(this->m_data));
            if (!(arr)) {
                throw std::runtime_error("This is NOT an array! (at)");
            }
            if (index >= arr->size()) {
                std::stringstream errss;
                errss << "Index \"" << index << "\" not found! (at)";
                throw std::out_of_range(errss.str());
            }
            return arr->at(index);
        }
        basic_json &at(const string_t &index) {
            /*TRAE 第三层：委托 const 真身（唯一实现留在下面 const 版）。
            object_t *obj=std::get_if<object_t>(&this->m_data);
            if (!(obj)) {
                throw std::runtime_error("This is NOT an object! (at)");
            }
            if (obj->find(index) == obj->end()) {
                std::stringstream errss;
                errss << "Key \"" << index << "\" not found! ";
                throw std::out_of_range(errss.str());
            }
            return obj->at(index);*/
            return const_cast<basic_json &>(
                std::as_const(*this).at(index)
            );
        }
        basic_json &at(const char index[]) {
            return this->at(string_t(index));
        }
        basic_json &at(const size_t &index) {
            /*TRAE 第三层：委托 const 真身（唯一实现留在下面 const 版）。
            array_t *arr=std::get_if<array_t>(&this->m_data);
            if (!(arr)) {
                throw std::runtime_error("This is NOT an array! (at)");
            }
            if (index >= arr->size()) {
                std::stringstream errss;
                errss << "Index \"" << index << "\" not found! (at)";
                throw std::out_of_range(errss.str());
            }
            return arr->at(index);*/
            return const_cast<basic_json &>(
                std::as_const(*this).at(index)
            );
        }
        boolean_t contains(const string_t &key) const {
            if (this->is_object()) {
                const object_t *obj=std::get_if<object_t>(&(this->m_data));
                return obj->find(key) != obj->end();
            }
            return false;
        }
        boolean_t contains(const char key[]) const {
            return this->contains(string_t(key));
        }
        size_t size() const {
            return std::visit([&](auto &&val) -> size_t{
                using T = std::decay_t<decltype(val)>;
                if constexpr(std::is_same_v<T, array_t>) {
                    const array_t &rval=val;
                    return rval.size();
                }else if constexpr(std::is_same_v<T, object_t>) {
                    const object_t &rval=val;
                    return rval.size();
                }else if constexpr(std::is_same_v<T, string_t>) {
                    const string_t &rval=val;
                    return rval.size();
                }else{
                    return 0;
                }
                //return 0;
            }, this->m_data);
        }
        template<typename T>
        void push_back(const T &val) {
            if (this->is_null()) {
                this->m_data = array_t{};
            }
            if (!(this->is_array())) {
                throw std::runtime_error(
                    "This is NOT an array! (push_back)"
                );
            }
            array_t *arr=std::get_if<array_t>(&(this->m_data));
            arr->push_back(val);
            return;
        }
        template<typename T>
        void push_back(T &&val) {
            if (this->is_null()) {
                this->m_data = array_t{};
            }
            if (!(this->is_array())) {
                throw std::runtime_error(
                    "This is NOT an array! (push_back)"
                );
            }
            array_t *arr=std::get_if<array_t>(&(this->m_data));
            arr->emplace_back(std::forward<T>(val));
            return;
        }
        void erase(const string_t &key) {
            object_t *obj=std::get_if<object_t>(&this->m_data);
            if (!(obj)) {
                throw std::runtime_error("This is NOT an object! (erase)");
            }
            obj->erase(key);
        }
        void erase(const char key[]) {
            this->erase(string_t(key));
        }
        void erase(const size_t &index) {
            array_t *arr=std::get_if<array_t>(&this->m_data);
            if (!(arr)) {
                throw std::runtime_error("This is NOT an array! (erase)");
            }
            /*array_t::iterator it=arr->begin();
            for (size_t i=0;it!=arr->end();i++,it++) {
                if (i == index) {
                    arr->erase(it);
                    break;
                }
            }*/
            if (index >= arr->size()) {
                throw std::out_of_range("Array out of range! (erase)");
            }
            arr->erase(arr->begin() + index);
            return;
        }
        boolean_t empty() const {
            return std::visit([&](auto &&val)->boolean_t{
                using T = std::decay_t<decltype(val)>;
                if constexpr (std::is_same_v<T, null_t>) {
                    return true;
                }else if constexpr (std::is_same_v<T, string_t>) {
                    const string_t &rval=val;
                    return rval.empty();
                }else if constexpr (std::is_same_v<T, array_t>) {
                    const array_t &rval=val;
                    return rval.empty();
                }else if constexpr (std::is_same_v<T, object_t>) {
                    const object_t &rval=val;
                    return rval.empty();
                }else{
                    return false;
                }
                //return false;
            }, this->m_data);
        }

        // TODO: parse

        class iterator;
        iterator begin() const;
        iterator end() const;
        iterator find() const;

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
                    if (result.find(".") == string_t::npos) {
                        result += ".0";
                    }
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
                            out += "\"" + escape_string(key) + "\":" + (
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
