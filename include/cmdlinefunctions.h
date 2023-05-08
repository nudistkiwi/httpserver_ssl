
void read_input(net::io_context& ioc,server_configuration & conf){

std::string line;



std::cout<<"Type INSERT to add User..."<<std::endl;
std::cout<<"Type NEW to add new Account..."<<std::endl;
std::cout<<"Type QUERY to look up Vault..."<<std::endl;
std::cout<<"Type .CSV to check CSV FILE..."<<std::endl;
std::cout<<"Type SQL to make and SQLITE Query..."<<std::endl;
std::cout<<"Type TEST POST to Test POST..."<<std::endl;
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
