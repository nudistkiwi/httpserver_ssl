
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


std::string callback(http::request<http::string_body>& A,http::response<http::string_body>& C,std::string & D, void* B) {

    std::cout<<"CCAALLBACK"<<std::endl;
/*
json request3=json::parse(A.body());

std::string xl="/blickgem/fa1/g"+std::string(request3["prompt"])+".pdf";
https_client_request req3("GET", xl);
req3.send_request("www.statistik.at", "443");

req3.dump_file(std::string(request3["prompt"])+".pdf");
*/
//return("/success.json");
     //http::response<http::string_body> res{http::status::ok, req.version()};
     //C.set(http::field::status_uri,http::status::ok);
       C.result(http::status::ok);
       C.version(A.version());
       
        C.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        C.set(http::field::content_type, "text/html");
        C.keep_alive(A.keep_alive());
        C.body() = "WHOOT";
        C.prepare_payload();


//    return("/success.json");


keyvault* hallo = (keyvault*) B;
//std::cout<<hallo->passcode<<std::endl;
//https_client_request req2("GET", "/v1/models");
//req2.send_request("api.openai.com", "443");

hallo->data();
json request=json::parse(A.body());
std::cout<<request.dump()<<std::endl;
std::cout<<request["operation"]<<std::endl;
if(request["operation"]=="chatbot"){
std::cout<<"GPT Start"<<std::endl;
std::string token=hallo->operator()("OpenAI");
//std::string token=conf.trezor("OpenAI");
https_client_request req2("POST", "/v1/chat/completions");
req2.req.set(http::field::authorization,token);
//req2.req.set(http::field::content_type,"application/json");
//req2.req.set(http::field::content_length,"109");
json JS;
//json JS2;
//JS2["role"]="user";
//JS2["content"]="Whats the newest Movie you know in your Training Data?";

//JS2["content"]=line;


//JS2["content"]="dsfdsf";
//std::vector<json> Ax;
//memory.push_back(JS2);

    JS["model"] = "gpt-3.5-turbo";

//JS["messages"] = memory;
JS["messages"]=request["conversation"];

    //JS["max_tokens"] = 60;
    JS["temperature"] = 0.7;
    req2.set_body(JS);


std::cout<<JS.dump()<<std::endl;
req2.send_request("api.openai.com", "443");

json response =json::parse(req2.res.body());
std::string message=response["choices"][0]["message"]["content"].dump();

dataframe o(0,1);
o.insert(response.dump());
o.write_csv("ai.json");
std::cout<<message<<std::endl;

//memory.push_back(response["choices"][0]["message"]);

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
C=req2.res;
}

return("");

};




void entry(){

dataframe testd;
testd.database_tables("server.db");
//testd.print();
if(testd.rows==1){
    std::string line;
std::string db;
    keyvault news;
    std::cout<<"set Database:";
    std::getline(std::cin,db);

    std::cout<<"NEW ACCOUNT:";
    std::getline(std::cin,line);
    auto x=line;
    std::cout<<"Token:";
    std::getline(std::cin,line);
    std::cout<<x<<" "<<line<<std::endl;
    news.new_account(x,line,db);
    std::cout<<"NEW ACCOUNT SUCCESSFULL!!"<<std::endl;
}

}


using namespace boost::asio;


std::vector<unsigned char> readFileToVector(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    
    if (!file)
    {
        // Error handling - Failed to open the file
        // You can throw an exception or handle the error as needed
        // For simplicity, this example terminates the program
        throw std::runtime_error("Failed to open the file.");
    }
    
    // Determine the file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read the file content into a vector
    std::vector<unsigned char> fileContent(fileSize);
    file.read(reinterpret_cast<char*>(fileContent.data()), fileSize);
    
    if (!file)
    {
        // Error handling - Failed to read the file
        // You can throw an exception or handle the error as needed
        // For simplicity, this example terminates the program
        throw std::runtime_error("Failed to read the file.");
    }
    
    return fileContent;
}


