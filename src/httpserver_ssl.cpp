
//test
#include "httpserver_ssl.h"
#include "server_certificate.hpp"

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;   

auto blacklist= std::vector<std::string>{"/database.db","/localhost.pem","/localhost.decrypted.key"};  

std::string check_if_file(const std::string view){
    std::smatch match;
    std::regex r("(.+); boundary=(-+.*)");
	//std::regex r("multipart/form-data");
	std::string subject =view;
    std::regex_search(subject, match, r);
    std::cout<<match.size()<<std::endl;
    
    if(match.size()==3){
    

    return(match[2]);

   }
   
return("empty");


}

void parse_write_file(const std::string& body)
{



 
  std::size_t found= body.find("\n");
  found= body.find("\n",found+1);
  found= body.find("\n",found+1);
  std::string boundary;
  std::string filename;
    

  std::string header=std::string(body.begin(),body.begin()+found);
  
  std::cout << header << std::endl;
  
  if(true){
	std::smatch match;
	std::regex r("[-]+.+");
	std::string subject =header;
    std::regex_search(subject, match, r);
  boundary=match[0];
  }
  
  std::cout <<boundary << std::endl;

  if(true){
	std::smatch match;
	std::regex r("filename=\"(.*)\"");
	std::string subject =header;
    std::regex_search(subject, match, r);
  filename=match[1];
  }
  
  std::cout << filename << std::endl;
  
  boundary=boundary+"--";
  
  found= body.find("\n",found+1);
  auto pos1=found; 
  auto pos2=body.find(boundary,found);
  //std::cout << pos1 << "  " << pos2 << std::endl;
  std::string new_body=std::string(body.begin()+pos1+1, body.begin()+pos2-2);
 // std::cout << new_body << std::endl;
 filename="downloads/"+filename;
  std::ofstream stream(filename.c_str(), std::ios::binary);
   stream <<new_body;
   stream.close();
}


// Return a reasonable mime type based on the extension of a file.
beast::string_view
mime_type(beast::string_view path)
{
    using beast::iequals;
    auto const ext = [&path]
    {
        auto const pos = path.rfind(".");
        if(pos == beast::string_view::npos)
            return beast::string_view{};
        return path.substr(pos);
    }();
    if(iequals(ext, ".htm"))  return "text/html";
    if(iequals(ext, ".html")) return "text/html";
    if(iequals(ext, ".php"))  return "text/html";
    if(iequals(ext, ".css"))  return "text/css";
    if(iequals(ext, ".txt"))  return "text/plain";
    if(iequals(ext, ".js"))   return "application/javascript";
    if(iequals(ext, ".json")) return "application/json";
    if(iequals(ext, ".xml"))  return "application/xml";
    if(iequals(ext, ".swf"))  return "application/x-shockwave-flash";
    if(iequals(ext, ".flv"))  return "video/x-flv";
    if(iequals(ext, ".png"))  return "image/png";
    if(iequals(ext, ".jpe"))  return "image/jpeg";
    if(iequals(ext, ".jpeg")) return "image/jpeg";
    if(iequals(ext, ".jpg"))  return "image/jpeg";
    if(iequals(ext, ".gif"))  return "image/gif";
    if(iequals(ext, ".bmp"))  return "image/bmp";
    if(iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
    if(iequals(ext, ".tiff")) return "image/tiff";
    if(iequals(ext, ".tif"))  return "image/tiff";
    if(iequals(ext, ".svg"))  return "image/svg+xml";
    if(iequals(ext, ".svgz")) return "image/svg+xml";
    return "application/text";
}



// Append an HTTP rel-path to a local filesystem path.
// The returned path is normalized for the platform.
std::string
path_cat(
    beast::string_view base,
    beast::string_view path)
{
    if(base.empty())
        return std::string(path);
    std::string result(base);
#ifdef BOOST_MSVC
    char constexpr path_separator = '\\';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
    for(auto& c : result)
        if(c == '/')
            c = path_separator;
#else
    char constexpr path_separator = '/';
    if(result.back() == path_separator)
        result.resize(result.size() - 1);
    result.append(path.data(), path.size());
#endif
    return result;
}
// Report a failure


//'''''''''''''''''''''''''''''''''''''''''''''''''''''''

template<
    class Body, class Allocator,
    class Send, typename callback>
void
handle_request(
    beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send,callback function)
{
    //char split_char = '&';
    //std::istringstream split(req.body());
    //auto head = req.header();
    //for (auto it = req.begin(); it != req.end();it++) { }
    //std::cout << std::string(req.base()) << std::endl; // DOES NOT WORK ON LINUX
    //std::cout << req.get() << std::endl;
    //try{

   // }
   // catch(.){}
    
    //req.get(http::field::content_type);
    //req.at("content_type");
    //std::string test = req["content_type"];
   // now use `tokens`
    //std::cout<<req.body()<<std::endl;
   // for(auto it:tokens)
    // Returns a bad request response
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

  // Respond to POST request
    //if(false){
    if (req.method() == http::verb::post) {// function(req.body()); }
    //if (false) {
    //if( req.method() == http::verb::post){


        auto test = std::string(req.at(http::field::content_type));
        // std::cout<<test<<std::endl;
     //check_if_file(test);
        std::string out_path;
        auto cp = check_if_file(test);
        if (cp != "empty") {//std::cout<<check_if_file(test)<<std::endl;
            parse_write_file(req.body());
            out_path = "test.json";
        }

        else {
             out_path = function(req.body());
        }
            beast::error_code ecc;
            http::file_body::value_type p_body;


            p_body.open(out_path.c_str(), beast::file_mode::scan, ecc);

            // Handle the case where the file doesn't exist
            if (ecc == beast::errc::no_such_file_or_directory)
                return send(not_found(req.target()));

            http::response<http::file_body> res{
                std::piecewise_construct,
                std::make_tuple(std::move(p_body)),
                std::make_tuple(http::status::ok, req.version()) };
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(out_path));
            // res.content_length(size);
            res.keep_alive(req.keep_alive());
            return send(std::move(res));


        
    }

    // Make sure we can handle the method
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head)
        return send(bad_request("Unknown HTTP-method"));

    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return send(bad_request("Illegal request-target"));

    for(auto it:blacklist){

if(it==req.target())return send(bad_request("Illegal request-target"));

    }


    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/')
        path.append("index.html");

    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);

    // Handle the case where the file doesn't exist
    if(ec == beast::errc::no_such_file_or_directory)
        return send(not_found(req.target()));

    // Handle an unknown error
    if(ec)
        return send(server_error(ec.message()));

    // Cache the size since we need it after the move
    auto const size = body.size();

    // Respond to HEAD request
    if(req.method() == http::verb::head)
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

