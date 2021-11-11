/**
 * @file: options.h
 * @brief: Header file for CL options parsing
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#pragma once

#include <string>
#include <exception>

/**
 * Implementation of CL Options Exception thrown by get_opts method
 */
class opts_exception : public std::exception
{
private:
    std::string msg_;

public:
    /**
     * A constructor
     * @param msg   Error message
     */
    opts_exception(std::string msg) : msg_(msg) {}

    /**
     * Get error message
     * @return error message as const C string
     */
    const char *what() const throw()
    {
        return msg_.c_str();
    }
};

struct Options
{
    std::string server;     /**< Server address */
    std::string auth_file;  /**< File containing credentials */
    std::string out_dir;    /**< Destination directory for messages */
    std::string port;       /**< Port number */

    bool T = false;         /**< T flag */
    bool S = false;         /**< S flag */
    std::string certfile;   /**< File containing certificated */
    std::string certaddr;   /**< Directory containing certificates */

    bool del = false;       /**< Delete messages flag */
    bool new_only = false;  /**< Read new messages only flag */
};

/**
 * Parse command line options
 * @param argc  number of arguments
 * @param argv  list of arguments
 * @return parsed options
 */
Options get_opts(int argc, char *argv[]);
