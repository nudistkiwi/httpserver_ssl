


namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>
using json = nlohmann::json;
// using request_body_t = boost::beast::http::string_body;

static const std::string currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

static std::string upload_func_standard(std::string)
{

    return "Upload Complete";
}

static std::string randoms(int n)
{

    srand(time(0)); // seed the random number generator with the current time

    std::string s;

    for (int i = 0; i < n; i++)
    {

        char c = 'a' + rand() % 52; // generate a random lowercase letter

        s.push_back(c);
    }

    return s;
}
// template <typename T = void>

class server_configuration
{
    std::function<std::string(http::request<http::string_body> &, http::response<http::string_body> &)> test;
    std::function<std::string(http::request<http::string_body> &, void *)> post_func;
    std::function<std::string(http::request<http::string_body> &,http::response<http::string_body> &,std::string&, void *)> post_func_ext;
    std::function<std::string(http::request<http::string_body> &, void *)> event_func;
    std::function<std::string(http::request<http::string_body> &, void *)> rpost_func;
    // std::function<std::string(http::request<http::string_body> &)> post_func1;
    std::function<std::string(http::request<http::string_body> &)> get_func;
    std::function<std::string(http::request<http::string_body> &)> auth_func;
    std::function<std::string(std::string)> upload_func = upload_func_standard;

    // std::vector<std::string> credentials = std::vector<std::string>{ "username=ECUser&password= };

public:
    bool open_server;
    int cookie_reset;
    time_t start_time;
    char *ports = "8080";
    std::string configdb;
    std::string cookie_base;
    std::string passkey;
    keyvault trezor;
    void *obj;
    // multiplicator=100000000000000000000;
    std::vector<std::string> cookies = std::vector<std::string>{"FcY6roNeX1nYZiwjJoN3", "vsCmKwbAzXmydPRahVPH", "Wff7ok2VxIfy4Y8QHiPp", "3nMTIevLkOi7lahVSISR", "wFflv26AVYpI3BPkdwk7"};
    std::vector<std::string> blacklist = std::vector<std::string>{"/database.db", "/localhost.pem", "/localhost.decrypted.key"};

    std::string get_key(std::string val)
    {
        dataframe table;
        // table.search_sqlite(configdb,)
    };

    template <class G>
    server_configuration(std::function<std::string(http::request<http::string_body> &, G &X)> post_func_)
    {

        // post_func = post_func_;
        set_server_config();
        // cookie_base=std::to_string(rand()*multiplicator);
    };

    server_configuration(std::function<std::string(http::request<http::string_body> &, void *)> post_func_)
    {

        post_func = post_func_;
        set_server_config();
        // cookie_base=std::to_string(rand()*multiplicator);
    };

    server_configuration(std::function<std::string(http::request<http::string_body> &, void *)> post_func_, char *ports_)
    {

        post_func = post_func_;
        ports = ports_;
        set_server_config();
        // cookie_base=std::to_string(rand()*multiplicator);
    };
    server_configuration(std::function<std::string(http::request<http::string_body> &, void *)> post_func_, std::function<std::string(http::request<http::string_body> &)> get_func_)
    {

        post_func = post_func_;
        get_func = get_func_;
        set_server_config();
        // cookie_base=std::to_string(rand()*multiplicator);
    };

    server_configuration(std::function<std::string(http::request<http::string_body> &,http::response<http::string_body> &,std::string &, void *)> post_func_,char *ports_)
    {

        post_func_ext = post_func_;
        ports = ports_;
        set_server_config();
        // cookie_base=std::to_string(rand()*multiplicator);
    };


    void set_server_config()
    {
        //   sqlite3 *DB;
        // sqlite3_stmt *stmt = 0;
        // auto exit = sqlite3_open("server.db", &DB);
        open_server = true;
        cookie_reset = 3600;
        start_time = std::time(nullptr);
        cookie_base = randoms(100);
        dataframe A(0, 1);
        configdb = "server.db";
        blacklist.push_back("/" + configdb);
        A.insert("value");
        A.write_sqlite("server.db", "tickets");

        dataframe C(0, 1);
        C.insert("file");

        C.insert("/style.css");
        C.insert("/login.html");
        C.insert("/script.js");

        C.write_sqlite(configdb, "whitelisted", std::vector<int>{1});
    }

    std::string post(http::request<http::string_body> &A, void *x) { return (post_func(A, x)); };
    std::string post(http::request<http::string_body> & request,http::response<http::string_body> & response,std::string& output, void * x) { return post_func_ext(request,response,output, x); };
    std::string get(http::request<http::string_body> &A) { return (get_func(A)); };

    void set_test(std::function<std::string(http::request<http::string_body> &, http::response<http::string_body> &)> X) { test = X; };
    // std::string auth(http::request<http::string_body>& req, http::response<http::string_body>& res) {
    // req.at(http::basic_fields::authorize);

