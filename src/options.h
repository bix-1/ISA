/**
 * @file: options.h
 * @brief: Header file for CL options parsing
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#pragma once

#include <string>
#include <exception>

class opts_exception : public std::exception
{
private:
    std::string msg_;

public:
    opts_exception(std::string msg) : msg_(msg) {}

    const char *what() const throw()
    {
        return msg_.c_str();
    }
};

struct Options
{
    std::string server;
    std::string auth_file;
    std::string out_dir;
    std::string port;

    bool T = false;
    bool S = false;
    std::string certfile;
    std::string certaddr;

    bool del = false;
    bool nw = false;
};

Options get_opts(int argc, char *argv[]);