//============================================================================


// Report a failure
void
fail(beast::error_code ec, char const* what)
{
    // ssl::error::stream_truncated, also known as an SSL "short read",
    // indicates the peer closed the connection without performing the
    // required closing handshake (for example, Google does this to
    // improve performance). Generally this can be a security issue,
    // but if your communication protocol is self-terminated (as
    // it is with both HTTP and WebSocket) then you may simply
    // ignore the lack of close_notify.
    //
    // https://github.com/boostorg/beast/issues/38
    //
    // https://security.stackexchange.com/questions/91435/how-to-handle-a-malicious-ssl-tls-shutdown
    //
    // When a short read would cut off the end of an HTTP message,
    // Beast returns the error beast::http::error::partial_message.
    // Therefore, if we see a short read here, it has occurred
    // after the message has been completed, so it is safe to ignore it.

    if(ec == net::ssl::error::stream_truncated)
        return;

    std::cerr << what << ": " << ec.message() << "\n";
}


class session : public std::enable_shared_from_this<session>
{
    // This is the C++11 equivalent of a generic lambda.
    // The function object is used to send an HTTP message.
    struct send_lambda
    {
        session& self_;

        explicit
            send_lambda(session& self)
            : self_(self)
        {
        }

        template<bool isRequest, class Body, class Fields>
        void
            operator()(http::message<isRequest, Body, Fields>&& msg) const
        {
            // The lifetime of the message has to extend
            // for the duration of the async operation so
            // we use a shared_ptr to manage it.
            auto sp = std::make_shared<
                http::message<isRequest, Body, Fields>>(std::move(msg));

            // Store a type-erased version of the shared
            // pointer in the class to keep it alive.
            self_.res_ = sp;

            // Write the response
            http::async_write(
                self_.stream_,
                *sp,
                beast::bind_front_handler(
                    &session::on_write,
                    self_.shared_from_this(),
                    sp->need_eof()));
        }
    };

   //1 beast::tcp_stream stream_;
    beast::ssl_stream<beast::tcp_stream> stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;
    std::function<std::string(std::string)> function_;
    boost::optional<http::request_parser<http::string_body>> parser_;
    //   template <typename func>
     //  func function_;    

public:
    // Take ownership of the stream
    template <typename call>
    session(
        tcp::socket&& socket,
       ssl::context& ctx,
        std::shared_ptr<std::string const> const& doc_root, call function
    )
        : stream_(std::move(socket), ctx)
        , doc_root_(doc_root)
        , lambda_(*this)
        , function_(function)
    {
    }

