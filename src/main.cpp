/**
 * @file: main.cpp
 * @brief: Main src file for ISA project at FIT BUT 2021
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "main.h"
#include <iostream>
#include <stdio.h>

/* OpenSSL headers */
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

int main()
{
    init_openssl();
}

void init_openssl()
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    SSL_library_init();
}
