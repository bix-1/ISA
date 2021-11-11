/**
 * @file: connection.h
 * @brief: Header file for Connection class
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#pragma once

#include "openssl/bio.h"
#include "options.h"
#include <string>
#include <vector>

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

class Connection
{
private:
    static const uint L = 1024;
    static void init_openssl();
    static std::vector<std::string> get_id_list(std::string dir);
    static bool is_skip_file(std::vector<std::string> list, std::string file);
    static bool is_end(std::string);

    void init_credentials();
    void load_trust_cert();
    void conn_create_secured(std::string address);
    void conn_create_unsecured(std::string address);
    void conn_make_secure();

    BIO *bio = NULL;
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    char buf[L];
    std::string username_;
    std::string password_;
    Options opts_;


public:
    Connection(Options);
    ~Connection();

    // basic command operations
    std::string read(bool check = true);
    void write(std::string msg);

    // message operations
    std::string login();
    std::string get_msgs();
    std::string delete_msgs();
};
