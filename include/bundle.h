

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <boost/functional/hash.hpp>
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
#include <ctime>
#include <time.h>

#include <sqlframe.h>

class dataframe: public sqlframe {
public:

    dataframe(std::string &input):baseframe(input){};
    dataframe(char *inputs):baseframe(inputs){};
    dataframe(const std::vector<std::string> A, int B):baseframe(A,B){};
    dataframe():baseframe(){};
    dataframe(int i,int j):baseframe(i,j){};
    dataframe(sqlite3 *DB, sqlite3_stmt *stmt):sqlframe(DB,stmt){};
    dataframe(baseframe rhs){
//dataframe temp;
frame=rhs.frame;
cols=rhs.cols;
rows=rhs.rows;
//return(temp);

    };
//using baseframe::operator=;




/*
    dataframe  operator=(const dataframe& rhs)
{

dataframe temp;
temp.frame=rhs.frame;
temp.cols=rhs.cols;
temp.rows=rhs.rows;
return(temp);

}
*/
    dataframe  operator=(const baseframe& rhs)
{

dataframe temp;
temp.frame=rhs.frame;
temp.cols=rhs.cols;
temp.rows=rhs.rows;
return(temp);

}

//dataframe():sqlframe(){};

private:

};


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;
//using request_body_t = boost::beast::http::string_body;


static const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

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
	/*
	 void hash(){for(auto& iter:cookies){
         auto date=currentDateTime();
        date=date.substr(0,12);
        //iter=date+iter;
        
        std::to_string(boost::hash_value(date+iter));
        }};
        */
	std::string post(http::request<http::string_body>& A) { return(post_func(A)); };
    std::string get(http::request<http::string_body>& A) { return(get_func(A)); };
	
	
	
    void set_test(std::function<std::string(http::request<http::string_body>&, http::response<http::string_body>&)> X) { test = X; };
    //std::string auth(http::request<http::string_body>& req, http::response<http::string_body>& res) { 
        //req.at(http::basic_fields::authorize);

    std::string authentication(http::request<http::string_body>& req) {
        
    auto date=currentDateTime();
    /*
    std::for_each(cookies.begin(),cookies.end(),
    //[](std::string& Ad){A=std::to_string(std::hash(Ad));}
    [date](std::string& Ad){
        auto hj=std::hash(Ad);
        Ad="sdffsd";}
    );    
*/


    for(auto& iter:cookies){
        
        date=date.substr(0,13);
        std::cout<<date<<std::endl;
        //iter=date+iter;
        
        iter=std::to_string(boost::hash_value(date+iter));
        }
     //   return "OK";currentDateTime()+


        if (req.method() == http::verb::get) {
            std::string tar(req.target());


            for (auto it : blacklist) {
                if (it == tar) return "NOK";
            }


           // for (auto iter : tickets) {
             //   std::cout << tar << " " << iter<< std::endl;
                      

          //  }


            
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



                        sqlite3 *DB;
                        sqlite3_stmt *stmt = 0;
                        sqlframe Ax;
                        sqlframe Bx;
               auto exit = sqlite3_open("server.db", &DB);
               Bx.search_sqlite(DB,stmt,"SELECT * from tickets;");
               
               Bx.print();
               
            Ax.search_sqlite(DB,stmt,"SELECT value FROM tickets WHERE value='"+tar+"';");
            std::cout<<"SELECT value FROM tickets WHERE value='"+tar+"';"<<std::endl;
               
                if (Ax.rows>0) {  //tickets.pop_back();
               Ax.print();   
                        //Ax.search_sqlite(DB,)

                         Ax.statement(DB,"DELETE FROM tickets WHERE value='"+tar+"';"); 

                //std::cout<<tickets.size()<<" Available Tickets"<<std::endl;
                 return "VALID TICKET";}
                else{
              

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


static std::string check_if_file(const std::string view){
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

static void parse_write_file(const std::string& body)
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
static beast::string_view
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
static std::string
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




template<
    class Body, class Allocator,
    class Send, class callback>
void
handle_request(
    beast::string_view doc_root,
    http::request<Body, http::basic_fields<Allocator>>&& req,
    Send&& send,callback function)
{
    

	    auto const ok_response =
    [&req](beast::string_view why)
    {
        http::response<http::string_body> res{http::status::ok, req.version()};
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        //res.set(http::field::content_type, "text/html");
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
    if( req.method() != http::verb::get &&
        req.method() != http::verb::head && req.method() != http::verb::post)
        return send(bad_request("Unknown HTTP-method"));
	    
	    
    auto auth_response = function.authentication(req);

    if (auth_response == "AUTHENTICATED" || auth_response=="VALID TICKET") { 
        
        auto out_path = path_cat(doc_root, "/index.html");
        //  if (req.target().back() == '/')
          //    path.append("index.html");
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
        //function.hash();
        res.set(http::field::set_cookie, function.cookies[0]);
        // res.content_length(size);
        res.keep_alive(req.keep_alive());
        return send(std::move(res));

        

    }
    
    
    if (auth_response != "OK") { 
        std::cout << "NOT OK REQUEST" << std::endl;
        return send(bad_request("Illegal request- not authorized")); }




  // Respond to POST request

    if (req.method() == http::verb::post) {
 
        auto test = std::string(req.at(http::field::content_type));
        std::string out_path;
        auto cp = check_if_file(test);
        if (cp != "empty") {//std::cout<<check_if_file(test)<<std::endl;
            parse_write_file(req.body());
            return send(ok_response("Upload completed"));
        }
	else {
             out_path = function.post(req);

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
                std::make_tuple(http::status::ok, req.version()) };
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, mime_type(out_path));
            // res.content_length(size);
            res.keep_alive(req.keep_alive());
            return send(std::move(res));

    }



    // Request path must be absolute and not contain "..".
    if( req.target().empty() ||
        req.target()[0] != '/' ||
        req.target().find("..") != beast::string_view::npos)
        return send(bad_request("Illegal request-target"));




    // Build the path to the requested file
    std::string path = path_cat(doc_root, req.target());
    if(req.target().back() == '/')
    path.append("index.html");

    // Attempt to open the file
    beast::error_code ec;
    http::file_body::value_type body;
    body.open(path.c_str(), beast::file_mode::scan, ec);
    std::cout<<path<<std::endl;
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


//void httpserver_ssl(bundle func);
