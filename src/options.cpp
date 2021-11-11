/**
 * @file: options.cpp
 * @brief: Implementation of CL option parsing methods
 * @project: POP3 client with TLS support
 * @authors: Jakub Bartko    xbartk07@stud.fit.vutbr.cz
 */

#include "options.h" // own header
#include <getopt.h>  // CL opt parsing
#include <iostream>  // help print
#include <string>

Options get_opts(int argc, char *argv[])
{
    Options opts;

    // get first (unnamed) option: <server>
    if (argc > 2)
        opts.server = argv[1];

    opterr = 0; // disable getopt error call
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int opt, opt_index;
    while (true)
    {
        opt = getopt_long(argc, argv, "a:o:hp:TSc:C:dn", long_options, &opt_index);
        if (opt == -1)
            break;

        switch (opt)
        {
            // help
            case 'h':
                std::cout << "usage: ./popcl <SERVER> [-h] [-p PORT] [-T|-S [-c CERTFILE] [-C CERTADDR]] [-d] [-n] -a AUTH_FILE -o OUT_DIR\n\n"
                          << "Email client implementing POP3, pop3s & POP3 STARTTLS.\n\n"
                          << "required arguments:\n"
                          << "  SERVER\t\tserver address\n"
                          << "  -a AUTH_FILE\t\tfile with user credentials\n"
                          << "  -o OUT_DIR\t\tdirectory for saving messages\n"
                          << "\noptional arguments:\n"
                          << "  -h, --help\t\tprint this help and exit\n"
                          << "  -p PORT\t\tserver port\n"
                          << "  -T\t\t\tenable pop3s connection\n"
                          << "  -S\t\t\tenable STLS connection\n"
                          << "  -c CERTFILE\t\tfile with certificates -- used only with [-T|-S]\n"
                          << "  -C CERTADDR\t\tdirectory with certificates -- used only with [-T|-S]\n"
                          << "  -d\t\t\tdelete messages after their retrieval\n"
                          << "  -n\t\t\tretrieve new messages only\n";
                exit(EXIT_SUCCESS);

            case 'a':
                opts.auth_file = optarg;
                break;

            case 'o':
                opts.out_dir = optarg;
                if (opts.out_dir.back() != '/')
                {
                    opts.out_dir += '/';
                }
                break;

            case 'p':
                opts.port = optarg;
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
                opts.new_only = true;
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
