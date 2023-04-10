
void read_input(net::io_context& ioc,server_configuration & conf){

std::string line;
std::cout<<"Type INSERT to add User..."<<std::endl;
std::cout<<"Type NEW to add new Account..."<<std::endl;
std::cout<<"Type QUERY to look up Vault..."<<std::endl;
while(std::getline(std::cin,line))
{
    //if(conf.trezor(line)!=""){

        
    //}
//std::cout<<"Received Input:"<<line<<std::endl;

if(line==".CSV"){
    std::cout<<"Give File Name: ";
    std::getline(std::cin,line);
    dataframe test;
    test.open_csv(line);
    test.check();
    int l;
    std::cout<<test.rows<<std::endl;
  

    }
    //test.print();
    //test.





if(line=="NEW"){
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

if(line=="INSERT"){
    std::cout<<"NEW SERVICE:";
    std::getline(std::cin,line);
    auto x=line;
    std::cout<<"Token:";
    std::getline(std::cin,line);
    std::cout<<x<<" "<<line<<std::endl;
    conf.trezor.insert_new(x,line);
    std::cout<<"SUCCESSFULL!!"<<std::endl;
}

if(line=="QUERY"){
    std::getline(std::cin,line);
std::cout<<conf.trezor(line)<<std::endl;
}
//std::cout<<hallo->passcode<<std::endl;
//https_client_request req2("GET", "/v1/models");
if(line=="CHATGPT"){
    std::cout<<"Welcome to GPT3.5-Turbo"<<std::endl;
    std::vector<json> memory;


while(std::getline(std::cin,line) && line!="RESET")
{

std::string token=conf.trezor("OpenAI");
https_client_request req2("POST", "/v1/chat/completions");
req2.req.set(http::field::authorization,token);
//req2.req.set(http::field::content_type,"application/json");
//req2.req.set(http::field::content_length,"109");
json JS;
json JS2;
JS2["role"]="user";
//JS2["content"]="Whats the newest Movie you know in your Training Data?";
JS2["content"]=line;
//std::vector<json> Ax;
memory.push_back(JS2);

    JS["model"] = "gpt-3.5-turbo";
    JS["messages"] = memory;
    //JS["max_tokens"] = 60;
    JS["temperature"] = 0.7;
    req2.set_body(JS);


//req2.req.body()="{\"messages\": [{\"role\": \"user\", \"content\": \"Say this is a test!\"}],\"model\":\"gpt-3.5-turbo\", \"temperature\": 0.7}";
//req2.req.set(http::field::content_length,"109");
//std::cout<<req2.req<<std::endl;
req2.send_request("api.openai.com", "443");
//req2.req.set(http::)
//req2.req.set(http::field::au


json response =json::parse(req2.res.body());
std::string message=response["choices"][0]["message"]["content"].dump();
//std::cout<<response["choices"][0]["message"]["content"].dump()<<std::endl;
//std::cout<<"Next:"<<std::endl;
//std::string rep="infinte God, you piece of shit";
//std::string re="AI language model";

//message.replace(message.find(re),re.size(),rep);
dataframe o(0,1);
o.insert(response.dump());
o.write_csv("ai.json");
std::cout<<message<<std::endl;
memory.push_back(response["choices"][0]["message"]);
}
}
}

ioc.stop();
}
