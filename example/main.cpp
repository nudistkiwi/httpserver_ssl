
#include "httpserver_ssl.h"
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
std::string callback(std::string A){
    std::string temp="";
    std::string op="<form action=\"/action_page.php\" method=\"POST\"><label for=\"Option\">Options found:</label><select id=\"option\" name=\"option\"><option value=\"bulk insert\">bulk insert</option><option value=\"import file\">import file</option><option value=\"scan files\">scan files</option><option value=\"expand table\">expand table</option><option value=\"scan csv file\">scan csv file</option><option value=\"ingest clipboard\">ingest clipboard</option><option value=\"clipboard search\">clipboard search</option></select><input type=\"text\" id=\"link\" name=\"link\" value=\"\"><br><input type=\"submit\" value=\"Submit\"></form>";
    std::string op1="<form action=\"/action_page.php\" method=\"POST\"><label for=\"Option\">Options found:</label><select id=\"option\" name=\"option\">";
    std::string op2="</select><input type=\"submit\" value=\"Submit\"></form>";
std::string query="<form action=\"/\" method=\"POST\"><label for=\"fname\">Qeuery String:</label><br><input type=\"text\" id=\"fname\" name=\"fname\" value=\"\"><br><input type=\"submit\" value=\"Submit\"></form>";
std::string clip="<form action=\"/\" method=\"POST\"><input type=\"submit\" name=\"clip\" value=\"Copy to Clipboard\"></form>";
    std::string first="<style>.dropbtn {background-color: #4CAF50;color: white;padding: 16px;font-size: 16px;border: none;}.dropdown {position: relative;display: inline-block;}.dropdown-content {display: none;position: absolute;background-color: #f1f1f1;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}.dropdown-content a {color: black;padding: 12px 16px;text-decoration: none;display: block;}.dropdown-content a:hover {background-color: #ddd;}.dropdown:hover .dropdown-content {display: block;}.dropdown:hover .dropbtn {background-color: #3e8e41;}</style>";
   std::string box="<form action=\"/action_page.php\" method=\"POST\"><label for=\"fname\">First name:</label><br><input type=\"text\" id=\"fname\" name=\"fname\" value=\"John\"><br><input type=\"submit\" value=\"Submit\"></form>";
   std::cout << "HHTP BODY" << std::endl;
   if(A.size()<1000){
   std::cout << A << std::endl;
   }
    /*
    for(auto it:A){temp+=it+"&#13";//std::cout<<it<<std::endl;
    }*/
   // std::string out="<!DOCTYPE html><html><head>"+first+"<textarea rows=\"30\" cols=\"205\" placeholder=test>"+temp+"</textarea>"+query+op1+option(A)+op2+op+clip+"</body></html>";
  
   int k = 1;
   /*
       for (auto it : frame)
       {
           file.append(it);
           if (k % cols == 0)
           {
               file.append("\n");
           }
           else
           {
               file.append(";");
           }
           k++;
       }
   */

   int N;/*
   const std::string filename = "New Microsoft Excel Worksheet.xlsx";
   std::ifstream input(filename.c_str(), std::ios::binary);
   std::ifstream src;
   std::ofstream dst;
   src.open("New Microsoft Excel Worksheet.xlsx", std::ios::in | std::ios::binary);
   input.seekg(0, input.end);
   N = input.tellg();
   input.seekg(0, input.beg);
   N = N - input.tellg();
   char* data = new char[N];
   input.read(data, N);
   std::string help(data, N);
   delete[] data;
   */
  // std::ofstream stream("testo.xlsx", std::ios::binary);
  // stream <<A;
  // stream.close();
   std::string out="test.json";
   /*
   dst.open("testo2.xlsx", std::ios::out | std::ios::binary);
   dst << src.rdbuf();

   src.close();
   dst.close();
   */
    return(out);};

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
          std::function<std::string(std::string)> func=callback;
   httpserver_ssl(func);
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