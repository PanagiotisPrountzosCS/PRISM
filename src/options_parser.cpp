#include "options_parser.h"

#include <iostream>

#include "cxxopts.hpp"

namespace PRISM
{

bool validate_opts(int argc, char** argv, options& options)
{
        cxxopts::Options opts("prism_cli / prism_gui",
                              "Predictive realtime industrial sensor monitor");

        // yeah this looks ugly ahhh
        opts.add_options()("h,help", "Print usage")(
            "H,hostname", "mqtt hostname", cxxopts::value<std::string>())(
            "t,topic", "topic to subscribe to", cxxopts::value<std::string>())(
            "u,username", "mqtt username", cxxopts::value<std::string>())(
            "P,password", "mqtt password", cxxopts::value<std::string>())(
            "p,port", "mqtt broker port", cxxopts::value<uint32_t>())(
            "c, cert", "root certificate", cxxopts::value<std::string>());

        cxxopts::ParseResult result;
        try
        {
                result = opts.parse(argc, argv);
        }
        catch (const cxxopts::exceptions::exception& e)
        {
                std::cout << "error parsing options: " << e.what() << '\n';
        }
        if (result.count("help"))
        {
                std::cout << opts.help();
                exit(EXIT_FAILURE);
        }
        if (!result.count("hostname")) return false;
        if (!result.count("topic")) return false;
        if (!result.count("username")) return false;
        if (!result.count("password")) return false;
        if (!result.count("port")) return false;

        options.host = result["hostname"].as<std::string>();
        options.topic = result["topic"].as<std::string>();
        options.username = result["username"].as<std::string>();
        options.password = result["password"].as<std::string>();
        options.port = result["port"].as<uint32_t>();

        if (result.count("cert"))
                options.cert_path = result["cert"].as<std::string>();
        else
                options.cert_path = "";

        return true;
}

}  // namespace PRISM