
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>

static std::vector<unsigned char> encrypt_symmetric_new(const std::vector<unsigned char>& plaintext, const std::string& key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char*)key.c_str(), nullptr);

    unsigned char iv[EVP_MAX_IV_LENGTH];
    RAND_bytes(iv, EVP_MAX_IV_LENGTH);

    EVP_EncryptInit_ex(ctx, nullptr, nullptr, nullptr, iv);

    int out_len = plaintext.size() + EVP_MAX_BLOCK_LENGTH;
    std::vector<unsigned char> ciphertext(out_len);

    int update_len = 0;
    EVP_EncryptUpdate(ctx, ciphertext.data(), &update_len, plaintext.data(), plaintext.size());

    int final_len = 0;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + update_len, &final_len);

    out_len = update_len + final_len;
    ciphertext.resize(out_len + EVP_MAX_IV_LENGTH);
    std::copy_n(iv, EVP_MAX_IV_LENGTH, ciphertext.data() + out_len);
    
    EVP_CIPHER_CTX_free(ctx);
    
    return ciphertext;
}

static std::vector<unsigned char> decrypt_symmetric_new(const std::vector<unsigned char>& ciphertext, const std::string& key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char*)key.c_str(), nullptr);

    unsigned char iv[EVP_MAX_IV_LENGTH];
    std::copy_n(ciphertext.end() - EVP_MAX_IV_LENGTH, EVP_MAX_IV_LENGTH, iv);

    std::vector<unsigned char> plaintext(ciphertext.size() - EVP_MAX_IV_LENGTH);

    int out_len = 0;
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, nullptr, iv);
    EVP_DecryptUpdate(ctx, plaintext.data(), &out_len, ciphertext.data(), ciphertext.size() - EVP_MAX_IV_LENGTH);

    int final_len = 0;
    EVP_DecryptFinal_ex(ctx, plaintext.data() + out_len, &final_len);

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(out_len + final_len); // Resize the vector to the actual plaintext length

    return plaintext;
}




static std::string encrypt_symmetric(const std::string &plaintext, const std::string &key)
{

    // Set up the encryption context

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    EVP_CIPHER_CTX_init(ctx);

    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *)key.c_str(), nullptr);

    // Generate a random IV

    unsigned char iv[EVP_MAX_IV_LENGTH];

    RAND_bytes(iv, EVP_MAX_IV_LENGTH);

    EVP_EncryptInit_ex(ctx, nullptr, nullptr, nullptr, iv);

    // Encrypt the plaintext

    int out_len = plaintext.size() + EVP_MAX_BLOCK_LENGTH;

    unsigned char *ciphertext = new unsigned char[out_len];

    EVP_EncryptUpdate(ctx, ciphertext, &out_len, (const unsigned char *)plaintext.c_str(), plaintext.size());

    // Finalize the encryption and append the IV to the ciphertext

    int final_len;

    EVP_EncryptFinal_ex(ctx, ciphertext + out_len, &final_len);

    out_len += final_len;

    std::string result((const char *)iv, EVP_MAX_IV_LENGTH);

    result += std::string((const char *)ciphertext, out_len);

    // Write the result to the output file

    // Clean up

    delete[] ciphertext;

    EVP_CIPHER_CTX_free(ctx);

    return result;
}

// Define the decryption function

static std::string decrypt_symmetric(const std::string &input_file, const std::string &key)
{

    std::stringstream input(input_file);
    std::vector<unsigned char> ciphertext_vec((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    std::string ciphertext((const char *)ciphertext_vec.data(), ciphertext_vec.size());

    // std::string ciphertext(input_file);

    // Extract the IV from the ciphertext

    unsigned char iv[EVP_MAX_IV_LENGTH];

    std::string ciphertext_without_iv = ciphertext.substr(EVP_MAX_IV_LENGTH);

    std::string iv_str = ciphertext.substr(0, EVP_MAX_IV_LENGTH);

    memcpy(iv, iv_str.c_str(), EVP_MAX_IV_LENGTH);

    // Set up the decryption context

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    EVP_CIPHER_CTX_init(ctx);

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *)key.c_str(), nullptr);
    EVP_DecryptInit_ex(ctx, nullptr, nullptr, nullptr, iv);

    // handle error

    // Decrypt the ciphertext

    int out_len = ciphertext_without_iv.size() + EVP_MAX_BLOCK_LENGTH;
    //  std::cout<<out_len<<std::endl;
    unsigned char *plaintext = new unsigned char[out_len];

    EVP_DecryptUpdate(ctx, plaintext, &out_len, (const unsigned char *)ciphertext_without_iv.c_str(), ciphertext_without_iv.size());

    // Finalize the decryption

    int final_len = 0;

    if (EVP_DecryptFinal_ex(ctx, plaintext + out_len, &final_len) != 1)
    {
        // std::cout<<ERR_error_string()<<std::endl;
        unsigned long err = ERR_get_error();
        char err_buf[256];
        ERR_error_string(err, err_buf);
        std::cout << err_buf << std::endl;
    }
    // std::cout<<plaintext<<"  "<<final_len<< std::endl;

    out_len += final_len;

    std::string result((const char *)plaintext, out_len);

    // Clean up

    delete[] plaintext;

    EVP_CIPHER_CTX_free(ctx);

    return result;
}

