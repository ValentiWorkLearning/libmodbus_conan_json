#include "modbus_scenario_handler.hpp"
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <ranges>
#include <spdlog/spdlog.h>
#include <thread>

using json = nlohmann::json;

namespace
{
constinit std::string_view kWriteOperation = "write";
constinit std::string_view kReadOperation = "read";

std::uint16_t stringToRegister(const std::string& registerHex)
{
    auto registerNumStrView{std::string_view{registerHex}};

    auto xPos{registerNumStrView.find_last_of("x") + 1};
    std::string_view registerNumHex = registerNumStrView.substr(xPos);

    std::stringstream ss;
    ss << std::hex << registerNumHex;

    std::uint16_t hexValue{};
    ss >> hexValue;
    return hexValue;
}

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
            throw std::runtime_error("Scenario path doesn't exist!");
    }

public:
    void launchScenario()
    {
        std::fstream scenarioFile{m_scenarioPath};
        json jsonRoot{json::parse(scenarioFile)};
        json jsonScenario{jsonRoot["scenarios"]};

        for (const auto& scenarioItem : jsonScenario)
        {
            spdlog::warn(
                "***********SCENARIO {} BEGIN********************",
                scenarioItem.value("title", ""));

            const auto& operation = scenarioItem["operation"].get<std::string>();
            const auto& repetionsCount = scenarioItem["repitions"].get<std::int64_t>();

            for (auto repitionsIt : std::ranges::iota_view{0, repetionsCount})
            {
                if (operation == kReadOperation)
                    handleReadOperation(scenarioItem);
                else if (operation == kWriteOperation)
                    handleWriteOperation(scenarioItem);
            }
            spdlog::warn(
                "***********SCENARIO {} END**********************",
                scenarioItem.value("title", ""));
            if (scenarioItem.find("delay") != scenarioItem.end())
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(scenarioItem["delay"].get<int>()));
        }
    }

private:
    void handleReadOperation(const json& scenarioItem)
    {
        const auto& registers = scenarioItem["registers"];

        for (const auto& registerItem : registers)
        {
            std::uint16_t kRegisterNum{};
            if (registerItem.is_array())
            {
                kRegisterNum = stringToRegister(registerItem.at(0).get<std::string>());
            }
            else
            {
                kRegisterNum = stringToRegister(registerItem.get<std::string>());
            }

            auto registerRead{m_pModbusDevice->readRegister(kRegisterNum)};
            if (registerRead)
                spdlog::info("register 0x{:x} value: {}", kRegisterNum, registerRead.value());
            else
                spdlog::error("failed to read from {}", kRegisterNum);
        }
    }

    void handleWriteOperation(const json& scenarioItem)
    {
        const auto& registers = scenarioItem["registers"];
        for (const auto& registerItem : registers)
        {
            auto registerNum = stringToRegister(registerItem[0].get<std::string>());
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
