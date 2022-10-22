#pragma once
#include "modbus_proxy_types.hpp"
#include <boost/program_options.hpp>
#include <spdlog/spdlog.h>

namespace po = boost::program_options;

static ModbusConfigData parseCmdConfig(const po::variables_map& vm)
{
    ModbusConfigData data{};
    const auto& serialPath{vm["serial_path"].as<std::string>()};
    data.serialDevicePath = serialPath;

    int serialBaud{vm["baudrate"].as<int>()};
    data.modbusBaudrate = serialBaud;

    int slaveId{vm["slave_id"].as<int>()};
    data.modbusSlaveAddress = slaveId;

    auto fCheckOption = [&vm]<typename TOption>(
                            auto optionName,
                            auto existsOptionText,
                            auto unexistingOptionText,
                            TOption&& optionValue) mutable {
        if (vm.count(optionName))
        {
            spdlog::info(existsOptionText, optionValue);
        }
        else
        {
            spdlog::warn(unexistingOptionText, optionValue);
        }
    };

    fCheckOption(
        "serial_path",
        "Seial connection path is: {}",
        "serial_path is not set. The default {} will be used",
        serialPath);
    fCheckOption(
        "baudrate",
        "connection baudrate is: {}",
        "baudrate is not set. The default {} will be used",
        serialBaud);

    fCheckOption(
        "slave_id", "Slave ID is: {}", "Slave id is not set, the default {} will be used", slaveId);

    return data;
}