class keyvault
{

public:

    bool locked(){

        if(true==flag){return false;}
        else{return true;}
    }

    keyvault()
    {
        flag = false;

    };

    keyvault(std::string database, std::string accounts, std::string pass)
    {

        dataframe X(0, 2);
        X.insert(accounts);
        // X.insert(pass);
        X.search_sqlite(database, accounts, std::vector<std::string>{"account"}, std::vector<std::string>{"account", "token"});
        std::string keys = pass;

        if (X.rows > 1)
        {
            const std::string val = X(2, 2);
            try
            {
                auto passed = decrypt(val, keys);
                if (passed == pass)
                {
                    passcode = pass;
                    account = accounts;
                    vault = database;
                    flag = true;
                    std::cout << "authenicated...." << std::endl;
                }
                else
                {
                    flag = false;
                    std::cout << "failed...." << std::endl;
                }
            }

            catch (...)
            {
                flag = false;
                std::cout << "failed...." << std::endl;
            }
        }
        else
        {
            flag = false;
            std::cout << "failed...." << std::endl;
        }
    };

    std::string key(const std::string &pass)
    {
        auto temp = pass;
        while (temp.size() < 32)
            temp += pass;
        while (temp.size() > 32)
            temp.pop_back();
        return temp;
    };





    std::vector<unsigned char>  encrypt2(std::vector<unsigned char> &text, const std::string &pass)
    {
        //std::vector<unsigned char> encodedData(text.begin(), text.end());
        auto keys = key(pass);
        //std::cout<<encrypt_symmetric(text, keys)<<std::endl;
        return encrypt_symmetric_new(text, keys);
    }

    std::vector<unsigned char> decrypt2(std::vector<unsigned char> text, const std::string pass)
    {
        auto keys = key(pass);
        //std::vector<unsigned char> encodedData(text.begin(), text.end());
        return decrypt_symmetric_new(text, keys); // decrypt the ciphertext using the symmetric encryption algorithm and the IV
    }


    std::string encrypt(const std::string &text, const std::string &pass)
    {
        
        auto keys = key(pass);
        //std::cout<<encrypt_symmetric(text, keys)<<std::endl;
        return encrypt_symmetric(text, keys);
    }

    std::string decrypt(const std::string encrypted_text, const std::string pass)
    {
        auto keys = key(pass);
        return decrypt_symmetric(encrypted_text, keys); // decrypt the ciphertext using the symmetric encryption algorithm and the IV
    }

    void insert(std::string service, std::string token)
    {
        dataframe X(0, 2);
        X.insert(std::vector<std::string>{"account", "token"});

        // auto keys=key(token);
        std::string x = token;
        x = encrypt(token, token);
        // std::cout<<"test  "<<decrypt(x,token)<<std::endl;
        X.insert(std::vector<std::string>{service, x});
        X.write_sqlite(vault, service, std::vector<std::string>{"TEXT", "BLOB"}, std::vector<int>{1});
    };

    std::string operator()(std::string service)
    {
        try
        {
            if (flag)
            {
                dataframe X(0, 1);
                X.insert(service);
                X.search_sqlite(vault, account, std::vector<std::string>{"account"}, std::vector<std::string>{"account", "token"});
                //      X.print();
                if (X.rows > 1)
                {
                    const std::string as = X(2, 2);
                    return decrypt(as, passcode);
                }
                else
                {
                    return "";
                }
            }
            else
            {
                return "";
            }
        }
        catch (...)
        {
            return "";
        }
    };

    void set_vault(std::string a) { vault = a; }
    void data()
    {
        std::cout << passcode << std::endl;
        std::cout << vault << std::endl;
        std::cout << account << std::endl;
        std::cout << flag << std::endl;
    };

    void insert_new(std::string service, std::string token)
    {

        if (flag)
        {
            dataframe X(0, 2);
            X.insert(std::vector<std::string>{"account", "token"});
            // auto keys=key(token);
            token = encrypt(token, passcode);
            X.insert(std::vector<std::string>{service, token});
            // X.print();
            X.write_sqlite(vault, account, std::vector<std::string>{"TEXT", "BLOB"}, std::vector<int>{1});
        }
    }


    void new_account(std::string account, std::string token,std::string db)
    {


            dataframe X(0, 2);
            X.insert(std::vector<std::string>{"account", "token"});
            // auto keys=key(token);
            token = encrypt(token, token);
            X.insert(std::vector<std::string>{account, token});
            // X.print();
            X.write_sqlite(db, account, std::vector<std::string>{"TEXT", "BLOB"}, std::vector<int>{1});
    }


private:
    bool flag;
    std::string passcode;
    std::string account;
    std::string vault;
};
