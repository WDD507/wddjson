# wddjson (WDD's JSON)

一个 header-only 的 C++17 JSON 库，无任何第三方依赖。

## 特性

- 单头文件：把 `wddjson.hpp` 拷进项目即可使用，完全离线可用
- 基于 `std::variant` 的类型安全值模型
- `dump()` 序列化：紧凑模式与缩进美化模式
- 完整的字符串转义（遵循 JSON 的 10 种转义白名单，UTF-8 原样通过）
- `operator<<` 直接输出到流

## 最低要求

**C++17**。编译时需开启 `-std=c++17`（GCC/Clang）或 `/std:c++17`（MSVC）。

如果你的项目必须停留在更旧的标准，推荐使用
[nlohmann/json](https://github.com/nlohmann/json)（支持 C++11）。

## 快速上手

```cpp
#include "wddjson.hpp"
#include <iostream>

int main() {
    using WDDWXY::json;

    json a = 42;
    json s = "hello";
    json arr = json::array_t{ json(1), json(2.5), json("hi"), json(false) };

    std::cout << arr.dump() << std::endl;    // 紧凑输出
    std::cout << arr.dump(2) << std::endl;   // 缩进美化输出
    std::cout << arr << std::endl;           // 等价于 dump(默认缩进)
    return 0;
}
```

输出：

```
[1,2.5,"hi",false]
[
  1,
  2.5,
  "hi",
  false
]
```

## 编译与测试

```bat
g++ -std=c++17 tests/test_json.cpp -o test_json.exe && test_json.exe
```

## 作者
- WDDWXY
- AI
  - TRAE

## 路线图

- [x] 值类型基础：构造、类型判断、取值
- [x] 序列化：`dump()` 紧凑 / 美化、字符串转义
- [ ] 容器访问：`operator[]`、`at`、`contains`、`size`、`push_back`、`erase`
- [ ] 解析器：`parse()`（递归下降，含 `\uXXXX` 解码）
- [ ] 迭代器与 STL 风格接口、`operator==`
- [ ] 增强功能：`merge_patch`、自定义类型转换

## 许可证

MIT<!--待定。-->
