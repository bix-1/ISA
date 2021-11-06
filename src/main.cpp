/**
 * @file: main.cpp
 * @brief: Main src file for ISA project at FIT BUT 2021
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

/**
 * TODO:
 * deleting mails
 * new
 */

#include "connection.h"
#include "options.h"
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace std;

int main(int argc, char *argv[])
try
{
    auto opts = get_opts(argc, argv);
    Connection conn{opts};
    conn.login();
    cout << conn.get_all();
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
