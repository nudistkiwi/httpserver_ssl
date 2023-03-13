

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
req.set_body(json A);
req.send_request("vtapi.floscodes.net","443");
//req.set_body(std::string A);


// ... Or Create an GET request port 80

https_client_request req("GET", "/1/messages.json");
req.send_request("vtapi.floscodes.net","80");
```

### SET UP SERVER

Assume you want to create hard-code this literal JSON value in a file, as a `json` object:

```cpp
std::string callback(http::request<http::string_body>& A) {
    return("/success.json");
    
    std::function<std::string(http::request<http::string_body>&)> func=callback;
    server_config funcs(func,"8080");
    funcs.open_server=false; //true no authenication server no cookies
    funcs.cookie_reset=3600; //in seconds
    funcs.ports = "8080"; //port number
    httpserver_ssl(funcs);

};

```
