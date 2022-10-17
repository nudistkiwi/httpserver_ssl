
#include <httpserver_ssl.h>
//#include "functional.h"



std::string option(std::vector<std::string>A ){
std::string temp="";
 for(auto it:A){temp+="<option value=\""+it+"\">"+it+"</option>";}
    return(temp);

};


/*
std::cout<<"0...bulk insert:"<<std::endl;
std::cout<<"1...import file:"<<std::endl;
//std::cout<<"2...import xlsx:"<<std::endl;
std::cout<<"3...scan files:"<<std::endl;
//std::cout<<"4...multi table:"<<std::endl;
std::cout<<"5...expand table:"<<std::endl;
//std::cout<<"6...scan csv file:"<<std::endl;
std::cout<<"7...ingest clipboard"<<std::endl;
std::cout<<"8...search for element:"<<std::endl;
std::cout<<"11...clipboard search:"<<std::endl;
*/
std::string callback(http::request<http::string_body>& A){
   
   


    return(A.body());};

int main(//int argc, char* argv[]
)
{
    // Check command line arguments.
    /*
    if (argc != 5)
    {
        std::cerr <<
            "Usage: http-server-async <address> <port> <doc_root> <threads>\n" <<
            "Example:\n" <<
            "    http-server-async 0.0.0.0 8080 . 1\n";
        return EXIT_FAILURE;
    }
    */

     //std::function<std::string(std::vector<std::string>)> func=callback;
    std::function<std::string(http::request<http::string_body>&)> func=callback;
    bundle funcs(func,"8080");
    httpserver_ssl(funcs);
   /*
    char *ipaddress="127.0.0.1";
    char *ports="9090";
    char *filelocation=".";
    char *threadnum="1";
    /*
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const doc_root = std::make_shared<std::string>(argv[3]);
    auto const threads = std::max<int>(1, std::atoi(argv[4]));
    */
   /*
    auto const address = net::ip::make_address(ipaddress);
    auto const port = static_cast<unsigned short>(std::atoi(ports));
    auto const doc_root = std::make_shared<std::string>(filelocation);
    auto const threads = std::max<int>(1, std::atoi(threadnum));
    // The io_context is required for all I/O
    net::io_context ioc{threads};

    // Create and launch a listening port
    //std::function<std::string(std::vector<std::string>)> func=[](std::vector<std::string> A){return(A[0]);};
   
    std::make_shared<listener>(
        ioc,
        tcp::endpoint{address, port},
        doc_root,
        //[](std::vector<std::string> A){return(A[0]);}
       func
        )->run();
        /*

    // Run the I/O service on the requested number of threads
 /*
    std::vector<std::thread> v;

    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
        [&ioc]
        {
            ioc.run();
        });
*/
//    ioc.run();

    return EXIT_SUCCESS;
}