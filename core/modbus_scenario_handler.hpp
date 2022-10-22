#pragma once
#include "ih_modbus_proxy.hpp"
#include <filesystem>

class ModbusScenarioHandler
{
public:
    ModbusScenarioHandler(
        const std::filesystem::path& scenarioPath,
        std::unique_ptr<IModbusProxy>&& modbusProxy);
    ~ModbusScenarioHandler();

public:
    void launchScenario();

public:
    class ModbusScenarioHandlerImpl;
    std::unique_ptr<ModbusScenarioHandlerImpl> m_pImpl;
};