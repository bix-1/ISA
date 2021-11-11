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

/**
 * Implementation of Connection Exception thrown by Connection class
 */
class conn_exception : public std::exception
{
private:
    std::string msg_;

public:
    /**
     * A constructor
     * @param msg   Error message
     */
    conn_exception(std::string msg) : msg_(msg) {}

    /**
     * Get error message
     * @return error message as const C string
     */
    const char *what() const throw()
    {
        return msg_.c_str();
    }
};

/**
 * Implementation of Client Connection to POP3 server
 */
class Connection
{
private:
    Options opts_;              /**< Command line options */
    BIO *bio = NULL;            /**< BIO struct */
    SSL_CTX *ctx = NULL;        /**< Context struct */
    SSL *ssl = NULL;            /**< SSL struct */
    std::string username_;      /**< Username for loggin in */
    std::string password_;      /**< Password for loggin in */
    static const uint L = 1024; /**< Buffer length*/
    char buf[L];                /**< Buffer for reading server responses */

    /*__________________Initialization_Methods__________________*/
    /**Initialize SSL */
    static void init_openssl();
    /**
     * Initialize user's credentials
     * Credentials will be stored in username_ & password_
     */
    void init_credentials();
    /**Load trust certificates */
    void load_trust_cert();
    /**Create secured connection*/
    void conn_create_secured(std::string address);
    /**Create unsecured connection*/
    void conn_create_unsecured(std::string address);
    /**Make unsecured connection secure*/
    void conn_make_secure();

    /*___________________Message_Manipulation___________________*/
    /**
     * Get list of saved message IDs
     * @param dir   Name of dir containing messages
     * @return vector of message IDs
     */
    static std::vector<std::string> get_id_list(std::string dir);
    /**
     * Check whether file should be skipped -- if already saved
     * @param list  List of saved message IDs
     * @param file  Filename -- message ID to be checked
     * @return True if file in list, False otherwise
     */
    static bool is_skip_file(std::vector<std::string> list, std::string file);
    /**
     * Check for message ending
     * @param msg   Message to be checked
     * @return True if end of message found, False otherwise
     */
    static bool is_end(std::string msg);

public:
    /**
     * A constructor
     * @param opts  Command line options
     */
    Connection(Options);
    /**A destructor */
    ~Connection();

    /*___________________Command_Manipulation___________________*/
    /**
     * Read server response
     * @param check Check for +OK prefix
     * @return Contents of response
     */
    std::string read(bool check = true);
    /**
     * Send command to server
     * @param msg   Command to be sent
     */
    void write(std::string cmd);

    /*___________________Message_Manipulation___________________*/
    /**
     * Login using user's credentials
     * @return Server response
     */
    std::string login();
    /**
     * Retrieve messages from server
     * Checks for "new" CL option
     * @return Status report of message retrieval
     */
    std::string get_msgs();
    /**
     * Delete messages from server
     * @return Status report of message removal
     */
    std::string delete_msgs();
};
