/**
 * @file: connection.cpp
 * @brief: Implementation of Connection class
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "connection.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string.h> // memset
#include <vector>

/* OpenSSL headers */
#include "openssl/bio.h"
#include "openssl/err.h"
#include "openssl/ssl.h"

namespace fs = std::filesystem;

Connection::Connection(Options opts)
try : opts_{opts}
{
    // init credentials
    auto creds = get_creds(opts.auth_file);
    if (creds.username.empty() || creds.password.empty())
        throw conn_exception("Failed to parse username or password");
    username_ = creds.username;
    password_ = creds.password;

    // init server address
    std::string port = !opts.port.empty() ? opts.port : (opts.T ? "995" : "110");
    std::string address = opts.server + ":" + port;

    // init connection
    init_openssl();
    bio = BIO_new_connect(address.c_str());
    if (bio == NULL)
        throw conn_exception("Failed to create new connection");
    if (BIO_do_connect(bio) <= 0)
        throw conn_exception("Failed to connect");

    // check server response
    read();
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
    Credentials creds;
    if (std::ifstream file{filename})
    {
        std::string tmp;
        file >> tmp >> tmp >> creds.username;
        file >> tmp >> tmp >> creds.password;
    }
    return creds;
}

void Connection::init_openssl()
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
}

std::string Connection::read(bool check)
{
    memset(buf, 0, L);
    int x = BIO_read(bio, buf, L);
    if (x == 0)
    {
        throw conn_exception("Connection was closed");
    }
    else if (x < 0 && !BIO_should_retry(bio))
    {
        throw conn_exception("Failed to read the response");
    }

    // process response
    if (check)
    {
        std::string resp(buf, 3);
        if (resp != "+OK")
            throw conn_exception("Err on response:\n  " + std::string(buf));
    }

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

std::string Connection::get_msgs()
{
    // check for messages to ommit -- if -n option specified
    std::vector<std::string> skip_id_list;
    if (opts_.new_only)
        skip_id_list = get_id_list(opts_.out_dir);

    // get messages count
    write("STAT");
    std::stringstream ss(read());
    std::string _;
    int mail_count, size, cnt{};
    ss >> _ >> mail_count >> size;

    // create dest dir for messages
    fs::create_directories(opts_.out_dir);

    // retrieve messages
    for (auto i = 0; i < mail_count; i++)
    {
        // get n-th message & check initial response
        write("RETR " + std::to_string(i + 1));
        std::string response = read();
        // get the rest of message
        do
        {
            response += read(false);
        } while (!is_end(response));

        // get message ID for filename
        std::smatch m;
        std::regex pattern("Message-ID: <(.+)>\r", std::regex_constants::icase);
        std::regex_search(response, m, pattern);
        std::string filename = opts_.out_dir + std::string(m[1]);

        // message might be specified as old -- due to -n flag
        if (opts_.new_only && !is_skip_file(skip_id_list, filename))
            continue;

        if (std::ofstream file{filename})
        {
            file << response.substr(0, response.find("\r\n.\r\n") + 2);
            cnt++;
        }
        else
        {
            std::cerr << "Unable to open file \"" << filename << "\"" << std::endl;
        }
    }
    return "Saved [" + std::to_string(cnt) + "] messages to dir \"" + opts_.out_dir + "\"\n";
}

bool Connection::is_end(std::string msg)
{
    return msg.substr(msg.length() - 5) == "\r\n.\r\n";
}

std::string Connection::delete_msgs()
{
    write("STAT");
    std::stringstream ss(read());
    std::string _;
    int mail_count, size;
    ss >> _ >> mail_count >> size;
    auto cnt = 0;
    for (auto i = 0; i < mail_count; i++)
    {
        write("DELE " + std::to_string(i + 1));
        try
        {
            read();
            cnt++;
        }
        catch (const conn_exception &err)
        {
            std::cerr << "Delete failed: " << err.what() << std::endl;
        }
    }

    /** TODO: remove after testing */
    // UPDATE state --> delete marked messages
    // write("QUIT");
    // read();

    return "Deleted " + std::to_string(cnt) + " messages\n";
}

std::vector<std::string> Connection::get_id_list(std::string dirname)
{
    if (!fs::is_directory(dirname))
        return std::vector<std::string>();

    std::vector<std::string> out;
    for (const auto &filename : fs::directory_iterator(dirname))
        out.push_back(filename.path());

    return out;
}

bool Connection::is_skip_file(std::vector<std::string> list, std::string file)
{
    return std::find(list.begin(), list.end(), file) == list.end();
}
