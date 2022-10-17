

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
//#include <boost/tokenizer.hpp>
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
#include <json.hpp>



class bundle {
    std::function<std::string(http::request<http::string_body>&, http::response<http::string_body>&)> test;
    std::function<std::string(http::request<http::string_body>&)> post_func;
    std::function<std::string(http::request<http::string_body>&)> get_func;
    std::function<std::string(http::request<http::string_body>&)> auth_func;
    
    //std::vector<std::string> credentials = std::vector<std::string>{ "username=ECUser&password=x7bhm3Ma" };


public:
    char* ports = "8080";
    std::vector<std::string> cookies = std::vector<std::string>{ "FcY6roNeX1nYZiwjJoN3","vsCmKwbAzXmydPRahVPH","Wff7ok2VxIfy4Y8QHiPp","3nMTIevLkOi7lahVSISR","wFflv26AVYpI3BPkdwk7" };
    std::vector<std::string> credentials = std::vector<std::string>{ "Username=ECUser&Password=GenerationECU","username=ECUser&password=GenerationECU" };
    std::vector<std::string> token = std::vector<std::string>{ "FcY6roNeX1nYZiwjJoN3","vsCmKwbAzXmydPRahVPH","Wff7ok2VxIfy4Y8QHiPp","3nMTIevLkOi7lahVSISR","wFflv26AVYpI3BPkdwk7" };
    std::vector<std::string> tickets = std::vector<std::string>{ "/ticket","/ticket","/ticket","/ticket","/ticket" };

    std::vector<std::string> whitelisted_folders= std::vector<std::string>{ "/vendors","/images","/build" };
    std::vector<std::string> whitelisted_files = std::vector<std::string>{ "/login.html" };
    std::vector<std::string> blacklist = std::vector<std::string>{ "/database.db","/localhost.pem","/localhost.decrypted.key" };
    std::vector<std::string> blacklisted_folders = std::vector<std::string>{};

    bundle(std::function<std::string(http::request<http::string_body>&)> post_func_) { post_func = post_func_; };
    bundle(std::function<std::string(http::request<http::string_body>&)> post_func_, char* ports_) { post_func = post_func_; ports = ports_; };
    bundle(std::function<std::string(http::request<http::string_body>&)> post_func_, std::function<std::string(http::request<http::string_body>&)> get_func_) { post_func = post_func_; get_func = get_func_;
    };
	
	
	std::string post(http::request<http::string_body>& A) { return(post_func(A)); };
    std::string get(http::request<http::string_body>& A) { return(get_func(A)); };
	
	
	
    void set_test(std::function<std::string(http::request<http::string_body>&, http::response<http::string_body>&)> X) { test = X; };
    //std::string auth(http::request<http::string_body>& req, http::response<http::string_body>& res) { 
        //req.at(http::basic_fields::authorize);

    std::string authentication(http::request<http::string_body>& req) {
        

     //   return "OK";


        if (req.method() == http::verb::get) {
            std::string tar(req.target());


            for (auto it : blacklist) {
                if (it == tar) return "NOK";
            }


            for (auto iter : tickets) {
                std::cout << tar << " " << iter<< std::endl;
                if (iter == tar) {  tickets.pop_back();
                std::cout<<tickets.size()<<" Available Tickets"<<std::endl;
                 return "VALID TICKET";}

            }


            
            std::string cookie(req["cookie"]);
            if (cookie == "") { cookie = std::string(req["authorization"]); }

            for (auto iter : cookies) {
                std::cout << cookie << " " << iter<< std::endl;
                //if (iter == cookie) return "OK";
                if (cookie.find(iter)!=std::string::npos) return "OK";

            }
            

            for (auto iter : whitelisted_folders) {
                std::cout << iter << " " << req.target() << std::endl;
                if (iter.size() <= tar.size() && tar.substr(0, iter.size()) == iter) return "OK";

            }

            for (auto iter : whitelisted_files) {
                std::cout << iter << " " << req.target() << std::endl;
                if (iter == tar) return "OK";

            }



            return "NOK";

        }
        if (req.method() == http::verb::post) {
            
            std::string cookie(req["cookie"]);
            if (cookie == "") { cookie = std::string(req["authorization"]); }

            for (auto iter : cookies) {
                std::cout << cookie << " " << iter<< std::endl;
               // if (iter == cookie) return "OK";
                if (cookie.find(iter)!=std::string::npos) return "OK";

            }


            for (auto iter : credentials) {
                std::cout<<iter<<" " << req.body() << std::endl;
                if (iter == req.body()) { 
                    
                req.body() = "/index.html"; 
                      
                return "AUTHENTICATED";
                
                
                }



            }

        
        if(json::accept(req.body())){
        json j = json::parse(req.body());
        std::string pswd;
        if(j.contains("token")){pswd=j["token"];}
        if(j.contains("password")){pswd=j["password"];}

        };



            return "NOK";
        }




    };


};


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
void
fail(beast::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}
