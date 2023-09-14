
void read_input(net::io_context& ioc,server_configuration & conf){

std::string line;



std::cout<<"Type INSERT to add User..."<<std::endl;
std::cout<<"Type NEW to add new Account..."<<std::endl;
std::cout<<"Type QUERY to look up Vault..."<<std::endl;
std::cout<<"Type .CSV to check CSV FILE..."<<std::endl;
std::cout<<"Type SQL to make and SQLITE Query..."<<std::endl;
std::cout<<"Type TEST POST to Test POST..."<<std::endl;
std::cout<<"Type BULK ENCRYPT to Encrypt files..."<<std::endl;


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


if(line=="BULK ENCRYPT"){
    std::cout<<"ENCRYPT FILES:";
    std::cout<<"Format like this : {\"files\":[\"file.txt\"]}:";
    std::getline(std::cin,line);
    auto x=line;

    std::cout<<x<<" "<<line<<std::endl;
    json test=json::parse(x);
    std::vector<std::string> files=test["files"];
    
    for(auto it:files){
    dataframe fil(0,2);
    fil.insert(it);
    fil.insert_file(it);
    conf.trezor.insert_new(it,fil.frame.back());
    
    
    }


    std::cout<<"SUCCESSFULL!!"<<std::endl;
}

if(line=="BULK DECRYPT"){
    std::cout<<"DECRYPT FILES:";
    std::getline(std::cin,line);
    auto x=line;

    std::cout<<x<<" "<<line<<std::endl;
    json test=json::parse(x);
    std::vector<std::string> files=test["files"];
    dataframe fil(0,2); 
    for(auto it:files){
   
    fil.insert(it);
    fil.insert(conf.trezor(it));
    //conf.trezor.insert_new(it,fil.frame.back());
    
    
    }
fil.write_files();

    std::cout<<"SUCCESSFULL!!"<<std::endl;
}



if(line=="QUERY"){
    std::getline(std::cin,line);
std::cout<<conf.trezor(line)<<std::endl;
}
//std::cout<<hallo->passcode<<std::endl;
//https_client_request req2("GET", "/v1/models");
if(line=="SQL"){
    dataframe querys;
    std::cout<<"SELECT DATABASE:";
    std::getline(std::cin,line);
    auto x=line;
    std::cout<<"Insert SQL QUERY:";
    std::getline(std::cin,line);
    querys.search_sqlite(x,line);
    if(querys.rows>0){
        querys.write_csv("samples.csv");
    dataframe he;
    he.open_csv("samples.csv");
    if(he!=querys){
    //std::cout<<"SOME MISMATCH"<<std::endl;
    //he.compare(querys);
    }
    if(querys.rows<50){querys.print();}
    else{
        auto help=querys.select_rows(std::vector<int>{1,2});
        help.print();

    }
    }



}

if(line=="HTTP"){

std::cout<<"HTTP REQ URL";
//std::getline(std::cin,line);
auto met="GET";
std::cout<<"Target??";
std::getline(std::cin,line);
dataframe xx(0,1);
xx.insert(line);

auto yyy=xx;
//xx.tokenizer(std::vector<std::string>{"^(.+)/","/.+"});
xx.regex_expand("(^[a-z\.]+)/",1);
yyy.regex_expand("/.+",1);
xx.print();
yyy.print();

//line="/iad/searchagent/alert?searchId=102&alertId=52505926&lastSearchInMilliseconds=1684238304871&xtor=EPR-320-[Suchagent]";
https_client_request req(met, yyy.frame.back());

//std::cout<<"Host??";
//std::getline(std::cin,line);
req.send_request(xx(1,1), "443");

std::cout<<req.res.result()<<std::endl;
//std::cout<<req.res.body()<<std::endl;
//std::cout<<req2.res.body()<<std::endl;
dataframe x3(0,1);
x3.insert(req.res.body());
x3.write_csv("jhkl.html");



}


if(line=="TEST POST"){
https_client_request req("POST", "/v1/chat/completions");
while(true){
    std::cout<<"Insert POST REQUEST:";
    std::getline(std::cin,line);
    if(line=="EXIT"){
        std::cout<<"EXITING POST TEST"<<std::endl;
        break;}
json jrequest =json::parse(line);
req.set_body(jrequest);
http::response<http::string_body> response;
std::string y;
conf.post(req.req,response,y,conf.obj);
std::cout<<"POST DONE"<<std::endl;
if(y==""){
std::cout<<std::string(response.body())<<std::endl;

}
else{std::cout<<y<<std::endl;}
}

}
}

ioc.stop();
}
