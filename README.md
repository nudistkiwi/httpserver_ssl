

## Design goals

There are myriads oflibraries out there, and each may even have its reason to exist. Our class had these design goals:

- **Intuitive syntax**. 

- **Trivial integration**. 

- **Serious testing**. 

## Sponsors



### :office: Corporate Sponsor


### :label: Named Sponsors

Thanks everyone!

## Support

## Examples

Here are some examples to give you an idea how to use the class.

Beside the examples below, you may want to:

→ Check the
→ Browse the

### Make HTTP/S REQUESTS

The `https_client_request` class provides an API for manipulating a http requests.

```cpp
#include <httpserver_ssl.h>

// Create a POST request on ssl port 443
https_client_request req("POST", "/1/messages.json");
req.send_request("vtapi.floscodes.net","443");
//req.set_body(std::string A);
req.set_body(json A);

// ... Or Create an GET request port 80

https_client_request req("GET", "/1/messages.json");
req.send_request("vtapi.floscodes.net","80");
```

### SET UP SERVER

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
