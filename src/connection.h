/**
 * @file: connection.h
 * @brief: Header file for Connection class
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#pragma once

#include "openssl/bio.h"
#include <string>

class Connection {
private:
    static const uint L = 1024;
    static void init_openssl();

    BIO *bio;
    char buf[L];
    std::string username_;
    std::string password_;

public:
    Connection(std::string username, std::string password);
    ~Connection();

    std::string read();
    void write(std::string msg);
    std::string login();
};
