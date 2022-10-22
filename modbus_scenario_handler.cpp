#include "modbus_scenario_handler.hpp"
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <ranges>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

namespace
{
constinit std::string_view kWriteOperation = "write";
constinit std::string_view kReadOperation = "read";

} // namespace

class ModbusScenarioHandler::ModbusScenarioHandlerImpl
{
public:
    ModbusScenarioHandlerImpl(
        const std::filesystem::path& scenarioPath,
        std::unique_ptr<IModbusProxy>&& modbusProxy)
        : m_scenarioPath{scenarioPath}, m_pModbusDevice{std::move(modbusProxy)}
    {
        if (!std::filesystem::exists(scenarioPath))
            throw std::runtime_error("Scenario path doesn't exists!");
    }

public:
    void launchScenario()
    {
        std::fstream scenarioFile{m_scenarioPath};
        json jsonRoot{json::parse(scenarioFile)};
        json jsonScenario{jsonRoot["scenarios"]};

        spdlog::warn(
            "***********SCENARIO {} BEGIN********************", jsonScenario.value("title", ""));
        for (const auto& scenarioItem : jsonScenario)
        {
            const auto& operation = scenarioItem["operation"].get<std::string>();
            const auto& repetionsCount = scenarioItem["repitions"].get<std::int64_t>();

            for (auto repitionsIt : std::ranges::iota_view{0, repetionsCount})
            {
                if (operation == kReadOperation)
                    handleReadOperation(scenarioItem);
                else if (operation == kWriteOperation)
                    handleWriteOperation(scenarioItem);
            }
        }
        spdlog::warn(
            "***********SCENARIO {} END**********************", jsonScenario.value("title", ""));
    }

private:
    void handleReadOperation(const json& scenarioItem)
    {
        const auto& registers = scenarioItem["registers"];

        for (const auto& registerItem : registers)
        {
            auto registerNum = boost::lexical_cast<std::uint16_t>(registerItem.get<std::string>());
            auto registerRead{m_pModbusDevice->readRegister(registerNum)};
            if (registerRead)
                spdlog::info("register {} value: {}", registerNum, registerRead.value());
            else
                spdlog::error("failed to read from {}",registerNum);
        }
    }

    void handleWriteOperation(const json& scenarioItem)
    {
        const auto& registers = scenarioItem["registers"];
        for (const auto& registerItem : registers)
        {
            auto registerNum = boost::lexical_cast<std::uint16_t>(registerItem[0].get<std::string>());
            auto registerValue = registerItem[1].get<std::uint16_t>();

           m_pModbusDevice->scheduleRegistersWrite(registerNum, {registerValue});
        }
    }

private:
    std::filesystem::path m_scenarioPath;
    std::unique_ptr<IModbusProxy> m_pModbusDevice;
};

ModbusScenarioHandler::ModbusScenarioHandler(
    const std::filesystem::path& scenarioPath,
    std::unique_ptr<IModbusProxy>&& modbusProxy)
    : m_pImpl{std::make_unique<ModbusScenarioHandlerImpl>(scenarioPath, std::move(modbusProxy))}
{
}

ModbusScenarioHandler::~ModbusScenarioHandler() = default;

void ModbusScenarioHandler::launchScenario()
{
    m_pImpl->launchScenario();
}