    // Start the asynchronous operation
    //template <typename call>
    void
        run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
       //std::cout<<function_(std::vector<std::string>{"Test2"})<<std::endl;
        net::dispatch(stream_.get_executor(),
            beast::bind_front_handler(
                //2&session::do_read,
                &session::on_run,
                shared_from_this()//,function
            ));
    }
    //template <typename calls>
    
    
    
    void
    on_run()
    {
        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(
            std::chrono::seconds(30));

        // Perform the SSL handshake
        stream_.async_handshake(
            ssl::stream_base::server,
            beast::bind_front_handler(
                &session::on_handshake,
                shared_from_this()));
    }

    void
    on_handshake(beast::error_code ec)
    {
        if(ec)
            return fail(ec, "handshake");

        do_read();
    }

    
    
    
    void
        do_read(
            // calls function
        )
    {


        parser_.emplace();

        // Apply a reasonable limit to the allowed size
        // of the body in bytes to prevent abuse.
        //parser_->body_limit(500000000);
        parser_->body_limit(std::numeric_limits<std::uint64_t>::max());
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        req_ = {};

        // Set the timeout.
        //3stream_.expires_after(std::chrono::seconds(30));
         beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
        // Read a request
        http::async_read(stream_, buffer_, *parser_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }
    //template <typename func>
 
 
    void
        on_read(
            beast::error_code ec,
            //std::size_t bytes_transferred, func Function)
            std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        // This means they closed the connection
        if (ec == http::error::end_of_stream)
            return do_close();

        if (ec)
            return fail(ec, "read");

        // Send the response

       // handle_request(*doc_root_, std::move(req_), lambda_,[](std::vector<std::string>){return(std::string("<!DOCTYPE html><html><body><h1>My Callback</h1><p>My first callback.</p></body></html>"));});
        handle_request(*doc_root_,parser_->release(), lambda_, function_);


    }

    void
        on_write(
            bool close,
            beast::error_code ec,
            std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec)
            return fail(ec, "write");

        if (close)
        {
            // This means we should close the connection, usually because
            // the response indicated the "Connection: close" semantic.
            return do_close();
        }

        // We're done with the response so delete it
        res_ = nullptr;

        // Read another request
        do_read();
    }

    void
        do_close()
    {

        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));

        // Send a TCP shutdown
       //4 beast::error_code ec;
        //5stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
        stream_.async_shutdown(
            beast::bind_front_handler(
                &session::on_shutdown,
                shared_from_this()));


        // At this point the connection is closed gracefully
    }
    void
    on_shutdown(beast::error_code ec)
    {
        if(ec)
            return fail(ec, "shutdown");

        // At this point the connection is closed gracefully
    }


};



class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    ssl::context& ctx_;
    tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;
    std::function<std::string(std::string)> function_;

public:
    template <typename call>
    listener(
        net::io_context& ioc,
        ssl::context& ctx,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root
        , call function
    )
        : ioc_(ioc)
        , ctx_(ctx)
        , acceptor_(ioc)
        , doc_root_(doc_root)
        , function_(function)


    {
        beast::error_code ec;
        // std::cout<<function(std::vector<std::string>{"Test"})<<std::endl;
         // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if (ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if (ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    // Start accepting incoming connections
    void
        run()
    {
        do_accept();
    }

private:
    void
        do_accept()
    {
        // The new connection gets its own strand
        acceptor_.async_accept(
            net::make_strand(ioc_),
            beast::bind_front_handler(
                &listener::on_accept,
                shared_from_this()));
    }



    void
        on_accept(beast::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(
                std::move(socket),
                ctx_,
                doc_root_, function_)->run();
        }

        // Accept another connection
        do_accept();
    }
};




void httpserver_ssl(std::function<std::string(std::string)> func) {

    char* ipaddress = "0.0.0.0";
    char* ports = "8080";
    char* filelocation = ".";
    char* threadnum = "1";
    /*
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));
    */
    auto const address = net::ip::make_address(ipaddress);
    auto const port = static_cast<unsigned short>(std::atoi(ports));
    auto const doc_root = std::make_shared<std::string>(filelocation);
    auto const threads = std::max<int>(1, std::atoi(threadnum));
    // The io_context is required for all I/O
    net::io_context ioc{ threads };
    ssl::context ctx{ssl::context::tlsv12};
    load_server_certificate(ctx);
    // Create and launch a listening port
    //std::function<std::string(std::vector<std::string>)> func=[](std::vector<std::string> A){return(A[0]);};
      
    std::make_shared<listener>(
        ioc,
        ctx,
        tcp::endpoint{ address, port },
        doc_root,
        //[](std::vector<std::string> A){return(A[0]);}
        func
        )->run();
    ioc.run();

};



