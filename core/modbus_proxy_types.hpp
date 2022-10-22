#pragma once
#include <cstdint>
#include <string>

struct ModbusConfigData
{
    bool isDebugMode;
    std::uint8_t modbusSlaveAddress;
    std::string serialDevicePath;
    std::uint32_t modbusBaudrate;
};
