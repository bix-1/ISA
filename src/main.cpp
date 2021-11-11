/**
 * @file: main.cpp
 * @brief: Main src file for ISA project at FIT BUT 2021
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "connection.h" // conn manipulation
#include "options.h"    // CL options parsing
#include <iostream>     // status & err prints

using namespace std;

int main(int argc, char *argv[])
try
{
    // create connection & authorize
    auto opts = get_opts(argc, argv);
    Connection conn{opts};
    conn.login();

    // retrieve messages
    cout << conn.get_msgs();

    // delete messages if specified
    if (opts.del)
        cout << conn.delete_msgs();
}
catch (const opts_exception &err)
{
    cerr << "ERROR: " << err.what() << endl;
    cerr << "ERROR: Failed to parse CL options -- see ./popcl --help\n";
}
catch (const conn_exception &err)
{
    cerr << "ERROR: " << err.what() << endl;
    cerr << "ERROR: Connection failed\n";
}
catch (...)
{
    cerr << "ERROR: Unknown exception occured\n";
}
