#include "modbus_config_parser.hpp"
#include "modbus_proxy.hpp"
#include "modbus_scenario_handler.hpp"
#include <boost/program_options.hpp>
#include <format>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, const char** av)
{

    po::options_description description{"Allowed options"};
    description.add_options()("help", "produce help message")(
        "serial_path",
        po::value<std::string>()->default_value("COM8"),
        "serial device which will be used for connection")(
        "baudrate", po::value<int>()->default_value(9600), "connection baudrate")(
        "slave_id", po::value<int>()->default_value(1), "modbus slave id")(
        "scenario_path",
        po::value<std::filesystem::path>()->default_value("./scenario.json"),
        "path to scenarios file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, av, description), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << description << std::endl;
        return 1;
    }

    auto scenarioPath{vm["scenario_path"].as<std::filesystem::path>()};
    auto pModbusProxy{createModbusProxy(parseCmdConfig(vm))};

    try
    {

        auto modbusScenariosHandler{
            std::make_unique<ModbusScenarioHandler>(scenarioPath, std::move(pModbusProxy))};

        modbusScenariosHandler->launchScenario();
    }
    catch (const std::exception& ex)
    {
        spdlog::error("EXCEPTION OCCURED: {}", ex.what());
        return 1;
    }
    return 0;
}