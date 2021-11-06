/**
 * @file: connection.h
 * @brief: Header file for Connection class
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#pragma once

#include "options.h"
#include "openssl/bio.h"
#include <string>

struct Credentials {
    std::string username;
    std::string password;
};

class conn_exception : public std::exception
{
private:
    std::string msg_;

public:
    conn_exception(std::string msg) : msg_(msg) {}

    const char *what() const throw()
    {
        return msg_.c_str();
    }
};

class Connection {
private:
    static const uint L = 1024;
    static void init_openssl();
    static Credentials get_creds(std::string filename);

    BIO *bio;
    char buf[L];
    std::string username_;
    std::string password_;
    Options opts_;

public:
    Connection(Options);
    ~Connection();

    // basic operations
    std::string read();
    void write(std::string msg);

    std::string login();
};
