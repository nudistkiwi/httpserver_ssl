

## Design goals

There are myriads of [JSON](https://json.org) libraries out there, and each may even have its reason to exist. Our class had these design goals:

- **Intuitive syntax**. In languages such as Python, JSON feels like a first class data type. We used all the operator magic of modern C++ to achieve the same feeling in your code. Check out the [examples below](#examples) and you'll know what I mean.

- **Trivial integration**. Our whole code consists of a single header file [`json.hpp`](https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp). That's it. No library, no subproject, no dependencies, no complex build system. The class is written in vanilla C++11. All in all, everything should require no adjustment of your compiler flags or project settings.

- **Serious testing**. Our code is heavily [unit-tested](https://github.com/nlohmann/json/tree/develop/tests/src) and covers [100%](https://coveralls.io/r/nlohmann/json) of the code, including all exceptional behavior. Furthermore, we checked with [Valgrind](https://valgrind.org) and the [Clang Sanitizers](https://clang.llvm.org/docs/index.html) that there are no memory leaks. [Google OSS-Fuzz](https://github.com/google/oss-fuzz/tree/master/projects/json) additionally runs fuzz tests against all parsers 24/7, effectively executing billions of tests so far. To maintain high quality, the project is following the [Core Infrastructure Initiative (CII) best practices](https://bestpractices.coreinfrastructure.org/projects/289).

## Sponsors

You can sponsor this library at [GitHub Sponsors](https://github.com/sponsors/nlohmann).

### :office: Corporate Sponsor

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/9/9e/Codacy-logo-black.svg/320px-Codacy-logo-black.svg.png)](https://github.com/codacy/About)

### :label: Named Sponsors

Thanks everyone!

## Support

## Examples

Here are some examples to give you an idea how to use the class.

Beside the examples below, you may want to:

→ Check the [documentation](https://json.nlohmann.me/)\
→ Browse the [standalone example files](https://github.com/nlohmann/json/tree/develop/docs/examples)

Every API function (documented in the [API Documentation](https://json.nlohmann.me/api/basic_json/)) has a corresponding standalone example file. For example, the [`emplace()`](https://json.nlohmann.me/api/basic_json/emplace/) function has a matching [emplace.cpp](https://github.com/nlohmann/json/blob/develop/docs/examples/emplace.cpp) example file.

### Read JSON from a file

The `json` class provides an API for manipulating a JSON value. To create a `json` object by reading a JSON file:

```cpp
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// ...

std::ifstream f("example.json");
json data = json::parse(f);
```

### Creating `json` objects from JSON literals

Assume you want to create hard-code this literal JSON value in a file, as a `json` object:

```json
{
  "pi": 3.141,
  "happy": true
}
```

There are various options:

```cpp
// Using (raw) string literals and json::parse
json ex1 = json::parse(R"(
  {
    "pi": 3.141,
    "happy": true
  }
)");

// Using user-defined (raw) string literals
using namespace nlohmann::literals;
json ex2 = R"(
  {
    "pi": 3.141,
    "happy": true
  }
)"_json;

// Using initializer lists
json ex3 = {
  {"happy", true},
  {"pi", 3.141},
};
```
