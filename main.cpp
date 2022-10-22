#include <boost/program_options.hpp>
#include <format>
#include <iostream>
#include "modbus_proxy.hpp"
#include "modbus_config_parser.hpp"

namespace po = boost::program_options;

int main(int argc, const char** av)
{

    po::options_description description{"Allowed options"};
    description.add_options()("help", "produce help message")(
        "serial_path",
        po::value<std::string>()->default_value("COM8"),
        "serial device which will be used for connection")(
        "baudrate", po::value<int>()->default_value(9600), "connection baudrate")(
        "slave_id", po::value<int>()->default_value(1), "modbus slave id");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, av, description), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << description << std::endl;
        return 1;
    }

    auto modbusProxy = createModbusProxy(parseCmdConfig(vm));

}