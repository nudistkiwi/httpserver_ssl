




namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
//using json = nlohmann::json;
namespace ssl = boost::asio::ssl;
//using request_body_t = boost::beast::http::string_body;

static std::string mime_types(std::string path)
{

    if (path.find(".htm") != std::string::npos)  return "text/html";
    if (path.find(".html") != std::string::npos) return "text/html";
    if (path.find(".php") != std::string::npos)  return "text/html";
    if (path.find(".css") != std::string::npos)  return "text/css";
    if (path.find(".txt") != std::string::npos)  return "text/plain";
    if (path.find(".json") != std::string::npos) return "application/json";
    if (path.find(".js") != std::string::npos)   return "application/javascript";
    if (path.find(".xml") != std::string::npos)  return "application/xml";
    if (path.find(".swf") != std::string::npos)  return "application/x-shockwave-flash";
    if (path.find(".flv") != std::string::npos)  return "video/x-flv";
    if (path.find(".png") != std::string::npos)  return "image/png";
    if (path.find(".jpeg") != std::string::npos) return "image/jpeg";
    if (path.find(".jpe") != std::string::npos)  return "image/jpeg";
    if (path.find(".jpg") != std::string::npos)  return "image/jpeg";
    if (path.find(".gif") != std::string::npos)  return "image/gif";
    if (path.find(".bmp") != std::string::npos)  return "image/bmp";
    if (path.find(".ico") != std::string::npos)  return "image/vnd.microsoft.icon";
    if (path.find(".tiff") != std::string::npos) return "image/tiff";
    if (path.find(".tif") != std::string::npos)  return "image/tiff";
    if (path.find(".svgz") != std::string::npos) return "image/svg+xml";
    if (path.find(".svg") != std::string::npos)  return "image/svg+xml";
    return "application/text";
}




class https_client_request {


    std::string const host;
    std::string const port;
    std::string const target;

    // The io_context is required for all I/O
    boost::asio::io_context ioc;
    // The SSL context is required, and holds certificates
    ssl::context ctx{ ssl::context::sslv23_client };
    //ssl::context ctx{ ssl::context::sslv23_client };
    tcp::resolver resolver{ ioc };
    ssl::stream<tcp::socket> stream{ ioc, ctx };
    boost::beast::tcp_stream http_stream{ ioc };

    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;

    http::request<http::string_body> req;

public:



    std::string response_code() {
        return std::string(res.reason());

    };

    https_client_request::https_client_request(std::string type, std::string target) {
        /*

        */

        int version = 11;

        if (type == "POST") {
            req = http::request<http::string_body>{ http::verb::post, target.c_str(), version };
        }
        if (type == "GET") {
            req = http::request<http::string_body>{ http::verb::get, target.c_str(), version };
        }


        req.set(http::field::content_type, mime_types(target));

    };



    void connect(std::string host, std::string port) {
        // Look up the domain name




        auto const results = resolver.resolve(host, port);
        // Make the connection on the IP address we get from a lookup

        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        req.prepare_payload();
        req.keep_alive(true);
        req.set(http::field::connection, "keep-alive");
        if (port == "443") {
            if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
            {
                boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
                throw boost::system::system_error{ ec };
            }


            boost::asio::connect(stream.next_layer(), results.begin(), results.end());
            // Perform the SSL handshake
            stream.handshake(ssl::stream_base::client);
        }
        else { http_stream.connect(results); }
    };

    void send_request(std::string host, std::string port) {
        connect(host, port);

        // std::cout << req << std::endl;
        if (port == "443") {
            http::write(stream, req);
            http::read(stream, buffer, res);
            //std::cout << res << std::endl;
                // Gracefully close the stream
                //std::cout<<get_ip()<<std::endl;
            boost::system::error_code ec;
            stream.shutdown(ec);
        }
        else {
            //std::cout << req << std::endl;
            http::write(http_stream, req);
            http::read(http_stream, buffer, res);
            //std::cout << res << std::endl;
                // Gracefully close the stream
                //std::cout<<"HTTP"<<std::endl;
            boost::system::error_code ec;
            http_stream.socket().shutdown(tcp::socket::shutdown_both, ec);


        }


        /*
        if (ec == boost::asio::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec.assign(0, ec.category());
        }
       // if (ec)
       //     throw boost::system::system_error{ ec };
    */

    };

    std::string get_ip() { return stream.lowest_layer().remote_endpoint().address().to_string(); };

    template <class T > void set_body(T A) { req.body() = A.dump(); };
    void set_body(std::string A) { req.body() = A; };
    std::string response_body() { return(std::string(res.body())); };


};
