#pragma once
#include "modbus_proxy_types.hpp"
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

class ModbusRequestsProxy
{

public:
    ModbusRequestsProxy(const ModbusConfigData& applicationConfig);
    ~ModbusRequestsProxy();

public:
    std::optional<std::int16_t> readRegister(std::uint16_t registerAddress);
    std::optional<std::vector<std::uint16_t>> readRegisters(
        std::uint16_t registerAddress,
        std::uint16_t registersCount);

    void scheduleRegistersWrite(
        std::uint16_t registerAddress,
        const std::vector<std::uint16_t>& registers);

private:
    class ModbusRequestsProxyImpl;
    std::unique_ptr<ModbusRequestsProxyImpl> m_pImpl;
};

std::unique_ptr<ModbusRequestsProxy> createModbusProxy(const ModbusConfigData& applicationConfig);