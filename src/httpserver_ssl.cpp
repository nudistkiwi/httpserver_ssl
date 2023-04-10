
//test
//#include "httpserver_ssl.h"
//#include <httpserver_ssl.h>
#include "server_certificate.hpp"
#include <bundle.h>
#include <cmdlinefunctions.h>
//#include <boost/beast/ssl.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;   


// Report a failure


//'''''''''''''''''''''''''''''''''''''''''''''''''''''''


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
    server_configuration function_;
    boost::optional<http::request_parser<http::string_body>> parser_;
    //   template <typename func>
     //  func function_;    

public:
    // Take ownership of the stream
    template <class call>
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
        std::string optional;
        handle_request(*doc_root_,parser_->release(), lambda_, function_,optional);


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
    server_configuration function_;

public:
    template <class call>
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





void httpserver_ssl(server_configuration func) {


    if(func.trezor.locked()){
    std::cout<<"KEYVAULT LOCKED: SERVER MAY NOT FUNCTION PROPERERlY"<<std::endl;
    }

    char* ipaddress = "0.0.0.0";
    char* ports = "8080";
    char* filelocation = ".";
    char* threadnum = "4";
    /*
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));
    */
    auto const address = net::ip::make_address(ipaddress);
    auto const port = static_cast<unsigned short>(std::atoi(func.ports));
    auto const doc_root = std::make_shared<std::string>(filelocation);
    auto const threads = std::max<int>(1, std::atoi(threadnum));
    // The io_context is required for all I/O
    net::io_context ioc{ threads };
    ssl::context ctx{ssl::context::tlsv12};
    load_server_certificate(ctx);
    // Create and launch a listening port
    //std::function<std::string(std::vector<std::string>)> func=[](std::vector<std::string> A){return(A[0]);};
      
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    std::make_shared<listener>(
        ioc,
        ctx,
        tcp::endpoint{ address, port },
        doc_root,
        //[](std::vector<std::string> A){return(A[0]);}
        func
        )->run();

 for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });


std::thread input_thread([&ioc,&func]{read_input(ioc,func);});

 std::string ad;
 //std::cin>>ad;
 //ioc.run();


for(auto &t:v){
t.join();

}
input_thread.join();


};



