/**
 * @file: connection.cpp
 * @brief: Implementation of Connection class
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "connection.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string.h> // memset

/* OpenSSL headers */
#include "openssl/bio.h"
#include "openssl/err.h"
#include "openssl/ssl.h"

Connection::Connection(Options opts)
try
{
    init_openssl();
    auto creds = get_creds(opts.auth_file);
    if (creds.username.empty() || creds.password.empty())
        throw conn_exception("Failed to parse username or password");
    username_ = creds.username;
    password_ = creds.password;

    // CONNECT
    bio = BIO_new_connect(opts.server.c_str());
    if (bio == NULL)
        throw conn_exception("Failed to create new connection");
    if (BIO_do_connect(bio) <= 0)
        throw conn_exception("Failed to connect");
}
catch (const conn_exception &err)
{
    std::cerr << "ERROR: " << err.what() << std::endl;
    throw conn_exception("Connection constructor failed");
}

Connection::~Connection()
{
    BIO_free_all(bio);
}

Credentials Connection::get_creds(std::string filename)
{
    std::ifstream file(filename);
    std::string tmp;
    Credentials creds;

    file >> tmp >> tmp >> creds.username;
    file >> tmp >> tmp >> creds.password;

    return creds;
}

void Connection::init_openssl()
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
}

std::string Connection::read()
{
    memset(buf, 0, L);
    int x = BIO_read(bio, buf, L);
    if (x == 0)
    {
        throw conn_exception("Connection was closed");
    }
    else if (x < 0)
    {
        if (!BIO_should_retry(bio))
        {
            throw conn_exception("Failed to read the response");
        }
    }

    // process response
    std::string tmp(buf, 3);
    if (tmp != "+OK")
        throw conn_exception("Err on response:\n......." + std::string(buf));

    return buf;
}

void Connection::write(std::string msg)
{
    msg += "\r\n";
    if (BIO_write(bio, msg.c_str(), msg.length()) <= 0)
    {
        if (!BIO_should_retry(bio))
        {
            throw conn_exception("Failed to write message");
        }
    }
}

std::string Connection::login()
{
    std::string response;
    write("USER " + username_);
    response += read();
    write("PASS " + password_);

    return response + read();
}
