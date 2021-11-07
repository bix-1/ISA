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

struct Credentials
{
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

class Connection
{
private:
    static const uint L = 1024;
    static void init_openssl();
    static Credentials get_creds(std::string filename);
    static std::vector<std::string> get_id_list(std::string dir);
    static bool is_skip_file(std::vector<std::string> list, std::string file);
    static bool is_end(std::string);

    BIO *bio;
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