int main(//int argc, char* argv[]
)
{

/*
        io_service io_service;
        tcp_server server(io_service, 1234);
        io_service.run();
*/
/*
    keyvault *adsd =new keyvault;
    std::string in;
while(false){
std::cin>>in;
auto x=adsd->encrypt3(in,"x7bhm3Ma");
auto y=adsd->decrypt3(x,"x7bhm3Ma");
std::cout<<y<<std::endl;
}

int k;
    //auto xck=readFileToVector("localhost.decrypted.key");

    dataframe jkl(0,2);
    jkl.insert("account");
    jkl.insert("token");
    jkl.insert("server.key");
    jkl.insert_file("localhost.decrypted.key");
    //std::string str(xck.begin(),xck.end());
   auto str=jkl.frame.back();
   auto xck=str;
   str=adsd->encrypt3(str,"x7bhm3Ma");
    jkl.frame.back()=str;
    
   
    jkl.write_sqlite("server.db","admin",std::vector<std::string>{"TEXT","BLOB"},std::vector<int>{1});
    jkl.frame.clear();
    jkl.search_sqlite("server.db","SELECT \"token\" FROM \"admin\" WHERE \"account\"='server.key';");
    dataframe ol(0,1);
    ol.insert("server.key");
    ol.search_sqlite("server.db","admin",std::vector<std::string>{"TEXT","BLOB"},std::vector<std::string>{"account"},std::vector<std::string>{"token"});
    str=ol.frame.back();
    
    auto xcc=adsd->decrypt3(str,"x7bhm3Ma");
    */
   /*
 
    auto xc=std::vector<unsigned char>(str.begin(), str.end());
    xck=xc;
    //std::vector<unsigned char> xc{'a','d','d','d','f'};
    auto xc1=adsd->encrypt2(xc,"x7bhm3Ma");
    
    str=std::string(xc1.begin(),xc1.end());
    jkl.frame.back()=str;
    jkl.write_sqlite("server.db","admin",std::vector<std::string>{"TEXT","BLOB"},std::vector<int>{1});
    jkl.frame.clear();
    jkl.search_sqlite("server.db","SELECT \"token\" FROM \"admin\" WHERE \"account\"='server.key';");
    dataframe ol(0,1);
    ol.insert("server.key");
    ol.search_sqlite("server.db","admin",std::vector<std::string>{"TEXT","BLOB"},std::vector<std::string>{"account"},std::vector<std::string>{"token"});
    str=ol.frame.back();
    //str=jkl.frame.back();
    std::cout<<xc1.size()<<std::endl;
    xc1=std::vector<unsigned char>(str.begin(), str.end());
    std::cout<<xc1.size()<<std::endl;
    auto xcc=adsd->decrypt2(xc1,"x7bhm3Ma");

    if(xck==xcc){std::cout<<"ALL GODD"<<std::endl;}
    else{std::cout<<"FAAAKKKK"<<std::endl;}
    std::cin>>k;
    */
entry();
/*
    std::string line="gemliste_nam.csv";
    dataframe test;
    test.open_csv(line);
    test.check();
    int l;
    std::cout<<test.rows<<std::endl;
    for(int i=1;i<=test.rows;i++){
    dataframe out(0,1);

    //out.insert(test(i,1));
   
    
   
    out.regex_expand("[0-9][0-9][0-9][0-9][0-9]",1);
   //  out.print();

    dataframe out2(0,1);
    out2.insert(test(i,1));

    out2.regex_expand("([A-Za-zÖöÄäÜüß ]+)",1);
     
    //out.print();
    //out2.print();
    
    }
    */
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

    //Bx.search_sqlite(DB,stmt,"SELECT * from tickets;");
     //std::function<std::string(std::vector<std::string>)> func=callback;

 //OpenSSL_add_all_algorithms();

    //std::shared_ptr<keymanager> hs;
    std::function<std::string(http::request<http::string_body>&,http::response<http::string_body>&,std::string&, void*)> func=callback;
    server_configuration funcs(func,"8080");
    



    //std::string ciphertext = "T0TsiTwtKeNjYgiM7x1yWkQv4bGFL4gPPaJ6pnZU6H0=";
    //std::string key = "my_secret_key_1234";
std::string pass;
std::string user;

std::cout<<" Please type Username:";
std::cin>>user;
std::cout<<"Password:";
std::cin>>pass;

  //test.write_sqlite("server.db","userzero",std::vector<std::string>{"TEXT","BLOB"},std::vector<int>{1});
  
//  keyvault first;
//  first.insert("admin",as);
 keyvault vault("server.db",user,pass); 
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