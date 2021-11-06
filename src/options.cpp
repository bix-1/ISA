/**
 * @file: options.cpp
 * @brief: Implementation of CL option parsing methods
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "options.h"
#include <getopt.h>
#include <iostream>
#include <stdexcept>
#include <string>

Options get_opts(int argc, char *argv[])
{
    Options opts;

    opterr = 0; // disable getopt error call

    // get first (unnamed) option: <server>
    if (argc > 2)
        opts.server = argv[1];

    int opt;
    while (true)
    {
        opt = getopt(argc, argv, "a:o:hp:TSc:C:dn");
        if (opt == -1)
            break;

        switch (opt)
        {
            case 'h':
                std::cout << "TODO\n";
                exit(EXIT_SUCCESS);

            case 'a':
                opts.auth_file = optarg;
                break;

            case 'o':
                opts.out_dir = optarg;
                break;

            case 'p':
                try
                {
                    opts.port = std::stoi(optarg);
                }
                catch (...)
                {
                    throw opts_exception("Invalid port number \"" + std::string(optarg) + "\"");
                }
                break;

            case 'T':
                opts.T = true;
                break;

            case 'S':
                opts.S = true;
                break;

            case 'c':
                opts.certfile = optarg;
                break;

            case 'C':
                opts.certaddr = optarg;
                break;

            case 'd':
                opts.del = true;
                break;

            case 'n':
                opts.nw = true;
                break;

            case '?':
            {
                std::string msg;
                if (optopt == 'a' || optopt == 'o' || optopt == 'p' || optopt == 'c' || optopt == 'C')
                    msg = "Option [" + std::string(1, char(optopt)) + "] requires an argument";
                else
                    msg = "Unknown option [" + std::string(1, char(optopt)) + "]";
                throw opts_exception(msg.c_str());
            }

            default:
                throw opts_exception("Aborted");
        }
    }

    if (opts.server.empty() || opts.auth_file.empty() || opts.out_dir.empty())
        throw opts_exception("Missing required CL options");

    return opts;
}
