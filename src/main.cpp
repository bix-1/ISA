/**
 * @file: main.cpp
 * @brief: Main src file for ISA project at FIT BUT 2021
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

/**
 * TODO:
 * proccess CLI args
 * retrieval of emails
 * saving of emails
 */

#include "connection.h"
#include <iostream>
#include <stdio.h>

using namespace std;

int main()
try
{
    Connection conn{"jdbix@mail.com", "thisIsMyPassword"};
    cout << conn.read();
    cout << conn.login();
}
catch (const runtime_error &err)
{
    cerr << "ERROR: " << err.what() << endl;
}
catch (...)
{
    cerr << "ERROR: Unknown exception occured\n";
}
