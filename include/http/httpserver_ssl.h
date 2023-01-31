
#include <bundle.h>



namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;

//using request_body_t = boost::beast::http::string_body;



//'''''''''''''''''''''''''''''''''''''''''''''''''''''''

//============================================================================

void
fail(beast::error_code ec, char const* what)
{
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

    beast::tcp_stream stream_;
    beast::flat_buffer buffer_;
    std::shared_ptr<std::string const> doc_root_;
    http::request<http::string_body> req_;
    std::shared_ptr<void> res_;
    send_lambda lambda_;
    bundle function_;
    boost::optional<http::request_parser<http::string_body>> parser_;
 //   template <typename func>
  //  func function_;    

public:
    // Take ownership of the stream
    template <class call>
    session(
        tcp::socket&& socket,
        std::shared_ptr<std::string const> const& doc_root,call function 
        )
        : stream_(std::move(socket))
        , doc_root_(doc_root)
        , lambda_(*this)
        ,function_(function)
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
                          &session::do_read,
                          shared_from_this()//,function
                          ));
    }
    //template <typename calls>
    void
    do_read(
       // calls function
        )
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
       
       
       
        // Construct a new parser for each message
        parser_.emplace();

        // Apply a reasonable limit to the allowed size
        // of the body in bytes to prevent abuse.
        //parser_->body_limit(500000000);
        parser_->body_limit(std::numeric_limits<std::uint64_t>::max());
        
        
        
        req_ = {};

        // Set the timeout.
        stream_.expires_after(std::chrono::seconds(30));

        // Read a request
        http::async_read(stream_, buffer_, 
        //req_,
        *parser_,
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
        if(ec == http::error::end_of_stream)
            return do_close();

        if(ec)
            return fail(ec, "read");

        // Send the response
        
       // handle_request(*doc_root_, std::move(req_), lambda_,[](std::vector<std::string>){return(std::string("<!DOCTYPE html><html><body><h1>My Callback</h1><p>My first callback.</p></body></html>"));});
     //handle_request(*doc_root_, std::move(req_), lambda_,function_);
    std::cout<<stream_.socket().lowest_layer().remote_endpoint().address().to_string()<<std::endl;
    std::string ip=stream_.socket().lowest_layer().remote_endpoint().address().to_string();
    sqlframe logs(0,3);
    logs.insert(std::vector<std::string>{"IP_ADRESS"});
    logs.insert(std::vector<std::string>{ip});
    logs.add_timestamp("TIMESTAMP");
    sqlite3_stmt *stmt = 0;
    sqlite3 *DB;
    auto exit = sqlite3_open("logs.db", &DB);
    logs.write_sqlite(DB,stmt,"Logs");
     handle_request(*doc_root_, parser_->release(), lambda_,function_);
   
    
    }

    void
    on_write(
        bool close,
        beast::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");

        if(close)
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
        // Send a TCP shutdown
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        // At this point the connection is closed gracefully
    }
};



class listener : public std::enable_shared_from_this<listener>
{
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
    std::shared_ptr<std::string const> doc_root_;
    bundle function_;

public:
template <class call>
    listener(
        net::io_context& ioc,
        tcp::endpoint endpoint,
        std::shared_ptr<std::string const> const& doc_root
        ,call function
        )
        : ioc_(ioc)
        , acceptor_(net::make_strand(ioc))
        , doc_root_(doc_root)        
        ,function_(function)


    {
        beast::error_code ec;
       // std::cout<<function(std::vector<std::string>{"Test"})<<std::endl;
        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if(ec)
        {
            fail(ec, "open");
            return;
        }

        // Allow address reuse
        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if(ec)
        {
            fail(ec, "set_option");
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if(ec)
        {
            fail(ec, "bind");
            return;
        }

        // Start listening for connections
        acceptor_.listen(
            net::socket_base::max_listen_connections, ec);
        if(ec)
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
        if(ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(
                std::move(socket),
                doc_root_,function_)->run();
        }

        // Accept another connection
        do_accept();
    }
};



//void start_http_server(std::function<std::string(std::vector<std::string> )> func){
 void httpserver_ssl(bundle func){
    
     char* ipaddress = "0.0.0.0";
    // char* ports = "8080";
     char* filelocation = ".";
     char* threadnum = "16";

     /*
    // bundle func(funcs);
     std::vector<std::string> lines;
     std::string line;
     std::ifstream myfile("settings.ini");
     if (myfile.is_open())
     {
         while (getline(myfile, line))
         {
             lines.push_back(line);
             std::cout << line << '\n';
         }
         myfile.close();

         auto const address = net::ip::make_address(lines[0].c_str());
         auto const port = static_cast<unsigned short>(std::atoi(lines[1].c_str()));
         auto const doc_root = std::make_shared<std::string>(lines[2].c_str());
         auto const threads = std::max<int>(1, std::atoi(lines[3].c_str()));

         // The io_context is required for all I/O
         net::io_context ioc{ threads };

         // Create and launch a listening port
         //std::function<std::string(std::vector<std::string>)> func=[](std::vector<std::string> A){return(A[0]);};

         std::make_shared<listener>(
             ioc,
             tcp::endpoint{ address, port },
             doc_root,
             //[](std::vector<std::string> A){return(A[0]);}
             func
             )->run();
         ioc.run();



     }
     */
    // else {


        // std::cout << "Unable to open file";
         
         auto const address = net::ip::make_address(ipaddress);
         auto const port = static_cast<unsigned short>(std::atoi(func.ports));
         auto const doc_root = std::make_shared<std::string>(filelocation);
         auto const threads = std::max<int>(1, std::atoi(threadnum));

         // The io_context is required for all I/O
         net::io_context ioc{ threads };

         // Create and launch a listening port
         //std::function<std::string(std::vector<std::string>)> func=[](std::vector<std::string> A){return(A[0]);};

         std::make_shared<listener>(
             ioc,
             tcp::endpoint{ address, port },
             doc_root,
             //[](std::vector<std::string> A){return(A[0]);}
             func
             )->run();
         ioc.run();

  //   }
 //char *ipaddress="127.0.0.1";


    /*
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));
   
    auto const address = net::ip::make_address(ipaddress);
    auto const port = static_cast<unsigned short>(std::atoi(ports));
    auto const doc_root = std::make_shared<std::string>(filelocation);
    auto const threads = std::max<int>(1, std::atoi(threadnum));
     */



};
