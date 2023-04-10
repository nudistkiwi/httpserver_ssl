

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/read_until.hpp>
// #include <boost/tokenizer.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <sstream>
#include <fstream>
#include <regex>
// #include <json.hpp>
#include <ctime>
#include <time.h>
#include <client_ssl.h>
#include <dataframe.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <keyvault.h>
#include <server_config.h>



namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;
// using request_body_t = boost::beast::http::string_body;


static std::string check_if_file(const std::string view)
{
    std::smatch match;
    std::regex r("(.+); boundary=(-+.*)");
    // std::regex r("multipart/form-data");
    std::string subject = view;
    std::regex_search(subject, match, r);
    std::cout << match.size() << std::endl;

    if (match.size() == 3)
    {

        return (match[2]);
    }

    return ("empty");
}

static std::string parse_write_file(const std::string &body)
{

    std::size_t found = body.find("\n");
    found = body.find("\n", found + 1);
    found = body.find("\n", found + 1);
    std::string boundary;
    std::string filename;

    std::string header = std::string(body.begin(), body.begin() + found);

    std::cout << header << std::endl;

    if (true)
    {
        std::smatch match;
        std::regex r("[-]+.+");
        std::string subject = header;
        std::regex_search(subject, match, r);
        boundary = match[0];
    }

    std::cout << boundary << std::endl;

    if (true)
    {
        std::smatch match;
        std::regex r("filename=\"(.*)\"");
        std::string subject = header;
        std::regex_search(subject, match, r);
        filename = match[1];
    }

    std::cout << filename << std::endl;

    boundary = boundary + "--";

    found = body.find("\n", found + 1);
    auto pos1 = found;
    auto pos2 = body.find(boundary, found);
    // std::cout << pos1 << "  " << pos2 << std::endl;
    std::string new_body = std::string(body.begin() + pos1 + 1, body.begin() + pos2 - 2);
    // std::cout << new_body << std::endl;
    auto tfilename = "downloads/" + filename;
    std::ofstream stream(tfilename.c_str(), std::ios::binary);
    stream << new_body;
    stream.close();
    return filename;
}

// Return a reasonable mime type based on the extension of a file.
static beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if (pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if (iequals(ext, ".htm"))
        return "text/html";
    if (iequals(ext, ".html"))
        return "text/html";
    if (iequals(ext, ".php"))
        return "text/html";
    if (iequals(ext, ".css"))
        return "text/css";
    if (iequals(ext, ".txt"))
        return "text/plain";
    if (iequals(ext, ".js"))
        return "application/javascript";
    if (iequals(ext, ".json"))
        return "application/json";
    if (iequals(ext, ".xml"))
        return "application/xml";
    if (iequals(ext, ".swf"))
        return "application/x-shockwave-flash";
    if (iequals(ext, ".flv"))
        return "video/x-flv";
    if (iequals(ext, ".png"))
        return "image/png";
    if (iequals(ext, ".jpe"))
        return "image/jpeg";
    if (iequals(ext, ".jpeg"))
        return "image/jpeg";
    if (iequals(ext, ".jpg"))
        return "image/jpeg";
    if (iequals(ext, ".gif"))
        return "image/gif";
    if (iequals(ext, ".bmp"))
        return "image/bmp";
    if (iequals(ext, ".ico"))
        return "image/vnd.microsoft.icon";
    if (iequals(ext, ".tiff"))
        return "image/tiff";
    if (iequals(ext, ".tif"))
        return "image/tiff";
    if (iequals(ext, ".svg"))
        return "image/svg+xml";
    if (iequals(ext, ".svgz"))
        return "image/svg+xml";
    return "application/text";
}

// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
static std::string
path_cat(
    beast::string_view base,
    beast::string_view path)
{
    if (base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for (auto &c : result)
        if (c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if (result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}
// Report a failure

template <
    class Body, class Allocator,
    class Send, class callback>
void handle_request(
    beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>> &&req,
    Send &&send, callback function, std::string &message)
{

    auto const ok_response =
        [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        // res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    auto const bad_request =
        [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::bad_request, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = std::string(why);
        res.prepare_payload();
        return res;
    };

    // Returns a not found response
    auto const not_found =
        [&req](beast::string_view target)
    {
        http::response<http::string_body> res{http::status::not_found, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(target) + "' was not found.";
        res.prepare_payload();
        return res;
    };

    // Returns a Server sent event response
    auto const sse_message =
        [&req](beast::string_view data)
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "\nevent: ping\n\n";                    // Set the event name.
        res.body() += "data: " + std::string(data) + "\n\n"; // Set the event data.
        res.prepare_payload();
        return res;
    };

    // Returns a server error response
    auto const server_error =
        [&req](beast::string_view what)
    {
        http::response<http::string_body> res{http::status::internal_server_error, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "An error occurred: '" + std::string(what) + "'";
        res.prepare_payload();
        return res;
    };

    std::cout << "handle" << std::endl;
    // Make sure we can handle the method
    if (req.method() != http::verb::get &&
        req.method() != http::verb::head && req.method() != http::verb::post)
        return send(bad_request("Unknown HTTP-method"));

    auto auth_response = function.authentication(req);
    // auth_response="OK";
    if (auth_response == "AUTHENTICATED" || auth_response == "VALID TICKET")
    {
        std::cout << auth_response << std::endl;

        std::cout << "DONE" << std::endl;
        /* REDIRECT POST
           http::response<http::empty_body> res{http::status::found, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::location, "/index.html");
            res.set(http::field::set_cookie,function.cookies[0]);
            res.keep_alive(req.keep_alive());
            return send(std::move(res));
          */
        dataframe msg;
        std::string cookie = function.cookie_base;
        msg.insert(std::vector<std::string>{"token", cookie});
        msg.insert(std::vector<std::string>{"redirect", "index.html"});

        std::string out_path = "for_mime.json";
        // http::file_body::value_type p_body=

        http::response<http::string_body> res{

            // std::make_tuple(std::move(p_body)),
            http::status::ok, req.version()};
        res.body() = msg.to_json().dump();
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(out_path));
        res.set(http::field::set_cookie, cookie);
        // res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }

    if (auth_response != "OK")
    {
        std::cout << "NOT OK REQUEST" << std::endl;
        return send(bad_request("Illegal request- not authorized"));
    }

    // Respond to POST request
    try
    {
        if (req.method() == http::verb::post)
        {

            auto test = std::string(req.at(http::field::content_type));
            std::cout << req.at(http::field::content_type) << std::endl;
            std::string out_path;
            auto cp = check_if_file(test);

            /*
            if(req.at(http::field::content_type)=="text/event-stream"){
                 int i=0;
                while(true){
                    i++;
                    std::cout<<"WAZZUP"<<std::endl;
                    message="text/event-stream";
                    return send[std::move(sse_message("PACKAGE "+std::to_string(i)))];

                    sleep(1);

                }
                return;
                //return;
            }*/

            if (cp != "empty")
            { // std::cout<<check_if_file(test)<<std::endl;
                auto filename = parse_write_file(req.body());
                // return(send(upload_func_standard(filename)))
                // return send(ok_response("Upload completed"));
                return send(ok_response("Upload " + filename + " Complete"));
            }
            else
            {
                http::response<http::string_body> response;
                std::string file;
                out_path = function.post(req,response,file, function.obj);
                //out_path = function.post(req, function.obj);
                if(file==""){

                    return send(std::move(response));
                }

                std::cout << out_path << std::endl;
            }

            out_path = path_cat(doc_root, out_path);
            //  if (req.target().back() == '/')
            //    path.append("index.html");
            beast::error_code ecc;
            http::file_body::value_type p_body;
            p_body.open(out_path.c_str(), beast::file_mode::scan, ecc);

            // Handle the case where the file doesn't exist
            if (ecc == beast::errc::no_such_file_or_directory)
                return send(not_found(out_path));

            http::response<http::file_body> res{
                std::piecewise_construct,
                std::make_tuple(std::move(p_body)),
                std::make_tuple(http::status::ok, req.version())};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(out_path));
            // res.content_length(size);
            res.keep_alive(req.keep_alive());
            return send(std::move(res));
        }

        // Request path must be absolute and not contain "..".
        if (req.target().empty() ||
            req.target()[0] != '/' ||
            req.target().find("..") != beast::string_view::npos)
            return send(bad_request("Illegal request-target"));

        // Build the path to the requested file
        std::string path = path_cat(doc_root, req.target());
        if (req.target().back() == '/')
            path.append("index.html");

        // Attempt to open the file
        beast::error_code ec;
        http::file_body::value_type body;
        body.open(path.c_str(), beast::file_mode::scan, ec);
        std::cout << path << std::endl;
        // Handle the case where the file doesn't exist
        if (ec == beast::errc::no_such_file_or_directory)
            return send(not_found(req.target()));
        // Handle an unknown error
        if (ec)
            return send(server_error(ec.message()));
        // Cache the size since we need it after the move
        auto const size = body.size();

        // Respond to HEAD request
        if (req.method() == http::verb::head)
        {
            http::response<http::empty_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            //  res.set(http::field::content_type, mime_type(path));
            res.content_length(size);
            res.keep_alive(req.keep_alive());
            return send(std::move(res));
        }

        // Respond to GET request
        http::response<http::file_body> res{
            std::piecewise_construct,
            std::make_tuple(std::move(body)),
            std::make_tuple(http::status::ok, req.version())};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, mime_type(path));
        res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));
    }
    catch (...)
    {
        auto const server_error =
            [&req](beast::string_view what)
        {
            http::response<http::string_body> res{http::status::internal_server_error, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "text/html");
            res.keep_alive(req.keep_alive());
            res.body() = "An error occurred: '" + std::string(what) + "'";
            res.prepare_payload();
            return res;
        };
    }
};

