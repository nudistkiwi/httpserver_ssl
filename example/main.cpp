
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

struct keymanager{
std::string passcode="skfhdfd";
std::string vaultname="database.db";

};


std::string callback(http::request<http::string_body>& A,void* B) {

keyvault* hallo = (keyvault*) B;
//std::cout<<hallo->passcode<<std::endl;
//https_client_request req2("GET", "/v1/models");
//req2.send_request("api.openai.com", "443");
hallo->data();
std::string token=hallo->operator()("OpenAI");
https_client_request req2("POST", "/v1/chat/completions");
req2.req.set(http::field::authorization,token);
//req2.req.set(http::field::content_type,"application/json");
//req2.req.set(http::field::content_length,"109");
json JS;
json JS2;
JS2["role"]="user";
JS2["content"]="Whats the newest Movie you know in your Training Data?";
std::vector<json> Ax;
Ax.push_back(JS2);

    JS["model"] = "gpt-3.5-turbo";
    JS["messages"] = Ax;
    JS["temperature"] = 0.7;
    req2.set_body(JS);


//req2.req.body()="{\"messages\": [{\"role\": \"user\", \"content\": \"Say this is a test!\"}],\"model\":\"gpt-3.5-turbo\", \"temperature\": 0.7}";
//req2.req.set(http::field::content_length,"109");
std::cout<<req2.req<<std::endl;
req2.send_request("api.openai.com", "443");
//req2.req.set(http::)
//req2.req.set(http::field::au

std::cout<<req2.res.body()<<std::endl;

/*
    std::cout << "POST REQUEST  " << A << std::endl;
    https_client_request req2("POST", "/1/messages.json");
    //https_client_request req("GET","/monitor/?line=14A&station=Keplerplatz&towards=Reumannplatz&countdown");
    json JS;
    //	dataframe hallo;
    //	hallo.write_file(A);

    JS["user"] = "u25eipbejdp986qk5cv6d2d9kt8x4d";
    JS["message"] = "HAALLOOO";
    JS["token"] = "ahh7x5h32p2upqohfny3s5ndyqtw25";



    req2.set_body(JS);
    //req.send_request("vtapi.floscodes.net","443");
    req2.send_request("api.pushover.net", "443");

*/
    std::cout<<"CCAALLBACK"<<std::endl;
    return("/success.json");
};


int main(//int argc, char* argv[]
)
{
    //   OpenSSL_add_all_algorithms();
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
/*
    sqlite3 *DB;
    sqlite3_stmt *stmt = 0;
   auto exit = sqlite3_open("server.db", &DB);
    dataframe A(0,1);
    A.insert("value");
    A.insert("/bettercallsaul123");
    A.insert("/bettercallsaul1234");
    A.insert("/bettercallsaul12345");
    A.insert("/bettercallsaul123456");
    A.write_sqlite(DB,stmt,"tickets");

    dataframe B(0,2);
    B.insert("username");
    B.insert("password");
    B.insert("userzero");
    B.insert("abc314");

    B.write_sqlite(DB,stmt,"users",std::vector<int>{1});


    dataframe C(0,1);
    C.insert("file");
    
    C.insert("/style.css");
    C.insert("/login.html");
    C.insert("/script.js");
    C.insert("/script2.js");
    //C.insert("/");
    //C.insert("/index.html");

    C.write_sqlite("server.db","whitelisted",std::vector<int>{1});
*/
    //Bx.search_sqlite(DB,stmt,"SELECT * from tickets;");
     //std::function<std::string(std::vector<std::string>)> func=callback;

 //OpenSSL_add_all_algorithms();
    keyvault *adsd =new keyvault; ;
    //std::shared_ptr<keymanager> hs;
    std::function<std::string(http::request<http::string_body>&,void*)> func=callback;
    server_configuration funcs(func,"8080");
    



    //std::string ciphertext = "T0TsiTwtKeNjYgiM7x1yWkQv4bGFL4gPPaJ6pnZU6H0=";
    //std::string key = "my_secret_key_1234";
std::string as;

std::cout<<"Password:"<<std::endl;
std::cin>>as;

  //test.write_sqlite("server.db","userzero",std::vector<std::string>{"TEXT","BLOB"},std::vector<int>{1});
  
//  keyvault first;
//  first.insert("admin",as);
 keyvault vault("server.db","admin",as); 
 //vault.insert_new("userzero","qwertzuiop"); 
 vault.data();

  //vault.insert_new("userzero","asdfghjkl"); 
 //std::cout<<vault("userzero")<<std::endl;
  //std::cout<<vault("hello")<<std::endl;
    
    funcs.trezor=vault;
    funcs.obj=&funcs.trezor;

    //query.print();
    //std::string ass=query(2,2);

    //std::cout<<query(2,2)<<std::endl;
    //if(query(2,2)==as) std::cout<<"ALL GOOD"<<std::endl;
    //else std::cout<<"WTF"<<std::endl;
   
    //keymanager* test=hs;
    httpserver_ssl(funcs);
    //std::cout<<"sdfksdgfjhsdfgjdsfhgsd"<<std::endl;
    //std::cin>>as;
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