/**
 * @file: connection.cpp
 * @brief: Implementation of Connection class
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "connection.h"

#include <iostream>
#include <stdexcept>
#include <string.h> // memset

/* OpenSSL headers */
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

Connection::Connection(std::string username, std::string password)
try : username_{username}, password_{password}
{
    init_openssl();
    // CONNECT
    bio = BIO_new_connect("pop.mail.com:110");
    if (bio == NULL)
    {
        throw std::runtime_error("Failed to create new connection");
    }
    if (BIO_do_connect(bio) <= 0)
    {
        throw std::runtime_error("Failed to connect");
    }
}
catch (const std::runtime_error &err)
{
    std::cerr << "ERROR: " << err.what() << std::endl;
    throw std::runtime_error("Connection constructor failed");
}

Connection::~Connection() {
    BIO_free_all(bio);
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
        throw std::runtime_error("Connection was closed");
    }
    else if (x < 0)
    {
        if (!BIO_should_retry(bio))
        {
            throw std::runtime_error("Failed to read the response");
        }
    }

    // process response
    std::string tmp (buf, 3);
    if (tmp != "+OK")
        throw std::runtime_error("Err on response:\n......." + std::string(buf));

    return buf;
}

void Connection::write(std::string msg)
{
    msg += "\r\n";
    if (BIO_write(bio, msg.c_str(), msg.length()) <= 0)
    {
        if (!BIO_should_retry(bio))
        {
            throw std::runtime_error("Failed to write message");
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