    std::string authentication(http::request<http::string_body> &req)
    {

        if (open_server)
            return "OK";

        auto date = currentDateTime();
        time_t time = std::time(nullptr);
        int64_t elapsed = int64_t(time - start_time);
        auto roll = elapsed / cookie_reset;
        std::string pre = std::to_string(start_time + roll);
        std::cout << cookie_base << "  ||  " << pre << std::endl;

        date = date.substr(0, 13);
        std::cout << date << " " << roll << " " << start_time << std::endl;
        // iter=date+iter;

        // iter = std::to_string(boost::hash_value(date + iter));
        cookie_base = std::to_string(boost::hash_value(pre + cookie_base));
        //    }
        //   return "OK";currentDateTime()+

        if (req.method() == http::verb::get)
        {

            std::string tar(req.target());
            if (true)
            {
                // sqlite3 *DB;
                // sqlite3_stmt *stmt = 0;
                dataframe Ax(0, 1);
                dataframe Bx;
                // auto exit = sqlite3_open("server.db", &DB);
                Ax.insert("file");
                Ax.insert(tar);
                // Ax.write_sqlite(configdb, "files", std::vector<int>{1});
            }

            for (auto it : blacklist)
            {
                if (it == tar)
                    return "NOK";
            }

            std::string cookie(req["cookie"]);
            if (cookie == "")
            {
                cookie = std::string(req["authorization"]);
            }

            // for (auto iter : cookies)
            //{
            std::cout << cookie << " " << cookie_base << std::endl;
            // if (iter == cookie) return "OK";
            if (cookie.find(cookie_base) != std::string::npos)
                return "OK";

            if (true)
            {

                sqlite3 *DB2;
                sqlite3_stmt *stmt2 = 0;
                dataframe Ax;
                dataframe Bx;
                auto exit = sqlite3_open(configdb.c_str(), &DB2);
                dataframe check(0, 1);
                check.insert(tar);
                check.search_sqlite(configdb, "whitelisted", std::vector<std::string>{"file"}, std::vector<std::string>{"file"});
                if (check.rows > 1)
                {
                    return "OK";
                }
            }

            dataframe check(0, 1);
            check.insert(tar);
            check.search_sqlite(configdb, "tickets", std::vector<std::string>{"value"}, std::vector<std::string>{"value"});
            if (check.rows > 1)
            {
                sqlite3 *DB;
                sqlite3_stmt *stmt = 0;
                auto exit = sqlite3_open(configdb.c_str(), &DB);
                check.statement(DB, "DELETE FROM tickets WHERE value='" + tar + "';");

                // std::cout<<tickets.size()<<" Available Tickets"<<std::endl;
                return "VALID TICKET";
            }
            else
            {
            }

            return "NOK";
        }
        if (req.method() == http::verb::post)
        {

            std::string cookie(req["cookie"]);
            if (cookie == "")
            {
                cookie = std::string(req["authorization"]);
            }

            std::cout << cookie << " " << cookie_base << std::endl;

            if (cookie.find(cookie_base) != std::string::npos)
                return "OK";

            if (json::accept(req.body()))
            {
                std::cout << "SELECT username FRO" << std::endl;
                std::cout << req.body() << std::endl;
                json j = json::parse(req.body());
                std::string pswd;
                std::string usr;
                if (j.contains("token"))
                {
                    pswd = j["token"];
                }
                if (j.contains("password"))
                {
                    pswd = j["password"];
                }
                if (j.contains("username"))
                {
                    usr = j["username"];
                }

                std::cout << trezor(usr) << " " << pswd << " " << usr << std::endl;

                if (trezor(usr) == pswd && pswd != "")
                {

                    std::cout << trezor(usr) << " " << pswd << " " << usr << std::endl;

                    if (j.contains("new_password") && j.contains("confirm_new_password"))
                    {
                        std::cout << "NEW" << std::endl;
                        if (j["new_password"] == j["confirm_new_password"])
                        {
                            std::cout << "NEW NEEEWW" << std::endl;
                            pswd = j["new_password"];
                            trezor.insert_new(usr, pswd);
                            std::cout << usr << "  " << pswd << std::endl;
                            std::cout << trezor(usr) << std::endl;
                            // dataframe A(0, 2);
                            // A.insert("username");
                            // A.insert("password");
                            // A.insert(usr);
                            // A.insert(pswd);
                            // A.write_sqlite(configdb.c_str(), "users", std::vector<int>{1});
                        }
                        else
                        {
                            return "NOK";
                        }
                    }
                    return "VALID TICKET";
                }
                else
                {
                    return "NOK";
                }
            }

            return "NOK";
        }
    };
};




