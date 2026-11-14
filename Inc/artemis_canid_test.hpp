/* ==============================================================================
 * CAN ID and Message Definitions (artemis_canid.hpp)
 *
 * GENERATION METADATA:
 *   Generated on: 2026-11-14 07:09:19 UTC
 *   Generator:    tools/toHeader.py
 *   Source file:  PSR_Artemis27.dbc
 *
 * OVERVIEW:
 *   This file contains compile-time definitions of all CAN message IDs,
 *   signal layouts, bit-fields, and accessors for the vehicle CAN bus.
 *   All structures and accessors in this file are evaluated at compile time.
 *
 * HOW IT WORKS:
 *   1. Retrieve CAN ID for a message:
 *      uint32_t can_id = getID(MessageID::MotorBusVoltageCurrent);
 *
 *   2. Retrieve full MessageDefinition:
 *      const MessageDefinition& def = getMessageDefinition(MessageID::BmsSafetyCritical);
 *
 *   3. Retrieve SignalDefinition (Type-Safe):
 *      const SignalDefinition& sig = getSignal(BmsSafetyCriticalSignal::PackVoltage);
 *
 *   4. Endianness normalization on RX buffer:
 *      dataEndian(buffer, MessageID::BmsSafetyCritical);
 *
 * DO NOT EDIT DIRECTLY — Regenerate using tools/toDbc.py
 * ============================================================================== */

#pragma once
#if defined(STM32_PROCESSOR)
#include <cstdint>
#include <cstddef>
#elif defined(__has_include) && __has_include("pico/stdlib.h")
#include "pico/stdlib.h"
#else
#include <cstdint>
#include <cstddef>
#endif
#include <algorithm>
#include <cmath>

template<typename T, size_t N>
constexpr size_t array_size(const T (&)[N]) { return N; }

enum class Endianness
{
    Little,
    Big
};

struct BitDefinition
{
    const char* name;
    uint8_t bitIndex;
};

struct SignalDefinition
{
    const char* name;
    uint8_t startBit;
    uint8_t length;
    Endianness endian;

    const BitDefinition* bitDefinitions;
    size_t bitDefinitionCount;
};

struct MessageDefinition
{
    const char* name;
    uint32_t id;
    uint16_t periodMs;

    const SignalDefinition* signals;
    size_t signalCount;
};

// ─── 0x200  BmsSafetyCritical ─────────────────────────────────────────────────────

enum class BmsSafetyCriticalDtcFlags1Bit : uint8_t
{
    DischargeLimitEnforcementFault = 0,
    ChargerSafetyRelayFault = 1,
    InternalHardwareFault = 2,
    InternalHeatsinkThermistorFault = 3,
    InternalSoftwareFault = 4,
    HighestCellVoltageTooLowFault = 5,
    LowestCellVoltageTooLowFault = 6,
    PackTooHotFault = 7,
};

inline constexpr BitDefinition bms_safety_critical_dtc_flags1_bits[] = {
    {"DischargeLimitEnforcementFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::DischargeLimitEnforcementFault)},
    {"ChargerSafetyRelayFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::ChargerSafetyRelayFault)},
    {"InternalHardwareFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::InternalHardwareFault)},
    {"InternalHeatsinkThermistorFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::InternalHeatsinkThermistorFault)},
    {"InternalSoftwareFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::InternalSoftwareFault)},
    {"HighestCellVoltageTooLowFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::HighestCellVoltageTooLowFault)},
    {"LowestCellVoltageTooLowFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::LowestCellVoltageTooLowFault)},
    {"PackTooHotFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags1Bit::PackTooHotFault)},
};

enum class BmsSafetyCriticalDtcFlags21Bit : uint8_t
{
    InternalCommunicationFault = 0,
    CellBalancingStuckOffFault = 1,
    WeakCellFault = 2,
    LowCellVoltageFault = 3,
    OpenWiringFault = 4,
    CurrentSensorFault = 5,
    HighestCellVoltageAbove5vFault = 6,
    CellAsicFault = 7,
};

inline constexpr BitDefinition bms_safety_critical_dtc_flags21_bits[] = {
    {"InternalCommunicationFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::InternalCommunicationFault)},
    {"CellBalancingStuckOffFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::CellBalancingStuckOffFault)},
    {"WeakCellFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::WeakCellFault)},
    {"LowCellVoltageFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::LowCellVoltageFault)},
    {"OpenWiringFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::OpenWiringFault)},
    {"CurrentSensorFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::CurrentSensorFault)},
    {"HighestCellVoltageAbove5vFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::HighestCellVoltageAbove5vFault)},
    {"CellAsicFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags21Bit::CellAsicFault)},
};

enum class BmsSafetyCriticalDtcFlags22Bit : uint8_t
{
    WeakPackFault = 0,
    FanMonitorFault = 1,
    ThermistorFault = 2,
    ExternalCommunicationFault = 3,
    RedundantPowerSupplyFault = 4,
    HighVoltageIsolationFault = 5,
    InputPowerSupplyFault = 6,
    ChargeLimitEnforcementFault = 7,
};

inline constexpr BitDefinition bms_safety_critical_dtc_flags22_bits[] = {
    {"WeakPackFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::WeakPackFault)},
    {"FanMonitorFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::FanMonitorFault)},
    {"ThermistorFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::ThermistorFault)},
    {"ExternalCommunicationFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::ExternalCommunicationFault)},
    {"RedundantPowerSupplyFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::RedundantPowerSupplyFault)},
    {"HighVoltageIsolationFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::HighVoltageIsolationFault)},
    {"InputPowerSupplyFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::InputPowerSupplyFault)},
    {"ChargeLimitEnforcementFault", static_cast<uint8_t>(BmsSafetyCriticalDtcFlags22Bit::ChargeLimitEnforcementFault)},
};

enum class BmsSafetyCriticalRelayState1Bit : uint8_t
{
    DischargeRelayEnabled = 0,
    ChargeRelayEnabled = 1,
    ChargerSafetyEnabled = 2,
    DtcIndicatorActive = 3,
    MultiPurposeInputStatus = 4,
    AlwaysOnStatus = 5,
    IsReadyStatus = 6,
    IsChargingStatus = 7,
};

inline constexpr BitDefinition bms_safety_critical_relay_state1_bits[] = {
    {"DischargeRelayEnabled", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::DischargeRelayEnabled)},
    {"ChargeRelayEnabled", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::ChargeRelayEnabled)},
    {"ChargerSafetyEnabled", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::ChargerSafetyEnabled)},
    {"DtcIndicatorActive", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::DtcIndicatorActive)},
    {"MultiPurposeInputStatus", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::MultiPurposeInputStatus)},
    {"AlwaysOnStatus", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::AlwaysOnStatus)},
    {"IsReadyStatus", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::IsReadyStatus)},
    {"IsChargingStatus", static_cast<uint8_t>(BmsSafetyCriticalRelayState1Bit::IsChargingStatus)},
};

enum class BmsSafetyCriticalRelayState2Bit : uint8_t
{
    MultiPurposeInput2Status = 0,
    MultiPurposeInput3Status = 1,
    Reserved = 2,
    MultiPurposeOutput2Status = 3,
    MultiPurposeOutput3Status = 4,
    MultiPurposeOutput4Status = 5,
    MultiPurposeEnableStatus = 6,
    MultiPurposeOutput1Status = 7,
};

inline constexpr BitDefinition bms_safety_critical_relay_state2_bits[] = {
    {"MultiPurposeInput2Status", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeInput2Status)},
    {"MultiPurposeInput3Status", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeInput3Status)},
    {"Reserved", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::Reserved)},
    {"MultiPurposeOutput2Status", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeOutput2Status)},
    {"MultiPurposeOutput3Status", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeOutput3Status)},
    {"MultiPurposeOutput4Status", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeOutput4Status)},
    {"MultiPurposeEnableStatus", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeEnableStatus)},
    {"MultiPurposeOutput1Status", static_cast<uint8_t>(BmsSafetyCriticalRelayState2Bit::MultiPurposeOutput1Status)},
};

enum class BmsSafetyCriticalFailsafeStatusesBit : uint8_t
{
    VoltageFailsafeActive = 0,
    CurrentFailsafeActive = 1,
    RelayFailsafeActive = 2,
    CellBalancingActive = 3,
    ChargeInterlockFailsafeActive = 4,
    ThermistorBValueTableInvalid = 5,
    InputPowerSupplyFailsafeActive = 6,
    Reserved = 7,
};

inline constexpr BitDefinition bms_safety_critical_failsafe_statuses_bits[] = {
    {"VoltageFailsafeActive", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::VoltageFailsafeActive)},
    {"CurrentFailsafeActive", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::CurrentFailsafeActive)},
    {"RelayFailsafeActive", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::RelayFailsafeActive)},
    {"CellBalancingActive", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::CellBalancingActive)},
    {"ChargeInterlockFailsafeActive", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::ChargeInterlockFailsafeActive)},
    {"ThermistorBValueTableInvalid", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::ThermistorBValueTableInvalid)},
    {"InputPowerSupplyFailsafeActive", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::InputPowerSupplyFailsafeActive)},
    {"Reserved", static_cast<uint8_t>(BmsSafetyCriticalFailsafeStatusesBit::Reserved)},
};

inline constexpr SignalDefinition bms_safety_critical_signals[] = {
    {"DTC_Flags_1", 0, 8, Endianness::Big, bms_safety_critical_dtc_flags1_bits, array_size(bms_safety_critical_dtc_flags1_bits)},
    {"DTC_Flags_2_1", 8, 8, Endianness::Big, bms_safety_critical_dtc_flags21_bits, array_size(bms_safety_critical_dtc_flags21_bits)},
    {"DTC_Flags_2_2", 16, 8, Endianness::Big, bms_safety_critical_dtc_flags22_bits, array_size(bms_safety_critical_dtc_flags22_bits)},
    {"Relay_State_1", 24, 8, Endianness::Big, bms_safety_critical_relay_state1_bits, array_size(bms_safety_critical_relay_state1_bits)},
    {"Relay_State_2", 32, 8, Endianness::Big, bms_safety_critical_relay_state2_bits, array_size(bms_safety_critical_relay_state2_bits)},
    {"Failsafe_Statuses", 40, 8, Endianness::Big, bms_safety_critical_failsafe_statuses_bits, array_size(bms_safety_critical_failsafe_statuses_bits)},
    {"Pack_SOC", 48, 8, Endianness::Big, nullptr, 0},
    {"Adaptive_Pack_SOC", 56, 8, Endianness::Big, nullptr, 0},
};

// ─── 0x201  BmsPackVoltageAndEnergy ───────────────────────────────────────────────

inline constexpr SignalDefinition bms_pack_voltage_and_energy_signals[] = {
    {"Pack_kW_Power", 0, 16, Endianness::Big, nullptr, 0},
    {"Pack_Amphours", 16, 16, Endianness::Big, nullptr, 0},
    {"Pack_Current", 32, 16, Endianness::Big, nullptr, 0},
    {"Pack_Voltage", 48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x202  BmsTemperatureCurrentPower ────────────────────────────────────────────

inline constexpr SignalDefinition bms_temperature_current_power_signals[] = {
    {"Pack_DCL", 0, 8, Endianness::Big, nullptr, 0},
    {"Pack_CCL", 8, 8, Endianness::Big, nullptr, 0},
    {"Low_Temperature", 16, 8, Endianness::Big, nullptr, 0},
    {"High_Temperature", 24, 8, Endianness::Big, nullptr, 0},
    {"Current_Limits_Status", 32, 16, Endianness::Big, nullptr, 0},
    {"Adaptive_Amphours", 48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x300  PowerDistroDisplay ────────────────────────────────────────────────────

enum class PowerDistroDisplayDisplayFlagsPowerMonitorBit : uint8_t
{
    DistroBad = 0,
    MainHardwareDetectedFault = 1,
    AuxHardwareDetectedFault = 2,
    MainPowerMonitorI2cError = 3,
    AuxPowerMonitorI2cError = 4,
    PowerDistroPrecharge = 5,
    ContactorFault = 6,
    Empty7 = 7,
};

inline constexpr BitDefinition power_distro_display_display_flags_power_monitor_bits[] = {
    {"DistroBad", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::DistroBad)},
    {"MainHardwareDetectedFault", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::MainHardwareDetectedFault)},
    {"AuxHardwareDetectedFault", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::AuxHardwareDetectedFault)},
    {"MainPowerMonitorI2cError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::MainPowerMonitorI2cError)},
    {"AuxPowerMonitorI2cError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::AuxPowerMonitorI2cError)},
    {"PowerDistroPrecharge", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::PowerDistroPrecharge)},
    {"ContactorFault", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::ContactorFault)},
    {"Empty7", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsPowerMonitorBit::Empty7)},
};

enum class PowerDistroDisplayDisplayFlagsMainBatteryBit : uint8_t
{
    MainOverVoltageError = 0,
    MainUnderVoltageError = 1,
    MainOverCurrentError = 2,
    MainUnderCurrentError = 3,
    MainOverVoltageWarning = 4,
    MainUnderVoltageWarning = 5,
    MainOverCurrentWarning = 6,
    MainUnderCurrentWarning = 7,
};

inline constexpr BitDefinition power_distro_display_display_flags_main_battery_bits[] = {
    {"MainOverVoltageError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainOverVoltageError)},
    {"MainUnderVoltageError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainUnderVoltageError)},
    {"MainOverCurrentError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainOverCurrentError)},
    {"MainUnderCurrentError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainUnderCurrentError)},
    {"MainOverVoltageWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainOverVoltageWarning)},
    {"MainUnderVoltageWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainUnderVoltageWarning)},
    {"MainOverCurrentWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainOverCurrentWarning)},
    {"MainUnderCurrentWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsMainBatteryBit::MainUnderCurrentWarning)},
};

enum class PowerDistroDisplayDisplayFlagsAuxBatteryBit : uint8_t
{
    AuxOverVoltageError = 0,
    AuxUnderVoltageError = 1,
    AuxOverCurrentError = 2,
    AuxUnderCurrentError = 3,
    AuxOverVoltageWarning = 4,
    AuxUnderVoltageWarning = 5,
    AuxOverCurrentWarning = 6,
    AuxUnderCurrentWarning = 7,
};

inline constexpr BitDefinition power_distro_display_display_flags_aux_battery_bits[] = {
    {"AuxOverVoltageError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxOverVoltageError)},
    {"AuxUnderVoltageError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxUnderVoltageError)},
    {"AuxOverCurrentError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxOverCurrentError)},
    {"AuxUnderCurrentError", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxUnderCurrentError)},
    {"AuxOverVoltageWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxOverVoltageWarning)},
    {"AuxUnderVoltageWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxUnderVoltageWarning)},
    {"AuxOverCurrentWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxOverCurrentWarning)},
    {"AuxUnderCurrentWarning", static_cast<uint8_t>(PowerDistroDisplayDisplayFlagsAuxBatteryBit::AuxUnderCurrentWarning)},
};

inline constexpr SignalDefinition power_distro_display_signals[] = {
    {"Display_Flags_Power_Monitor", 0, 8, Endianness::Big, power_distro_display_display_flags_power_monitor_bits, array_size(power_distro_display_display_flags_power_monitor_bits)},
    {"Display_Flags_Main_Battery", 8, 8, Endianness::Big, power_distro_display_display_flags_main_battery_bits, array_size(power_distro_display_display_flags_main_battery_bits)},
    {"Display_Flags_Aux_Battery", 16, 8, Endianness::Big, power_distro_display_display_flags_aux_battery_bits, array_size(power_distro_display_display_flags_aux_battery_bits)},
};

// ─── 0x400  MotorControllerId ─────────────────────────────────────────────────────

inline constexpr SignalDefinition motor_controller_id_signals[] = {
    {"Prohelion_ID", 0, 32, Endianness::Little, nullptr, 0},
    {"Serial_Number", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x401  MotorControllerErrors ─────────────────────────────────────────────────

enum class MotorControllerErrorsMcLimitFlagsBit : uint8_t
{
    OutputVoltagePwm = 0,
    MotorCurrent = 1,
    Velocity = 2,
    BusCurrent = 3,
    BusVoltageUpperLimit = 4,
    BusVoltageLowerLimit = 5,
    IpmMotorTemperature = 6,
    Reserved = 7,
};

inline constexpr BitDefinition motor_controller_errors_mc_limit_flags_bits[] = {
    {"OutputVoltagePwm", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::OutputVoltagePwm)},
    {"MotorCurrent", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::MotorCurrent)},
    {"Velocity", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::Velocity)},
    {"BusCurrent", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::BusCurrent)},
    {"BusVoltageUpperLimit", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::BusVoltageUpperLimit)},
    {"BusVoltageLowerLimit", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::BusVoltageLowerLimit)},
    {"IpmMotorTemperature", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::IpmMotorTemperature)},
    {"Reserved", static_cast<uint8_t>(MotorControllerErrorsMcLimitFlagsBit::Reserved)},
};

enum class MotorControllerErrorsMcErrorFlags1Bit : uint8_t
{
    HardwareOverCurrent = 0,
    SoftwareOverCurrent = 1,
    DcBusOverVoltage = 2,
    BadMotorPositionHallSequence = 3,
    WatchdogCausedLastReset = 4,
    ConfigReadError = 5,
    Rail15vUnderVoltageLockOut = 6,
    DesaturationFault = 7,
};

inline constexpr BitDefinition motor_controller_errors_mc_error_flags1_bits[] = {
    {"HardwareOverCurrent", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::HardwareOverCurrent)},
    {"SoftwareOverCurrent", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::SoftwareOverCurrent)},
    {"DcBusOverVoltage", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::DcBusOverVoltage)},
    {"BadMotorPositionHallSequence", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::BadMotorPositionHallSequence)},
    {"WatchdogCausedLastReset", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::WatchdogCausedLastReset)},
    {"ConfigReadError", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::ConfigReadError)},
    {"Rail15vUnderVoltageLockOut", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::Rail15vUnderVoltageLockOut)},
    {"DesaturationFault", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags1Bit::DesaturationFault)},
};

enum class MotorControllerErrorsMcErrorFlags2Bit : uint8_t
{
    SpeedOver115Percent = 0,
    Reserved1 = 1,
    Reserved2 = 2,
    Reserved3 = 3,
    Reserved4 = 4,
    Reserved5 = 5,
    Reserved6 = 6,
    Reserved7 = 7,
};

inline constexpr BitDefinition motor_controller_errors_mc_error_flags2_bits[] = {
    {"SpeedOver115Percent", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::SpeedOver115Percent)},
    {"Reserved1", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved1)},
    {"Reserved2", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved2)},
    {"Reserved3", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved3)},
    {"Reserved4", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved4)},
    {"Reserved5", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved5)},
    {"Reserved6", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved6)},
    {"Reserved7", static_cast<uint8_t>(MotorControllerErrorsMcErrorFlags2Bit::Reserved7)},
};

inline constexpr SignalDefinition motor_controller_errors_signals[] = {
    {"MC_Limit_Flags", 0, 8, Endianness::Big, motor_controller_errors_mc_limit_flags_bits, array_size(motor_controller_errors_mc_limit_flags_bits)},
    {"Reserved", 8, 8, Endianness::Big, nullptr, 0},
    {"MC_Error_Flags_1", 16, 8, Endianness::Big, motor_controller_errors_mc_error_flags1_bits, array_size(motor_controller_errors_mc_error_flags1_bits)},
    {"MC_Error_Flags_2", 24, 8, Endianness::Big, motor_controller_errors_mc_error_flags2_bits, array_size(motor_controller_errors_mc_error_flags2_bits)},
    {"Active_Motor_Index", 32, 16, Endianness::Big, nullptr, 0},
    {"CAN_TX_Error_Count", 48, 8, Endianness::Big, nullptr, 0},
    {"CAN_RX_Error_Count", 56, 8, Endianness::Big, nullptr, 0},
};

// ─── 0x402  MotorBusVoltageCurrent ────────────────────────────────────────────────

inline constexpr SignalDefinition motor_bus_voltage_current_signals[] = {
    {"Bus_Voltage", 0, 32, Endianness::Little, nullptr, 0},
    {"Bus_Current", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x403  MotorSpeed ────────────────────────────────────────────────────────────

inline constexpr SignalDefinition motor_speed_signals[] = {
    {"Motor_Velocity", 0, 32, Endianness::Little, nullptr, 0},
    {"Vehicle_Velocity", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x404  PhaseCurrentMeasurement ───────────────────────────────────────────────

inline constexpr SignalDefinition phase_current_measurement_signals[] = {
    {"PhaseCurrentB", 0, 32, Endianness::Little, nullptr, 0},
    {"PhaseCurrentC", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x405  MotorVoltageVectorMeasurement ─────────────────────────────────────────

inline constexpr SignalDefinition motor_voltage_vector_measurement_signals[] = {
    {"Vq", 0, 32, Endianness::Little, nullptr, 0},
    {"Vd", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x406  MotorCurrentVectorMeasurement ─────────────────────────────────────────

inline constexpr SignalDefinition motor_current_vector_measurement_signals[] = {
    {"Iq", 0, 32, Endianness::Little, nullptr, 0},
    {"Id", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x407  BackEMFMeasurementPrediction ──────────────────────────────────────────

inline constexpr SignalDefinition back_emfmeasurement_prediction_signals[] = {
    {"BEMFq", 0, 32, Endianness::Little, nullptr, 0},
    {"BEMFd", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x408  VoltageRail15VMeasurement ─────────────────────────────────────────────

inline constexpr SignalDefinition voltage_rail15_vmeasurement_signals[] = {
    {"ReservedSupply15V", 0, 32, Endianness::Little, nullptr, 0},
    {"Supply15V", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x409  VoltageRail3V31V9Measurement ──────────────────────────────────────────

inline constexpr SignalDefinition voltage_rail3_v31_v9_measurement_signals[] = {
    {"Supply1V9", 0, 32, Endianness::Little, nullptr, 0},
    {"Supply3V3", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40A  Reserved0A ────────────────────────────────────────────────────────────

inline constexpr SignalDefinition reserved0_a_signals[] = {
    {"Reserved0A0", 0, 32, Endianness::Little, nullptr, 0},
    {"Reserved0A1", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40B  MotorTemperature ──────────────────────────────────────────────────────

inline constexpr SignalDefinition motor_temperature_signals[] = {
    {"Motor_Temp", 0, 32, Endianness::Little, nullptr, 0},
    {"Heat_Sink_Temp", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40C  DspBoardTempMeasurement ───────────────────────────────────────────────

inline constexpr SignalDefinition dsp_board_temp_measurement_signals[] = {
    {"DspBoardTemp", 0, 32, Endianness::Little, nullptr, 0},
    {"ReservedDspBoardTemp", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40D  Reserved0D ────────────────────────────────────────────────────────────

inline constexpr SignalDefinition reserved0_d_signals[] = {
    {"Reserved0D0", 0, 32, Endianness::Little, nullptr, 0},
    {"Reserved0D1", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40E  OdometerBusAhMeasurement ──────────────────────────────────────────────

inline constexpr SignalDefinition odometer_bus_ah_measurement_signals[] = {
    {"Odometer", 0, 32, Endianness::Little, nullptr, 0},
    {"DCBusAh", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40F  WS22_ModeCommand ──────────────────────────────────────────────────────

enum class Ws22ModeCommandModeBit : uint8_t
{
    Normal = 0,
    PhasorSense = 1,
    ParameterExtraction = 2,
    InductionMotorExtraction = 3,
};

inline constexpr BitDefinition ws22_mode_command_mode_bits[] = {
    {"Normal", static_cast<uint8_t>(Ws22ModeCommandModeBit::Normal)},
    {"PhasorSense", static_cast<uint8_t>(Ws22ModeCommandModeBit::PhasorSense)},
    {"ParameterExtraction", static_cast<uint8_t>(Ws22ModeCommandModeBit::ParameterExtraction)},
    {"InductionMotorExtraction", static_cast<uint8_t>(Ws22ModeCommandModeBit::InductionMotorExtraction)},
};

enum class Ws22ModeCommandStartBit : uint8_t
{
    Select = 0,
    Start = 1,
};

inline constexpr BitDefinition ws22_mode_command_start_bits[] = {
    {"Select", static_cast<uint8_t>(Ws22ModeCommandStartBit::Select)},
    {"Start", static_cast<uint8_t>(Ws22ModeCommandStartBit::Start)},
};

inline constexpr SignalDefinition ws22_mode_command_signals[] = {
    {"Reserved", 0, 32, Endianness::Little, nullptr, 0},
    {"Mode", 32, 16, Endianness::Little, ws22_mode_command_mode_bits, array_size(ws22_mode_command_mode_bits)},
    {"Start", 48, 16, Endianness::Little, ws22_mode_command_start_bits, array_size(ws22_mode_command_start_bits)},
};

// ─── 0x410  WS22_AcquisitionStatus ────────────────────────────────────────────────

inline constexpr SignalDefinition ws22_acquisition_status_signals[] = {
    {"Word0", 0, 16, Endianness::Little, nullptr, 0},
    {"Word1", 16, 16, Endianness::Little, nullptr, 0},
    {"Word2", 32, 16, Endianness::Little, nullptr, 0},
    {"Word3", 48, 16, Endianness::Little, nullptr, 0},
};

// ─── 0x411  WS22_AcquisitionSamples ───────────────────────────────────────────────

inline constexpr SignalDefinition ws22_acquisition_samples_signals[] = {
    {"Sample0", 0, 16, Endianness::Little, nullptr, 0},
    {"Sample1", 16, 16, Endianness::Little, nullptr, 0},
    {"Sample2", 32, 16, Endianness::Little, nullptr, 0},
    {"Sample3", 48, 16, Endianness::Little, nullptr, 0},
};

// ─── 0x412  ConfigTransferControl ─────────────────────────────────────────────────

enum class ConfigTransferControlBlockAckStatusBit : uint8_t
{
    BlockOk = 0,
    Complete = 1,
};

inline constexpr BitDefinition config_transfer_control_block_ack_status_bits[] = {
    {"BlockOk", static_cast<uint8_t>(ConfigTransferControlBlockAckStatusBit::BlockOk)},
    {"Complete", static_cast<uint8_t>(ConfigTransferControlBlockAckStatusBit::Complete)},
};

enum class ConfigTransferControlCloseKindBit : uint8_t
{
    Close = 0,
    Abort = 1,
    CloseAck = 2,
};

inline constexpr BitDefinition config_transfer_control_close_kind_bits[] = {
    {"Close", static_cast<uint8_t>(ConfigTransferControlCloseKindBit::Close)},
    {"Abort", static_cast<uint8_t>(ConfigTransferControlCloseKindBit::Abort)},
    {"CloseAck", static_cast<uint8_t>(ConfigTransferControlCloseKindBit::CloseAck)},
};

enum class ConfigTransferControlRequestStreamBit : uint8_t
{
    Fixed = 0,
    Stream = 1,
};

inline constexpr BitDefinition config_transfer_control_request_stream_bits[] = {
    {"Fixed", static_cast<uint8_t>(ConfigTransferControlRequestStreamBit::Fixed)},
    {"Stream", static_cast<uint8_t>(ConfigTransferControlRequestStreamBit::Stream)},
};

enum class ConfigTransferControlRequestResponderSendsBit : uint8_t
{
    Write = 0,
    Read = 1,
};

inline constexpr BitDefinition config_transfer_control_request_responder_sends_bits[] = {
    {"Write", static_cast<uint8_t>(ConfigTransferControlRequestResponderSendsBit::Write)},
    {"Read", static_cast<uint8_t>(ConfigTransferControlRequestResponderSendsBit::Read)},
};

inline constexpr SignalDefinition config_transfer_control_signals[] = {
    {"Op", 0, 16, Endianness::Little, nullptr, 0},
    {"ACTMOT_Tag", 16, 32, Endianness::Little, nullptr, 0},
    {"Size_Bytes", 16, 16, Endianness::Little, nullptr, 0},
    {"Request_Address", 16, 16, Endianness::Little, nullptr, 0},
    {"BadSeq_FramesReceived", 16, 16, Endianness::Little, nullptr, 0},
    {"BlockAck_Status", 16, 16, Endianness::Little, config_transfer_control_block_ack_status_bits, array_size(config_transfer_control_block_ack_status_bits)},
    {"ParamAck_BlockSize", 16, 16, Endianness::Little, nullptr, 0},
    {"Close_Kind", 16, 16, Endianness::Little, config_transfer_control_close_kind_bits, array_size(config_transfer_control_close_kind_bits)},
    {"Init_BlockSize", 16, 16, Endianness::Little, nullptr, 0},
    {"Request_Size", 32, 16, Endianness::Little, nullptr, 0},
    {"BadSeq_FramesPerBlock", 32, 16, Endianness::Little, nullptr, 0},
    {"ParamAck_Timeout", 32, 16, Endianness::Little, nullptr, 0},
    {"Init_Timeout", 32, 16, Endianness::Little, nullptr, 0},
    {"BlockAck_Toggle", 40, 1, Endianness::Little, nullptr, 0},
    {"ACTMOT_MotorIndex", 48, 16, Endianness::Little, nullptr, 0},
    {"Request_Stream", 48, 8, Endianness::Little, config_transfer_control_request_stream_bits, array_size(config_transfer_control_request_stream_bits)},
    {"ParamAck_Retries", 48, 16, Endianness::Little, nullptr, 0},
    {"Init_Retries", 48, 16, Endianness::Little, nullptr, 0},
    {"Request_ResponderSends", 56, 8, Endianness::Little, config_transfer_control_request_responder_sends_bits, array_size(config_transfer_control_request_responder_sends_bits)},
};

// ─── 0x414  ConfigTransferData ────────────────────────────────────────────────────

inline constexpr SignalDefinition config_transfer_data_signals[] = {
    {"Sequence", 0, 7, Endianness::Little, nullptr, 0},
    {"Toggle", 7, 1, Endianness::Little, nullptr, 0},
    {"Payload1", 8, 8, Endianness::Little, nullptr, 0},
    {"Payload2", 16, 8, Endianness::Little, nullptr, 0},
    {"Payload3", 24, 8, Endianness::Little, nullptr, 0},
    {"Payload4", 32, 8, Endianness::Little, nullptr, 0},
    {"Payload5", 40, 8, Endianness::Little, nullptr, 0},
    {"Payload6", 48, 8, Endianness::Little, nullptr, 0},
    {"Payload7", 56, 8, Endianness::Little, nullptr, 0},
};

// ─── 0x417  SlipSpeedMeasurement ──────────────────────────────────────────────────

inline constexpr SignalDefinition slip_speed_measurement_signals[] = {
    {"SlipSpeed", 0, 32, Endianness::Little, nullptr, 0},
    {"ReservedSlipSpeed", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x501  MotorCurrentVelocityControl ───────────────────────────────────────────

inline constexpr SignalDefinition motor_current_velocity_control_signals[] = {
    {"Motor_Velocity_Setpoint", 0, 32, Endianness::Little, nullptr, 0},
    {"Motor_Current_Setpoint", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x502  BusCurrentControl ─────────────────────────────────────────────────────

inline constexpr SignalDefinition bus_current_control_signals[] = {
    {"Reserved", 0, 32, Endianness::Little, nullptr, 0},
    {"Bus_Current_Setpoint", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x503  ResetCommand ──────────────────────────────────────────────────────────

inline constexpr SignalDefinition reset_command_signals[] = {
    {"Unused", 0, 64, Endianness::Little, nullptr, 0},
};

// ─── 0x601  BmsCellData ───────────────────────────────────────────────────────────

inline constexpr SignalDefinition bms_cell_data_signals[] = {
    {"Low_Cell_Voltage", 0, 16, Endianness::Big, nullptr, 0},
    {"High_Cell_Voltage", 16, 16, Endianness::Big, nullptr, 0},
    {"Low_Cell_Voltage_ID", 32, 8, Endianness::Big, nullptr, 0},
    {"High_Cell_Voltage_ID", 40, 8, Endianness::Big, nullptr, 0},
    {"Low_Thermistor_ID", 48, 8, Endianness::Big, nullptr, 0},
    {"High_Thermistor_ID", 56, 8, Endianness::Big, nullptr, 0},
};

// ─── 0x602  BmsThermistorBroadcast ────────────────────────────────────────────────

inline constexpr SignalDefinition bms_thermistor_broadcast_signals[] = {
    {"Thermistor_Data", 0, 64, Endianness::Big, nullptr, 0},
};

// ─── 0x603  BmsCellDataBroadcast ──────────────────────────────────────────────────

inline constexpr SignalDefinition bms_cell_data_broadcast_signals[] = {
    {"Cell_ID", 0, 8, Endianness::Big, nullptr, 0},
    {"Instant_Voltage", 8, 16, Endianness::Big, nullptr, 0},
    {"Internal_Resistance", 24, 16, Endianness::Big, nullptr, 0},
    {"Open_Voltage", 40, 16, Endianness::Big, nullptr, 0},
    {"Checksum", 56, 8, Endianness::Big, nullptr, 0},
};

// ─── 0x700  PeripheralControl ─────────────────────────────────────────────────────

enum class PeripheralControlDistroControlBit : uint8_t
{
    LeftLights = 0,
    RightLights = 1,
    HazardLights = 2,
    Horn = 3,
    Brake = 4,
    Empty5 = 5,
    Empty6 = 6,
    Empty7 = 7,
};

inline constexpr BitDefinition peripheral_control_distro_control_bits[] = {
    {"LeftLights", static_cast<uint8_t>(PeripheralControlDistroControlBit::LeftLights)},
    {"RightLights", static_cast<uint8_t>(PeripheralControlDistroControlBit::RightLights)},
    {"HazardLights", static_cast<uint8_t>(PeripheralControlDistroControlBit::HazardLights)},
    {"Horn", static_cast<uint8_t>(PeripheralControlDistroControlBit::Horn)},
    {"Brake", static_cast<uint8_t>(PeripheralControlDistroControlBit::Brake)},
    {"Empty5", static_cast<uint8_t>(PeripheralControlDistroControlBit::Empty5)},
    {"Empty6", static_cast<uint8_t>(PeripheralControlDistroControlBit::Empty6)},
    {"Empty7", static_cast<uint8_t>(PeripheralControlDistroControlBit::Empty7)},
};

inline constexpr SignalDefinition peripheral_control_signals[] = {
    {"Distro_Control", 0, 8, Endianness::Big, peripheral_control_distro_control_bits, array_size(peripheral_control_distro_control_bits)},
};

// ─── 0x7DF  Orion_FunctionalRequest ───────────────────────────────────────────────

enum class OrionFunctionalRequestPCIBit : uint8_t
{
    SingleFrame1 = 1,
    SingleFrame2 = 2,
    SingleFrame3 = 3,
    SingleFrame4 = 4,
    SingleFrame5 = 5,
    SingleFrame6 = 6,
    SingleFrame7 = 7,
    FirstFrame = 16,
    Consecutive0 = 32,
    Consecutive1 = 33,
    Consecutive2 = 34,
    Consecutive3 = 35,
    Consecutive4 = 36,
    Consecutive5 = 37,
    Consecutive6 = 38,
    Consecutive7 = 39,
    Consecutive8 = 40,
    Consecutive9 = 41,
    Consecutive10 = 42,
    Consecutive11 = 43,
    Consecutive12 = 44,
    Consecutive13 = 45,
    Consecutive14 = 46,
    Consecutive15 = 47,
    FlowControl = 48,
};

inline constexpr BitDefinition orion_functional_request_pci_bits[] = {
    {"SingleFrame_1", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame1)},
    {"SingleFrame_2", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame2)},
    {"SingleFrame_3", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame3)},
    {"SingleFrame_4", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame4)},
    {"SingleFrame_5", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame5)},
    {"SingleFrame_6", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame6)},
    {"SingleFrame_7", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::SingleFrame7)},
    {"FirstFrame", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::FirstFrame)},
    {"Consecutive_0", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive0)},
    {"Consecutive_1", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive1)},
    {"Consecutive_2", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive2)},
    {"Consecutive_3", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive3)},
    {"Consecutive_4", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive4)},
    {"Consecutive_5", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive5)},
    {"Consecutive_6", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive6)},
    {"Consecutive_7", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive7)},
    {"Consecutive_8", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive8)},
    {"Consecutive_9", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive9)},
    {"Consecutive_10", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive10)},
    {"Consecutive_11", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive11)},
    {"Consecutive_12", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive12)},
    {"Consecutive_13", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive13)},
    {"Consecutive_14", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive14)},
    {"Consecutive_15", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::Consecutive15)},
    {"FlowControl", static_cast<uint8_t>(OrionFunctionalRequestPCIBit::FlowControl)},
};

enum class OrionFunctionalRequestSfModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    CurrentDataReply = 65,
    FreezeFrameReply = 66,
    StoredDTCsReply = 67,
    ClearDTCsReply = 68,
    Mode05Reply = 69,
    PendingDTCsReply = 71,
    VehicleInfoReply = 73,
    PermanentDTCsReply = 74,
    ResetReply = 81,
    Mode17Reply = 87,
    LockReply = 96,
    ReadPID8Reply = 97,
    ReadPID16Reply = 98,
    ReadPID24Reply = 99,
    SecurityAccessReply = 103,
    WriteReply = 123,
    ProfileReadReply = 124,
    NegativeOrAck = 127,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
    ModeA3Reply = 227,
    ModeB0Reply = 240,
    CalibrateReply = 241,
    LinkStatusReply = 242,
    NvramLogReply = 243,
    WatchdogReply = 245,
};

inline constexpr BitDefinition orion_functional_request_sf_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ProfileRead)},
    {"CurrentData_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::CurrentDataReply)},
    {"FreezeFrame_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::FreezeFrameReply)},
    {"StoredDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::StoredDTCsReply)},
    {"ClearDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ClearDTCsReply)},
    {"Mode05_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Mode05Reply)},
    {"PendingDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::PendingDTCsReply)},
    {"VehicleInfo_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::VehicleInfoReply)},
    {"PermanentDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::PermanentDTCsReply)},
    {"Reset_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ResetReply)},
    {"Mode17_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Mode17Reply)},
    {"Lock_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::LockReply)},
    {"ReadPID8_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ReadPID8Reply)},
    {"ReadPID16_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ReadPID16Reply)},
    {"ReadPID24_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ReadPID24Reply)},
    {"SecurityAccess_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::SecurityAccessReply)},
    {"Write_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::WriteReply)},
    {"ProfileRead_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ProfileReadReply)},
    {"NegativeOrAck", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::NegativeOrAck)},
    {"ModeA3", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::Watchdog)},
    {"ModeA3_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ModeA3Reply)},
    {"ModeB0_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::ModeB0Reply)},
    {"Calibrate_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::CalibrateReply)},
    {"LinkStatus_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::LinkStatusReply)},
    {"NvramLog_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::NvramLogReply)},
    {"Watchdog_Reply", static_cast<uint8_t>(OrionFunctionalRequestSfModeBit::WatchdogReply)},
};

enum class OrionFunctionalRequestSfRejectedModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
};

inline constexpr BitDefinition orion_functional_request_sf_rejected_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ProfileRead)},
    {"ModeA3", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionFunctionalRequestSfRejectedModeBit::Watchdog)},
};

enum class OrionFunctionalRequestSfWriteSubBit : uint8_t
{
    ProfileChunk = 0,
    HistoricalData = 2,
    Commit = 3,
    SetRTC = 4,
    SetPassword = 6,
    SetNickname = 8,
};

inline constexpr BitDefinition orion_functional_request_sf_write_sub_bits[] = {
    {"ProfileChunk", static_cast<uint8_t>(OrionFunctionalRequestSfWriteSubBit::ProfileChunk)},
    {"HistoricalData", static_cast<uint8_t>(OrionFunctionalRequestSfWriteSubBit::HistoricalData)},
    {"Commit", static_cast<uint8_t>(OrionFunctionalRequestSfWriteSubBit::Commit)},
    {"SetRTC", static_cast<uint8_t>(OrionFunctionalRequestSfWriteSubBit::SetRTC)},
    {"SetPassword", static_cast<uint8_t>(OrionFunctionalRequestSfWriteSubBit::SetPassword)},
    {"SetNickname", static_cast<uint8_t>(OrionFunctionalRequestSfWriteSubBit::SetNickname)},
};

enum class OrionFunctionalRequestSfSecurityLevelBit : uint8_t
{
    Tier1SeedRequest = 1,
    Tier1Key = 2,
    Tier2SeedRequest = 5,
    Tier2Key = 6,
};

inline constexpr BitDefinition orion_functional_request_sf_security_level_bits[] = {
    {"Tier1_SeedRequest", static_cast<uint8_t>(OrionFunctionalRequestSfSecurityLevelBit::Tier1SeedRequest)},
    {"Tier1_Key", static_cast<uint8_t>(OrionFunctionalRequestSfSecurityLevelBit::Tier1Key)},
    {"Tier2_SeedRequest", static_cast<uint8_t>(OrionFunctionalRequestSfSecurityLevelBit::Tier2SeedRequest)},
    {"Tier2_Key", static_cast<uint8_t>(OrionFunctionalRequestSfSecurityLevelBit::Tier2Key)},
};

enum class OrionFunctionalRequestSfLivePid8Bit : uint8_t
{
    RUNTIME = 31,
    DtcActiveTime = 77,
    DtcClearTime = 78,
};

inline constexpr BitDefinition orion_functional_request_sf_live_pid8_bits[] = {
    {"RUNTIME", static_cast<uint8_t>(OrionFunctionalRequestSfLivePid8Bit::RUNTIME)},
    {"DTC_ACTIVE_TIME", static_cast<uint8_t>(OrionFunctionalRequestSfLivePid8Bit::DtcActiveTime)},
    {"DTC_CLEAR_TIME", static_cast<uint8_t>(OrionFunctionalRequestSfLivePid8Bit::DtcClearTime)},
};

enum class OrionFunctionalRequestSfResetTypeBit : uint8_t
{
    Restart = 1,
    ClearCycleCount = 10,
    ClearLearnedSOC = 11,
    ClearContactorCount = 12,
};

inline constexpr BitDefinition orion_functional_request_sf_reset_type_bits[] = {
    {"Restart", static_cast<uint8_t>(OrionFunctionalRequestSfResetTypeBit::Restart)},
    {"ClearCycleCount", static_cast<uint8_t>(OrionFunctionalRequestSfResetTypeBit::ClearCycleCount)},
    {"ClearLearnedSOC", static_cast<uint8_t>(OrionFunctionalRequestSfResetTypeBit::ClearLearnedSOC)},
    {"ClearContactorCount", static_cast<uint8_t>(OrionFunctionalRequestSfResetTypeBit::ClearContactorCount)},
};

enum class OrionFunctionalRequestSfInfoTypeBit : uint8_t
{
    FirmwareVersion = 4,
    ProfileCRC = 6,
    Serial = 10,
    Identify = 11,
    LoadedChips = 12,
    Nickname = 18,
    HardwareRevision = 19,
};

inline constexpr BitDefinition orion_functional_request_sf_info_type_bits[] = {
    {"FirmwareVersion", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::FirmwareVersion)},
    {"ProfileCRC", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::ProfileCRC)},
    {"Serial", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::Serial)},
    {"Identify", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::Identify)},
    {"LoadedChips", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::LoadedChips)},
    {"Nickname", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::Nickname)},
    {"HardwareRevision", static_cast<uint8_t>(OrionFunctionalRequestSfInfoTypeBit::HardwareRevision)},
};

enum class OrionFunctionalRequestFfModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    CurrentDataReply = 65,
    FreezeFrameReply = 66,
    StoredDTCsReply = 67,
    ClearDTCsReply = 68,
    Mode05Reply = 69,
    PendingDTCsReply = 71,
    VehicleInfoReply = 73,
    PermanentDTCsReply = 74,
    ResetReply = 81,
    Mode17Reply = 87,
    LockReply = 96,
    ReadPID8Reply = 97,
    ReadPID16Reply = 98,
    ReadPID24Reply = 99,
    SecurityAccessReply = 103,
    WriteReply = 123,
    ProfileReadReply = 124,
    NegativeOrAck = 127,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
    ModeA3Reply = 227,
    ModeB0Reply = 240,
    CalibrateReply = 241,
    LinkStatusReply = 242,
    NvramLogReply = 243,
    WatchdogReply = 245,
};

inline constexpr BitDefinition orion_functional_request_ff_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ProfileRead)},
    {"CurrentData_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::CurrentDataReply)},
    {"FreezeFrame_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::FreezeFrameReply)},
    {"StoredDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::StoredDTCsReply)},
    {"ClearDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ClearDTCsReply)},
    {"Mode05_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Mode05Reply)},
    {"PendingDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::PendingDTCsReply)},
    {"VehicleInfo_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::VehicleInfoReply)},
    {"PermanentDTCs_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::PermanentDTCsReply)},
    {"Reset_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ResetReply)},
    {"Mode17_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Mode17Reply)},
    {"Lock_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::LockReply)},
    {"ReadPID8_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ReadPID8Reply)},
    {"ReadPID16_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ReadPID16Reply)},
    {"ReadPID24_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ReadPID24Reply)},
    {"SecurityAccess_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::SecurityAccessReply)},
    {"Write_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::WriteReply)},
    {"ProfileRead_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ProfileReadReply)},
    {"NegativeOrAck", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::NegativeOrAck)},
    {"ModeA3", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::Watchdog)},
    {"ModeA3_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ModeA3Reply)},
    {"ModeB0_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::ModeB0Reply)},
    {"Calibrate_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::CalibrateReply)},
    {"LinkStatus_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::LinkStatusReply)},
    {"NvramLog_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::NvramLogReply)},
    {"Watchdog_Reply", static_cast<uint8_t>(OrionFunctionalRequestFfModeBit::WatchdogReply)},
};

enum class OrionFunctionalRequestFfWriteSubBit : uint8_t
{
    ProfileChunk = 0,
    HistoricalData = 2,
    Commit = 3,
    SetRTC = 4,
    SetPassword = 6,
    SetNickname = 8,
};

inline constexpr BitDefinition orion_functional_request_ff_write_sub_bits[] = {
    {"ProfileChunk", static_cast<uint8_t>(OrionFunctionalRequestFfWriteSubBit::ProfileChunk)},
    {"HistoricalData", static_cast<uint8_t>(OrionFunctionalRequestFfWriteSubBit::HistoricalData)},
    {"Commit", static_cast<uint8_t>(OrionFunctionalRequestFfWriteSubBit::Commit)},
    {"SetRTC", static_cast<uint8_t>(OrionFunctionalRequestFfWriteSubBit::SetRTC)},
    {"SetPassword", static_cast<uint8_t>(OrionFunctionalRequestFfWriteSubBit::SetPassword)},
    {"SetNickname", static_cast<uint8_t>(OrionFunctionalRequestFfWriteSubBit::SetNickname)},
};

enum class OrionFunctionalRequestFfSecurityLevelBit : uint8_t
{
    Tier1SeedRequest = 1,
    Tier1Key = 2,
    Tier2SeedRequest = 5,
    Tier2Key = 6,
};

inline constexpr BitDefinition orion_functional_request_ff_security_level_bits[] = {
    {"Tier1_SeedRequest", static_cast<uint8_t>(OrionFunctionalRequestFfSecurityLevelBit::Tier1SeedRequest)},
    {"Tier1_Key", static_cast<uint8_t>(OrionFunctionalRequestFfSecurityLevelBit::Tier1Key)},
    {"Tier2_SeedRequest", static_cast<uint8_t>(OrionFunctionalRequestFfSecurityLevelBit::Tier2SeedRequest)},
    {"Tier2_Key", static_cast<uint8_t>(OrionFunctionalRequestFfSecurityLevelBit::Tier2Key)},
};

enum class OrionFunctionalRequestFfResetTypeBit : uint8_t
{
    Restart = 1,
    ClearCycleCount = 10,
    ClearLearnedSOC = 11,
    ClearContactorCount = 12,
};

inline constexpr BitDefinition orion_functional_request_ff_reset_type_bits[] = {
    {"Restart", static_cast<uint8_t>(OrionFunctionalRequestFfResetTypeBit::Restart)},
    {"ClearCycleCount", static_cast<uint8_t>(OrionFunctionalRequestFfResetTypeBit::ClearCycleCount)},
    {"ClearLearnedSOC", static_cast<uint8_t>(OrionFunctionalRequestFfResetTypeBit::ClearLearnedSOC)},
    {"ClearContactorCount", static_cast<uint8_t>(OrionFunctionalRequestFfResetTypeBit::ClearContactorCount)},
};

enum class OrionFunctionalRequestFfInfoTypeBit : uint8_t
{
    FirmwareVersion = 4,
    ProfileCRC = 6,
    Serial = 10,
    Identify = 11,
    LoadedChips = 12,
    Nickname = 18,
    HardwareRevision = 19,
};

inline constexpr BitDefinition orion_functional_request_ff_info_type_bits[] = {
    {"FirmwareVersion", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::FirmwareVersion)},
    {"ProfileCRC", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::ProfileCRC)},
    {"Serial", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::Serial)},
    {"Identify", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::Identify)},
    {"LoadedChips", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::LoadedChips)},
    {"Nickname", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::Nickname)},
    {"HardwareRevision", static_cast<uint8_t>(OrionFunctionalRequestFfInfoTypeBit::HardwareRevision)},
};

enum class OrionFunctionalRequestSfResponseCodeBit : uint8_t
{
    InvalidMode = 17,
    Busy = 33,
    ConditionsNotCorrect = 34,
    AccessDenied = 51,
    InvalidKey = 53,
    Invalid = 64,
    Accepted = 114,
    Resend = 117,
    BadChecksum = 119,
};

inline constexpr BitDefinition orion_functional_request_sf_response_code_bits[] = {
    {"InvalidMode", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::InvalidMode)},
    {"Busy", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::Busy)},
    {"ConditionsNotCorrect", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::ConditionsNotCorrect)},
    {"AccessDenied", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::AccessDenied)},
    {"InvalidKey", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::InvalidKey)},
    {"Invalid", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::Invalid)},
    {"Accepted", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::Accepted)},
    {"Resend", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::Resend)},
    {"BadChecksum", static_cast<uint8_t>(OrionFunctionalRequestSfResponseCodeBit::BadChecksum)},
};

inline constexpr SignalDefinition orion_functional_request_signals[] = {
    {"PCI", 0, 8, Endianness::Little, orion_functional_request_pci_bits, array_size(orion_functional_request_pci_bits)},
    {"FC_BlockSize", 8, 8, Endianness::Little, nullptr, 0},
    {"CF_Data", 15, 56, Endianness::Big, nullptr, 0},
    {"FF_Length", 8, 8, Endianness::Little, nullptr, 0},
    {"SF_Mode", 8, 8, Endianness::Little, orion_functional_request_sf_mode_bits, array_size(orion_functional_request_sf_mode_bits)},
    {"SF_RejectedMode", 16, 8, Endianness::Little, orion_functional_request_sf_rejected_mode_bits, array_size(orion_functional_request_sf_rejected_mode_bits)},
    {"SF_ReadSub", 16, 8, Endianness::Little, nullptr, 0},
    {"SF_WriteSub", 16, 8, Endianness::Little, orion_functional_request_sf_write_sub_bits, array_size(orion_functional_request_sf_write_sub_bits)},
    {"SF_SecurityLevel", 16, 8, Endianness::Little, orion_functional_request_sf_security_level_bits, array_size(orion_functional_request_sf_security_level_bits)},
    {"SF_Pid24", 23, 24, Endianness::Big, nullptr, 0},
    {"SF_LivePid8", 16, 8, Endianness::Little, orion_functional_request_sf_live_pid8_bits, array_size(orion_functional_request_sf_live_pid8_bits)},
    {"SF_ResetType", 16, 8, Endianness::Little, orion_functional_request_sf_reset_type_bits, array_size(orion_functional_request_sf_reset_type_bits)},
    {"SF_InfoType", 16, 8, Endianness::Little, orion_functional_request_sf_info_type_bits, array_size(orion_functional_request_sf_info_type_bits)},
    {"SF_Pid16", 23, 16, Endianness::Big, nullptr, 0},
    {"SF_B2", 16, 8, Endianness::Little, nullptr, 0},
    {"SF_Pid8", 16, 8, Endianness::Little, nullptr, 0},
    {"FC_SeparationTime", 16, 8, Endianness::Little, nullptr, 0},
    {"FF_Mode", 16, 8, Endianness::Little, orion_functional_request_ff_mode_bits, array_size(orion_functional_request_ff_mode_bits)},
    {"FF_RejectedMode", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ReadSub", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_WriteSub", 24, 8, Endianness::Little, orion_functional_request_ff_write_sub_bits, array_size(orion_functional_request_ff_write_sub_bits)},
    {"FF_SecurityLevel", 24, 8, Endianness::Little, orion_functional_request_ff_security_level_bits, array_size(orion_functional_request_ff_security_level_bits)},
    {"FF_Pid24", 31, 24, Endianness::Big, nullptr, 0},
    {"FF_LivePid8", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ResetType", 24, 8, Endianness::Little, orion_functional_request_ff_reset_type_bits, array_size(orion_functional_request_ff_reset_type_bits)},
    {"FF_InfoType", 24, 8, Endianness::Little, orion_functional_request_ff_info_type_bits, array_size(orion_functional_request_ff_info_type_bits)},
    {"FF_Pid16", 31, 16, Endianness::Big, nullptr, 0},
    {"FF_B3", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid8", 24, 8, Endianness::Little, nullptr, 0},
    {"SF_ResponseCode", 24, 8, Endianness::Little, orion_functional_request_sf_response_code_bits, array_size(orion_functional_request_sf_response_code_bits)},
    {"SF_Seed", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_ReadLength", 24, 8, Endianness::Little, nullptr, 0},
    {"SF_WriteChecksum", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_Key", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_B3", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ResponseCode", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Seed", 39, 32, Endianness::Big, nullptr, 0},
    {"FF_ReadLength", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_WriteLength", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Key", 39, 32, Endianness::Big, nullptr, 0},
    {"FF_B4", 32, 8, Endianness::Little, nullptr, 0},
    {"SF_Detail", 32, 8, Endianness::Little, nullptr, 0},
    {"SF_ReadAddress", 39, 16, Endianness::Big, nullptr, 0},
    {"SF_B4", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Detail", 40, 8, Endianness::Little, nullptr, 0},
    {"FF_ReadAddress", 47, 16, Endianness::Big, nullptr, 0},
    {"FF_WriteAddress", 47, 16, Endianness::Big, nullptr, 0},
    {"FF_B5", 40, 8, Endianness::Little, nullptr, 0},
    {"SF_B5", 40, 8, Endianness::Little, nullptr, 0},
    {"FF_B6", 48, 8, Endianness::Little, nullptr, 0},
    {"SF_ReadChecksum", 55, 16, Endianness::Big, nullptr, 0},
    {"SF_B6", 48, 8, Endianness::Little, nullptr, 0},
    {"FF_B7", 56, 8, Endianness::Little, nullptr, 0},
    {"SF_B7", 56, 8, Endianness::Little, nullptr, 0},
};

// ─── 0x7E3  Orion_DiagRequest ─────────────────────────────────────────────────────

enum class OrionDiagRequestPCIBit : uint8_t
{
    SingleFrame1 = 1,
    SingleFrame2 = 2,
    SingleFrame3 = 3,
    SingleFrame4 = 4,
    SingleFrame5 = 5,
    SingleFrame6 = 6,
    SingleFrame7 = 7,
    FirstFrame = 16,
    Consecutive0 = 32,
    Consecutive1 = 33,
    Consecutive2 = 34,
    Consecutive3 = 35,
    Consecutive4 = 36,
    Consecutive5 = 37,
    Consecutive6 = 38,
    Consecutive7 = 39,
    Consecutive8 = 40,
    Consecutive9 = 41,
    Consecutive10 = 42,
    Consecutive11 = 43,
    Consecutive12 = 44,
    Consecutive13 = 45,
    Consecutive14 = 46,
    Consecutive15 = 47,
    FlowControl = 48,
};

inline constexpr BitDefinition orion_diag_request_pci_bits[] = {
    {"SingleFrame_1", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame1)},
    {"SingleFrame_2", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame2)},
    {"SingleFrame_3", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame3)},
    {"SingleFrame_4", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame4)},
    {"SingleFrame_5", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame5)},
    {"SingleFrame_6", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame6)},
    {"SingleFrame_7", static_cast<uint8_t>(OrionDiagRequestPCIBit::SingleFrame7)},
    {"FirstFrame", static_cast<uint8_t>(OrionDiagRequestPCIBit::FirstFrame)},
    {"Consecutive_0", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive0)},
    {"Consecutive_1", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive1)},
    {"Consecutive_2", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive2)},
    {"Consecutive_3", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive3)},
    {"Consecutive_4", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive4)},
    {"Consecutive_5", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive5)},
    {"Consecutive_6", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive6)},
    {"Consecutive_7", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive7)},
    {"Consecutive_8", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive8)},
    {"Consecutive_9", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive9)},
    {"Consecutive_10", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive10)},
    {"Consecutive_11", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive11)},
    {"Consecutive_12", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive12)},
    {"Consecutive_13", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive13)},
    {"Consecutive_14", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive14)},
    {"Consecutive_15", static_cast<uint8_t>(OrionDiagRequestPCIBit::Consecutive15)},
    {"FlowControl", static_cast<uint8_t>(OrionDiagRequestPCIBit::FlowControl)},
};

enum class OrionDiagRequestSfModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    CurrentDataReply = 65,
    FreezeFrameReply = 66,
    StoredDTCsReply = 67,
    ClearDTCsReply = 68,
    Mode05Reply = 69,
    PendingDTCsReply = 71,
    VehicleInfoReply = 73,
    PermanentDTCsReply = 74,
    ResetReply = 81,
    Mode17Reply = 87,
    LockReply = 96,
    ReadPID8Reply = 97,
    ReadPID16Reply = 98,
    ReadPID24Reply = 99,
    SecurityAccessReply = 103,
    WriteReply = 123,
    ProfileReadReply = 124,
    NegativeOrAck = 127,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
    ModeA3Reply = 227,
    ModeB0Reply = 240,
    CalibrateReply = 241,
    LinkStatusReply = 242,
    NvramLogReply = 243,
    WatchdogReply = 245,
};

inline constexpr BitDefinition orion_diag_request_sf_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionDiagRequestSfModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionDiagRequestSfModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionDiagRequestSfModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionDiagRequestSfModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionDiagRequestSfModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionDiagRequestSfModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionDiagRequestSfModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ProfileRead)},
    {"CurrentData_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::CurrentDataReply)},
    {"FreezeFrame_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::FreezeFrameReply)},
    {"StoredDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::StoredDTCsReply)},
    {"ClearDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ClearDTCsReply)},
    {"Mode05_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Mode05Reply)},
    {"PendingDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::PendingDTCsReply)},
    {"VehicleInfo_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::VehicleInfoReply)},
    {"PermanentDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::PermanentDTCsReply)},
    {"Reset_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ResetReply)},
    {"Mode17_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Mode17Reply)},
    {"Lock_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::LockReply)},
    {"ReadPID8_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ReadPID8Reply)},
    {"ReadPID16_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ReadPID16Reply)},
    {"ReadPID24_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ReadPID24Reply)},
    {"SecurityAccess_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::SecurityAccessReply)},
    {"Write_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::WriteReply)},
    {"ProfileRead_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ProfileReadReply)},
    {"NegativeOrAck", static_cast<uint8_t>(OrionDiagRequestSfModeBit::NegativeOrAck)},
    {"ModeA3", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionDiagRequestSfModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionDiagRequestSfModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionDiagRequestSfModeBit::Watchdog)},
    {"ModeA3_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ModeA3Reply)},
    {"ModeB0_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::ModeB0Reply)},
    {"Calibrate_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::CalibrateReply)},
    {"LinkStatus_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::LinkStatusReply)},
    {"NvramLog_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::NvramLogReply)},
    {"Watchdog_Reply", static_cast<uint8_t>(OrionDiagRequestSfModeBit::WatchdogReply)},
};

enum class OrionDiagRequestSfRejectedModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
};

inline constexpr BitDefinition orion_diag_request_sf_rejected_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ProfileRead)},
    {"ModeA3", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionDiagRequestSfRejectedModeBit::Watchdog)},
};

enum class OrionDiagRequestSfWriteSubBit : uint8_t
{
    ProfileChunk = 0,
    HistoricalData = 2,
    Commit = 3,
    SetRTC = 4,
    SetPassword = 6,
    SetNickname = 8,
};

inline constexpr BitDefinition orion_diag_request_sf_write_sub_bits[] = {
    {"ProfileChunk", static_cast<uint8_t>(OrionDiagRequestSfWriteSubBit::ProfileChunk)},
    {"HistoricalData", static_cast<uint8_t>(OrionDiagRequestSfWriteSubBit::HistoricalData)},
    {"Commit", static_cast<uint8_t>(OrionDiagRequestSfWriteSubBit::Commit)},
    {"SetRTC", static_cast<uint8_t>(OrionDiagRequestSfWriteSubBit::SetRTC)},
    {"SetPassword", static_cast<uint8_t>(OrionDiagRequestSfWriteSubBit::SetPassword)},
    {"SetNickname", static_cast<uint8_t>(OrionDiagRequestSfWriteSubBit::SetNickname)},
};

enum class OrionDiagRequestSfSecurityLevelBit : uint8_t
{
    Tier1SeedRequest = 1,
    Tier1Key = 2,
    Tier2SeedRequest = 5,
    Tier2Key = 6,
};

inline constexpr BitDefinition orion_diag_request_sf_security_level_bits[] = {
    {"Tier1_SeedRequest", static_cast<uint8_t>(OrionDiagRequestSfSecurityLevelBit::Tier1SeedRequest)},
    {"Tier1_Key", static_cast<uint8_t>(OrionDiagRequestSfSecurityLevelBit::Tier1Key)},
    {"Tier2_SeedRequest", static_cast<uint8_t>(OrionDiagRequestSfSecurityLevelBit::Tier2SeedRequest)},
    {"Tier2_Key", static_cast<uint8_t>(OrionDiagRequestSfSecurityLevelBit::Tier2Key)},
};

enum class OrionDiagRequestSfLivePid8Bit : uint8_t
{
    RUNTIME = 31,
    DtcActiveTime = 77,
    DtcClearTime = 78,
};

inline constexpr BitDefinition orion_diag_request_sf_live_pid8_bits[] = {
    {"RUNTIME", static_cast<uint8_t>(OrionDiagRequestSfLivePid8Bit::RUNTIME)},
    {"DTC_ACTIVE_TIME", static_cast<uint8_t>(OrionDiagRequestSfLivePid8Bit::DtcActiveTime)},
    {"DTC_CLEAR_TIME", static_cast<uint8_t>(OrionDiagRequestSfLivePid8Bit::DtcClearTime)},
};

enum class OrionDiagRequestSfResetTypeBit : uint8_t
{
    Restart = 1,
    ClearCycleCount = 10,
    ClearLearnedSOC = 11,
    ClearContactorCount = 12,
};

inline constexpr BitDefinition orion_diag_request_sf_reset_type_bits[] = {
    {"Restart", static_cast<uint8_t>(OrionDiagRequestSfResetTypeBit::Restart)},
    {"ClearCycleCount", static_cast<uint8_t>(OrionDiagRequestSfResetTypeBit::ClearCycleCount)},
    {"ClearLearnedSOC", static_cast<uint8_t>(OrionDiagRequestSfResetTypeBit::ClearLearnedSOC)},
    {"ClearContactorCount", static_cast<uint8_t>(OrionDiagRequestSfResetTypeBit::ClearContactorCount)},
};

enum class OrionDiagRequestSfInfoTypeBit : uint8_t
{
    FirmwareVersion = 4,
    ProfileCRC = 6,
    Serial = 10,
    Identify = 11,
    LoadedChips = 12,
    Nickname = 18,
    HardwareRevision = 19,
};

inline constexpr BitDefinition orion_diag_request_sf_info_type_bits[] = {
    {"FirmwareVersion", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::FirmwareVersion)},
    {"ProfileCRC", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::ProfileCRC)},
    {"Serial", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::Serial)},
    {"Identify", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::Identify)},
    {"LoadedChips", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::LoadedChips)},
    {"Nickname", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::Nickname)},
    {"HardwareRevision", static_cast<uint8_t>(OrionDiagRequestSfInfoTypeBit::HardwareRevision)},
};

enum class OrionDiagRequestFfModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    CurrentDataReply = 65,
    FreezeFrameReply = 66,
    StoredDTCsReply = 67,
    ClearDTCsReply = 68,
    Mode05Reply = 69,
    PendingDTCsReply = 71,
    VehicleInfoReply = 73,
    PermanentDTCsReply = 74,
    ResetReply = 81,
    Mode17Reply = 87,
    LockReply = 96,
    ReadPID8Reply = 97,
    ReadPID16Reply = 98,
    ReadPID24Reply = 99,
    SecurityAccessReply = 103,
    WriteReply = 123,
    ProfileReadReply = 124,
    NegativeOrAck = 127,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
    ModeA3Reply = 227,
    ModeB0Reply = 240,
    CalibrateReply = 241,
    LinkStatusReply = 242,
    NvramLogReply = 243,
    WatchdogReply = 245,
};

inline constexpr BitDefinition orion_diag_request_ff_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionDiagRequestFfModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionDiagRequestFfModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionDiagRequestFfModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionDiagRequestFfModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionDiagRequestFfModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionDiagRequestFfModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionDiagRequestFfModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ProfileRead)},
    {"CurrentData_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::CurrentDataReply)},
    {"FreezeFrame_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::FreezeFrameReply)},
    {"StoredDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::StoredDTCsReply)},
    {"ClearDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ClearDTCsReply)},
    {"Mode05_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Mode05Reply)},
    {"PendingDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::PendingDTCsReply)},
    {"VehicleInfo_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::VehicleInfoReply)},
    {"PermanentDTCs_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::PermanentDTCsReply)},
    {"Reset_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ResetReply)},
    {"Mode17_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Mode17Reply)},
    {"Lock_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::LockReply)},
    {"ReadPID8_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ReadPID8Reply)},
    {"ReadPID16_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ReadPID16Reply)},
    {"ReadPID24_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ReadPID24Reply)},
    {"SecurityAccess_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::SecurityAccessReply)},
    {"Write_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::WriteReply)},
    {"ProfileRead_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ProfileReadReply)},
    {"NegativeOrAck", static_cast<uint8_t>(OrionDiagRequestFfModeBit::NegativeOrAck)},
    {"ModeA3", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionDiagRequestFfModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionDiagRequestFfModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionDiagRequestFfModeBit::Watchdog)},
    {"ModeA3_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ModeA3Reply)},
    {"ModeB0_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::ModeB0Reply)},
    {"Calibrate_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::CalibrateReply)},
    {"LinkStatus_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::LinkStatusReply)},
    {"NvramLog_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::NvramLogReply)},
    {"Watchdog_Reply", static_cast<uint8_t>(OrionDiagRequestFfModeBit::WatchdogReply)},
};

enum class OrionDiagRequestSfResponseCodeBit : uint8_t
{
    InvalidMode = 17,
    Busy = 33,
    ConditionsNotCorrect = 34,
    AccessDenied = 51,
    InvalidKey = 53,
    Invalid = 64,
    Accepted = 114,
    Resend = 117,
    BadChecksum = 119,
};

inline constexpr BitDefinition orion_diag_request_sf_response_code_bits[] = {
    {"InvalidMode", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::InvalidMode)},
    {"Busy", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::Busy)},
    {"ConditionsNotCorrect", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::ConditionsNotCorrect)},
    {"AccessDenied", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::AccessDenied)},
    {"InvalidKey", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::InvalidKey)},
    {"Invalid", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::Invalid)},
    {"Accepted", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::Accepted)},
    {"Resend", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::Resend)},
    {"BadChecksum", static_cast<uint8_t>(OrionDiagRequestSfResponseCodeBit::BadChecksum)},
};

inline constexpr SignalDefinition orion_diag_request_signals[] = {
    {"PCI", 0, 8, Endianness::Little, orion_diag_request_pci_bits, array_size(orion_diag_request_pci_bits)},
    {"FC_BlockSize", 8, 8, Endianness::Little, nullptr, 0},
    {"CF_Data", 15, 56, Endianness::Big, nullptr, 0},
    {"FF_Length", 8, 8, Endianness::Little, nullptr, 0},
    {"SF_Mode", 8, 8, Endianness::Little, orion_diag_request_sf_mode_bits, array_size(orion_diag_request_sf_mode_bits)},
    {"SF_RejectedMode", 16, 8, Endianness::Little, orion_diag_request_sf_rejected_mode_bits, array_size(orion_diag_request_sf_rejected_mode_bits)},
    {"SF_ReadSub", 16, 8, Endianness::Little, nullptr, 0},
    {"SF_WriteSub", 16, 8, Endianness::Little, orion_diag_request_sf_write_sub_bits, array_size(orion_diag_request_sf_write_sub_bits)},
    {"SF_SecurityLevel", 16, 8, Endianness::Little, orion_diag_request_sf_security_level_bits, array_size(orion_diag_request_sf_security_level_bits)},
    {"SF_Pid24", 23, 24, Endianness::Big, nullptr, 0},
    {"SF_LivePid8", 16, 8, Endianness::Little, orion_diag_request_sf_live_pid8_bits, array_size(orion_diag_request_sf_live_pid8_bits)},
    {"SF_ResetType", 16, 8, Endianness::Little, orion_diag_request_sf_reset_type_bits, array_size(orion_diag_request_sf_reset_type_bits)},
    {"SF_InfoType", 16, 8, Endianness::Little, orion_diag_request_sf_info_type_bits, array_size(orion_diag_request_sf_info_type_bits)},
    {"SF_Pid16", 23, 16, Endianness::Big, nullptr, 0},
    {"SF_B2", 16, 8, Endianness::Little, nullptr, 0},
    {"SF_Pid8", 16, 8, Endianness::Little, nullptr, 0},
    {"FC_SeparationTime", 16, 8, Endianness::Little, nullptr, 0},
    {"FF_Mode", 16, 8, Endianness::Little, orion_diag_request_ff_mode_bits, array_size(orion_diag_request_ff_mode_bits)},
    {"FF_RejectedMode", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ReadSub", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_WriteSub", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_SecurityLevel", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid24", 31, 24, Endianness::Big, nullptr, 0},
    {"FF_LivePid8", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ResetType", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_InfoType", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid16", 31, 16, Endianness::Big, nullptr, 0},
    {"FF_B3", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid8", 24, 8, Endianness::Little, nullptr, 0},
    {"SF_ResponseCode", 24, 8, Endianness::Little, orion_diag_request_sf_response_code_bits, array_size(orion_diag_request_sf_response_code_bits)},
    {"SF_Seed", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_ReadLength", 24, 8, Endianness::Little, nullptr, 0},
    {"SF_WriteChecksum", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_Key", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_B3", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ResponseCode", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Seed", 39, 32, Endianness::Big, nullptr, 0},
    {"FF_ReadLength", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_WriteLength", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Key", 39, 32, Endianness::Big, nullptr, 0},
    {"FF_B4", 32, 8, Endianness::Little, nullptr, 0},
    {"SF_Detail", 32, 8, Endianness::Little, nullptr, 0},
    {"SF_ReadAddress", 39, 16, Endianness::Big, nullptr, 0},
    {"SF_B4", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Detail", 40, 8, Endianness::Little, nullptr, 0},
    {"FF_ReadAddress", 47, 16, Endianness::Big, nullptr, 0},
    {"FF_WriteAddress", 47, 16, Endianness::Big, nullptr, 0},
    {"FF_B5", 40, 8, Endianness::Little, nullptr, 0},
    {"SF_B5", 40, 8, Endianness::Little, nullptr, 0},
    {"FF_B6", 48, 8, Endianness::Little, nullptr, 0},
    {"SF_ReadChecksum", 55, 16, Endianness::Big, nullptr, 0},
    {"SF_B6", 48, 8, Endianness::Little, nullptr, 0},
    {"FF_B7", 56, 8, Endianness::Little, nullptr, 0},
    {"SF_B7", 56, 8, Endianness::Little, nullptr, 0},
};

// ─── 0x7E5  Orion_ThirdPartyRequest ───────────────────────────────────────────────

inline constexpr SignalDefinition orion_third_party_request_signals[] = {
    {"Raw", 7, 64, Endianness::Big, nullptr, 0},
};

// ─── 0x7EB  Orion_DiagResponse ────────────────────────────────────────────────────

enum class OrionDiagResponsePCIBit : uint8_t
{
    SingleFrame1 = 1,
    SingleFrame2 = 2,
    SingleFrame3 = 3,
    SingleFrame4 = 4,
    SingleFrame5 = 5,
    SingleFrame6 = 6,
    SingleFrame7 = 7,
    FirstFrame = 16,
    Consecutive0 = 32,
    Consecutive1 = 33,
    Consecutive2 = 34,
    Consecutive3 = 35,
    Consecutive4 = 36,
    Consecutive5 = 37,
    Consecutive6 = 38,
    Consecutive7 = 39,
    Consecutive8 = 40,
    Consecutive9 = 41,
    Consecutive10 = 42,
    Consecutive11 = 43,
    Consecutive12 = 44,
    Consecutive13 = 45,
    Consecutive14 = 46,
    Consecutive15 = 47,
    FlowControl = 48,
};

inline constexpr BitDefinition orion_diag_response_pci_bits[] = {
    {"SingleFrame_1", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame1)},
    {"SingleFrame_2", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame2)},
    {"SingleFrame_3", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame3)},
    {"SingleFrame_4", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame4)},
    {"SingleFrame_5", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame5)},
    {"SingleFrame_6", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame6)},
    {"SingleFrame_7", static_cast<uint8_t>(OrionDiagResponsePCIBit::SingleFrame7)},
    {"FirstFrame", static_cast<uint8_t>(OrionDiagResponsePCIBit::FirstFrame)},
    {"Consecutive_0", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive0)},
    {"Consecutive_1", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive1)},
    {"Consecutive_2", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive2)},
    {"Consecutive_3", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive3)},
    {"Consecutive_4", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive4)},
    {"Consecutive_5", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive5)},
    {"Consecutive_6", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive6)},
    {"Consecutive_7", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive7)},
    {"Consecutive_8", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive8)},
    {"Consecutive_9", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive9)},
    {"Consecutive_10", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive10)},
    {"Consecutive_11", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive11)},
    {"Consecutive_12", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive12)},
    {"Consecutive_13", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive13)},
    {"Consecutive_14", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive14)},
    {"Consecutive_15", static_cast<uint8_t>(OrionDiagResponsePCIBit::Consecutive15)},
    {"FlowControl", static_cast<uint8_t>(OrionDiagResponsePCIBit::FlowControl)},
};

enum class OrionDiagResponseSfModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    CurrentDataReply = 65,
    FreezeFrameReply = 66,
    StoredDTCsReply = 67,
    ClearDTCsReply = 68,
    Mode05Reply = 69,
    PendingDTCsReply = 71,
    VehicleInfoReply = 73,
    PermanentDTCsReply = 74,
    ResetReply = 81,
    Mode17Reply = 87,
    LockReply = 96,
    ReadPID8Reply = 97,
    ReadPID16Reply = 98,
    ReadPID24Reply = 99,
    SecurityAccessReply = 103,
    WriteReply = 123,
    ProfileReadReply = 124,
    NegativeOrAck = 127,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
    ModeA3Reply = 227,
    ModeB0Reply = 240,
    CalibrateReply = 241,
    LinkStatusReply = 242,
    NvramLogReply = 243,
    WatchdogReply = 245,
};

inline constexpr BitDefinition orion_diag_response_sf_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionDiagResponseSfModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionDiagResponseSfModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionDiagResponseSfModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionDiagResponseSfModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionDiagResponseSfModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionDiagResponseSfModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionDiagResponseSfModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ProfileRead)},
    {"CurrentData_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::CurrentDataReply)},
    {"FreezeFrame_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::FreezeFrameReply)},
    {"StoredDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::StoredDTCsReply)},
    {"ClearDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ClearDTCsReply)},
    {"Mode05_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Mode05Reply)},
    {"PendingDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::PendingDTCsReply)},
    {"VehicleInfo_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::VehicleInfoReply)},
    {"PermanentDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::PermanentDTCsReply)},
    {"Reset_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ResetReply)},
    {"Mode17_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Mode17Reply)},
    {"Lock_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::LockReply)},
    {"ReadPID8_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ReadPID8Reply)},
    {"ReadPID16_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ReadPID16Reply)},
    {"ReadPID24_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ReadPID24Reply)},
    {"SecurityAccess_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::SecurityAccessReply)},
    {"Write_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::WriteReply)},
    {"ProfileRead_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ProfileReadReply)},
    {"NegativeOrAck", static_cast<uint8_t>(OrionDiagResponseSfModeBit::NegativeOrAck)},
    {"ModeA3", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionDiagResponseSfModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionDiagResponseSfModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionDiagResponseSfModeBit::Watchdog)},
    {"ModeA3_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ModeA3Reply)},
    {"ModeB0_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::ModeB0Reply)},
    {"Calibrate_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::CalibrateReply)},
    {"LinkStatus_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::LinkStatusReply)},
    {"NvramLog_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::NvramLogReply)},
    {"Watchdog_Reply", static_cast<uint8_t>(OrionDiagResponseSfModeBit::WatchdogReply)},
};

enum class OrionDiagResponseSfRejectedModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
};

inline constexpr BitDefinition orion_diag_response_sf_rejected_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ProfileRead)},
    {"ModeA3", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionDiagResponseSfRejectedModeBit::Watchdog)},
};

enum class OrionDiagResponseSfWriteSubBit : uint8_t
{
    ProfileChunk = 0,
    HistoricalData = 2,
    Commit = 3,
    SetRTC = 4,
    SetPassword = 6,
    SetNickname = 8,
};

inline constexpr BitDefinition orion_diag_response_sf_write_sub_bits[] = {
    {"ProfileChunk", static_cast<uint8_t>(OrionDiagResponseSfWriteSubBit::ProfileChunk)},
    {"HistoricalData", static_cast<uint8_t>(OrionDiagResponseSfWriteSubBit::HistoricalData)},
    {"Commit", static_cast<uint8_t>(OrionDiagResponseSfWriteSubBit::Commit)},
    {"SetRTC", static_cast<uint8_t>(OrionDiagResponseSfWriteSubBit::SetRTC)},
    {"SetPassword", static_cast<uint8_t>(OrionDiagResponseSfWriteSubBit::SetPassword)},
    {"SetNickname", static_cast<uint8_t>(OrionDiagResponseSfWriteSubBit::SetNickname)},
};

enum class OrionDiagResponseSfSecurityLevelBit : uint8_t
{
    Tier1SeedRequest = 1,
    Tier1Key = 2,
    Tier2SeedRequest = 5,
    Tier2Key = 6,
};

inline constexpr BitDefinition orion_diag_response_sf_security_level_bits[] = {
    {"Tier1_SeedRequest", static_cast<uint8_t>(OrionDiagResponseSfSecurityLevelBit::Tier1SeedRequest)},
    {"Tier1_Key", static_cast<uint8_t>(OrionDiagResponseSfSecurityLevelBit::Tier1Key)},
    {"Tier2_SeedRequest", static_cast<uint8_t>(OrionDiagResponseSfSecurityLevelBit::Tier2SeedRequest)},
    {"Tier2_Key", static_cast<uint8_t>(OrionDiagResponseSfSecurityLevelBit::Tier2Key)},
};

enum class OrionDiagResponseSfLivePid8Bit : uint8_t
{
    RUNTIME = 31,
    DtcActiveTime = 77,
    DtcClearTime = 78,
};

inline constexpr BitDefinition orion_diag_response_sf_live_pid8_bits[] = {
    {"RUNTIME", static_cast<uint8_t>(OrionDiagResponseSfLivePid8Bit::RUNTIME)},
    {"DTC_ACTIVE_TIME", static_cast<uint8_t>(OrionDiagResponseSfLivePid8Bit::DtcActiveTime)},
    {"DTC_CLEAR_TIME", static_cast<uint8_t>(OrionDiagResponseSfLivePid8Bit::DtcClearTime)},
};

enum class OrionDiagResponseSfResetTypeBit : uint8_t
{
    Restart = 1,
    ClearCycleCount = 10,
    ClearLearnedSOC = 11,
    ClearContactorCount = 12,
};

inline constexpr BitDefinition orion_diag_response_sf_reset_type_bits[] = {
    {"Restart", static_cast<uint8_t>(OrionDiagResponseSfResetTypeBit::Restart)},
    {"ClearCycleCount", static_cast<uint8_t>(OrionDiagResponseSfResetTypeBit::ClearCycleCount)},
    {"ClearLearnedSOC", static_cast<uint8_t>(OrionDiagResponseSfResetTypeBit::ClearLearnedSOC)},
    {"ClearContactorCount", static_cast<uint8_t>(OrionDiagResponseSfResetTypeBit::ClearContactorCount)},
};

enum class OrionDiagResponseSfInfoTypeBit : uint8_t
{
    FirmwareVersion = 4,
    ProfileCRC = 6,
    Serial = 10,
    Identify = 11,
    LoadedChips = 12,
    Nickname = 18,
    HardwareRevision = 19,
};

inline constexpr BitDefinition orion_diag_response_sf_info_type_bits[] = {
    {"FirmwareVersion", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::FirmwareVersion)},
    {"ProfileCRC", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::ProfileCRC)},
    {"Serial", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::Serial)},
    {"Identify", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::Identify)},
    {"LoadedChips", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::LoadedChips)},
    {"Nickname", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::Nickname)},
    {"HardwareRevision", static_cast<uint8_t>(OrionDiagResponseSfInfoTypeBit::HardwareRevision)},
};

enum class OrionDiagResponseFfModeBit : uint8_t
{
    CurrentData = 1,
    FreezeFrame = 2,
    StoredDTCs = 3,
    ClearDTCs = 4,
    Mode05 = 5,
    PendingDTCs = 7,
    VehicleInfo = 9,
    PermanentDTCs = 10,
    Reset = 17,
    Mode17 = 23,
    Lock = 32,
    ReadPID8 = 33,
    ReadPID16 = 34,
    ReadPID24 = 35,
    SecurityAccess = 39,
    Write = 59,
    ProfileRead = 60,
    CurrentDataReply = 65,
    FreezeFrameReply = 66,
    StoredDTCsReply = 67,
    ClearDTCsReply = 68,
    Mode05Reply = 69,
    PendingDTCsReply = 71,
    VehicleInfoReply = 73,
    PermanentDTCsReply = 74,
    ResetReply = 81,
    Mode17Reply = 87,
    LockReply = 96,
    ReadPID8Reply = 97,
    ReadPID16Reply = 98,
    ReadPID24Reply = 99,
    SecurityAccessReply = 103,
    WriteReply = 123,
    ProfileReadReply = 124,
    NegativeOrAck = 127,
    ModeA3 = 163,
    ModeB0 = 176,
    Calibrate = 177,
    LinkStatus = 178,
    NvramLog = 179,
    Watchdog = 181,
    ModeA3Reply = 227,
    ModeB0Reply = 240,
    CalibrateReply = 241,
    LinkStatusReply = 242,
    NvramLogReply = 243,
    WatchdogReply = 245,
};

inline constexpr BitDefinition orion_diag_response_ff_mode_bits[] = {
    {"CurrentData", static_cast<uint8_t>(OrionDiagResponseFfModeBit::CurrentData)},
    {"FreezeFrame", static_cast<uint8_t>(OrionDiagResponseFfModeBit::FreezeFrame)},
    {"StoredDTCs", static_cast<uint8_t>(OrionDiagResponseFfModeBit::StoredDTCs)},
    {"ClearDTCs", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ClearDTCs)},
    {"Mode05", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Mode05)},
    {"PendingDTCs", static_cast<uint8_t>(OrionDiagResponseFfModeBit::PendingDTCs)},
    {"VehicleInfo", static_cast<uint8_t>(OrionDiagResponseFfModeBit::VehicleInfo)},
    {"PermanentDTCs", static_cast<uint8_t>(OrionDiagResponseFfModeBit::PermanentDTCs)},
    {"Reset", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Reset)},
    {"Mode17", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Mode17)},
    {"Lock", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Lock)},
    {"ReadPID8", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ReadPID8)},
    {"ReadPID16", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ReadPID16)},
    {"ReadPID24", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ReadPID24)},
    {"SecurityAccess", static_cast<uint8_t>(OrionDiagResponseFfModeBit::SecurityAccess)},
    {"Write", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Write)},
    {"ProfileRead", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ProfileRead)},
    {"CurrentData_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::CurrentDataReply)},
    {"FreezeFrame_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::FreezeFrameReply)},
    {"StoredDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::StoredDTCsReply)},
    {"ClearDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ClearDTCsReply)},
    {"Mode05_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Mode05Reply)},
    {"PendingDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::PendingDTCsReply)},
    {"VehicleInfo_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::VehicleInfoReply)},
    {"PermanentDTCs_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::PermanentDTCsReply)},
    {"Reset_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ResetReply)},
    {"Mode17_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Mode17Reply)},
    {"Lock_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::LockReply)},
    {"ReadPID8_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ReadPID8Reply)},
    {"ReadPID16_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ReadPID16Reply)},
    {"ReadPID24_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ReadPID24Reply)},
    {"SecurityAccess_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::SecurityAccessReply)},
    {"Write_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::WriteReply)},
    {"ProfileRead_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ProfileReadReply)},
    {"NegativeOrAck", static_cast<uint8_t>(OrionDiagResponseFfModeBit::NegativeOrAck)},
    {"ModeA3", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ModeA3)},
    {"ModeB0", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ModeB0)},
    {"Calibrate", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Calibrate)},
    {"LinkStatus", static_cast<uint8_t>(OrionDiagResponseFfModeBit::LinkStatus)},
    {"NvramLog", static_cast<uint8_t>(OrionDiagResponseFfModeBit::NvramLog)},
    {"Watchdog", static_cast<uint8_t>(OrionDiagResponseFfModeBit::Watchdog)},
    {"ModeA3_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ModeA3Reply)},
    {"ModeB0_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::ModeB0Reply)},
    {"Calibrate_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::CalibrateReply)},
    {"LinkStatus_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::LinkStatusReply)},
    {"NvramLog_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::NvramLogReply)},
    {"Watchdog_Reply", static_cast<uint8_t>(OrionDiagResponseFfModeBit::WatchdogReply)},
};

enum class OrionDiagResponseSfResponseCodeBit : uint8_t
{
    InvalidMode = 17,
    Busy = 33,
    ConditionsNotCorrect = 34,
    AccessDenied = 51,
    InvalidKey = 53,
    Invalid = 64,
    Accepted = 114,
    Resend = 117,
    BadChecksum = 119,
};

inline constexpr BitDefinition orion_diag_response_sf_response_code_bits[] = {
    {"InvalidMode", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::InvalidMode)},
    {"Busy", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::Busy)},
    {"ConditionsNotCorrect", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::ConditionsNotCorrect)},
    {"AccessDenied", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::AccessDenied)},
    {"InvalidKey", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::InvalidKey)},
    {"Invalid", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::Invalid)},
    {"Accepted", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::Accepted)},
    {"Resend", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::Resend)},
    {"BadChecksum", static_cast<uint8_t>(OrionDiagResponseSfResponseCodeBit::BadChecksum)},
};

inline constexpr SignalDefinition orion_diag_response_signals[] = {
    {"PCI", 0, 8, Endianness::Little, orion_diag_response_pci_bits, array_size(orion_diag_response_pci_bits)},
    {"FC_BlockSize", 8, 8, Endianness::Little, nullptr, 0},
    {"CF_Data", 15, 56, Endianness::Big, nullptr, 0},
    {"FF_Length", 8, 8, Endianness::Little, nullptr, 0},
    {"SF_Mode", 8, 8, Endianness::Little, orion_diag_response_sf_mode_bits, array_size(orion_diag_response_sf_mode_bits)},
    {"SF_RejectedMode", 16, 8, Endianness::Little, orion_diag_response_sf_rejected_mode_bits, array_size(orion_diag_response_sf_rejected_mode_bits)},
    {"SF_ReadSub", 16, 8, Endianness::Little, nullptr, 0},
    {"SF_WriteSub", 16, 8, Endianness::Little, orion_diag_response_sf_write_sub_bits, array_size(orion_diag_response_sf_write_sub_bits)},
    {"SF_SecurityLevel", 16, 8, Endianness::Little, orion_diag_response_sf_security_level_bits, array_size(orion_diag_response_sf_security_level_bits)},
    {"SF_Pid24", 23, 24, Endianness::Big, nullptr, 0},
    {"SF_LivePid8", 16, 8, Endianness::Little, orion_diag_response_sf_live_pid8_bits, array_size(orion_diag_response_sf_live_pid8_bits)},
    {"SF_ResetType", 16, 8, Endianness::Little, orion_diag_response_sf_reset_type_bits, array_size(orion_diag_response_sf_reset_type_bits)},
    {"SF_InfoType", 16, 8, Endianness::Little, orion_diag_response_sf_info_type_bits, array_size(orion_diag_response_sf_info_type_bits)},
    {"SF_Pid16", 23, 16, Endianness::Big, nullptr, 0},
    {"SF_B2", 16, 8, Endianness::Little, nullptr, 0},
    {"SF_Pid8", 16, 8, Endianness::Little, nullptr, 0},
    {"FC_SeparationTime", 16, 8, Endianness::Little, nullptr, 0},
    {"FF_Mode", 16, 8, Endianness::Little, orion_diag_response_ff_mode_bits, array_size(orion_diag_response_ff_mode_bits)},
    {"FF_RejectedMode", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ReadSub", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_WriteSub", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_SecurityLevel", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid24", 31, 24, Endianness::Big, nullptr, 0},
    {"FF_LivePid8", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ResetType", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_InfoType", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid16", 31, 16, Endianness::Big, nullptr, 0},
    {"FF_B3", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_Pid8", 24, 8, Endianness::Little, nullptr, 0},
    {"SF_ResponseCode", 24, 8, Endianness::Little, orion_diag_response_sf_response_code_bits, array_size(orion_diag_response_sf_response_code_bits)},
    {"SF_Seed", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_ReadLength", 24, 8, Endianness::Little, nullptr, 0},
    {"SF_WriteChecksum", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_Key", 31, 16, Endianness::Big, nullptr, 0},
    {"SF_B3", 24, 8, Endianness::Little, nullptr, 0},
    {"FF_ResponseCode", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Seed", 39, 32, Endianness::Big, nullptr, 0},
    {"FF_ReadLength", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_WriteLength", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Key", 39, 32, Endianness::Big, nullptr, 0},
    {"FF_B4", 32, 8, Endianness::Little, nullptr, 0},
    {"SF_Detail", 32, 8, Endianness::Little, nullptr, 0},
    {"SF_ReadAddress", 39, 16, Endianness::Big, nullptr, 0},
    {"SF_B4", 32, 8, Endianness::Little, nullptr, 0},
    {"FF_Detail", 40, 8, Endianness::Little, nullptr, 0},
    {"FF_ReadAddress", 47, 16, Endianness::Big, nullptr, 0},
    {"FF_WriteAddress", 47, 16, Endianness::Big, nullptr, 0},
    {"FF_B5", 40, 8, Endianness::Little, nullptr, 0},
    {"SF_B5", 40, 8, Endianness::Little, nullptr, 0},
    {"FF_B6", 48, 8, Endianness::Little, nullptr, 0},
    {"SF_ReadChecksum", 55, 16, Endianness::Big, nullptr, 0},
    {"SF_B6", 48, 8, Endianness::Little, nullptr, 0},
    {"FF_B7", 56, 8, Endianness::Little, nullptr, 0},
    {"SF_B7", 56, 8, Endianness::Little, nullptr, 0},
};

// ─── 0x7ED  Orion_ThirdPartyResponse ──────────────────────────────────────────────

inline constexpr SignalDefinition orion_third_party_response_signals[] = {
    {"Raw", 7, 64, Endianness::Big, nullptr, 0},
};

// ─── 0x1806E5F4  Charger1Communication ────────────────────────────────────────────

inline constexpr SignalDefinition charger1_communication_signals[] = {
    {"Charger_Control", 24, 8, Endianness::Big, nullptr, 0},
    {"Pack_CCL", 32, 16, Endianness::Big, nullptr, 0},
    {"Maximum_Pack_Voltage", 48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x1806E7F4  Charger2Communication ────────────────────────────────────────────

inline constexpr SignalDefinition charger2_communication_signals[] = {
    {"Charger_Control", 24, 8, Endianness::Big, nullptr, 0},
    {"Pack_CCL", 32, 16, Endianness::Big, nullptr, 0},
    {"Maximum_Pack_Voltage", 48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x1806E9F4  Charger3Communication ────────────────────────────────────────────

inline constexpr SignalDefinition charger3_communication_signals[] = {
    {"Charger_Control", 24, 8, Endianness::Big, nullptr, 0},
    {"Pack_CCL", 32, 16, Endianness::Big, nullptr, 0},
    {"Maximum_Pack_Voltage", 48, 16, Endianness::Big, nullptr, 0},
};

// ─── Message and Signal ID Enums ─────────────────────────────────────────────

enum class MessageID : uint8_t
{
    BmsSafetyCritical = 0,
    BmsPackVoltageAndEnergy = 1,
    BmsTemperatureCurrentPower = 2,
    PowerDistroDisplay = 3,
    MotorControllerId = 4,
    MotorControllerErrors = 5,
    MotorBusVoltageCurrent = 6,
    MotorSpeed = 7,
    PhaseCurrentMeasurement = 8,
    MotorVoltageVectorMeasurement = 9,
    MotorCurrentVectorMeasurement = 10,
    BackEMFMeasurementPrediction = 11,
    VoltageRail15VMeasurement = 12,
    VoltageRail3V31V9Measurement = 13,
    Reserved0A = 14,
    MotorTemperature = 15,
    DspBoardTempMeasurement = 16,
    Reserved0D = 17,
    OdometerBusAhMeasurement = 18,
    Ws22ModeCommand = 19,
    Ws22AcquisitionStatus = 20,
    Ws22AcquisitionSamples = 21,
    ConfigTransferControl = 22,
    ConfigTransferData = 23,
    SlipSpeedMeasurement = 24,
    MotorCurrentVelocityControl = 25,
    BusCurrentControl = 26,
    ResetCommand = 27,
    BmsCellData = 28,
    BmsThermistorBroadcast = 29,
    BmsCellDataBroadcast = 30,
    PeripheralControl = 31,
    OrionFunctionalRequest = 32,
    OrionDiagRequest = 33,
    OrionThirdPartyRequest = 34,
    OrionDiagResponse = 35,
    OrionThirdPartyResponse = 36,
    Charger1Communication = 37,
    Charger2Communication = 38,
    Charger3Communication = 39,
};

enum class BmsSafetyCriticalSignal : uint8_t
{
    DtcFlags1 = 0,
    DtcFlags21 = 1,
    DtcFlags22 = 2,
    RelayState1 = 3,
    RelayState2 = 4,
    FailsafeStatuses = 5,
    PackSoc = 6,
    AdaptivePackSoc = 7,
};

enum class BmsPackVoltageAndEnergySignal : uint8_t
{
    PackKWPower = 0,
    PackAmphours = 1,
    PackCurrent = 2,
    PackVoltage = 3,
};

enum class BmsTemperatureCurrentPowerSignal : uint8_t
{
    PackDcl = 0,
    PackCcl = 1,
    LowTemperature = 2,
    HighTemperature = 3,
    CurrentLimitsStatus = 4,
    AdaptiveAmphours = 5,
};

enum class PowerDistroDisplaySignal : uint8_t
{
    DisplayFlagsPowerMonitor = 0,
    DisplayFlagsMainBattery = 1,
    DisplayFlagsAuxBattery = 2,
};

enum class MotorControllerIdSignal : uint8_t
{
    ProhelionId = 0,
    SerialNumber = 1,
};

enum class MotorControllerErrorsSignal : uint8_t
{
    McLimitFlags = 0,
    Reserved = 1,
    McErrorFlags1 = 2,
    McErrorFlags2 = 3,
    ActiveMotorIndex = 4,
    CanTxErrorCount = 5,
    CanRxErrorCount = 6,
};

enum class MotorBusVoltageCurrentSignal : uint8_t
{
    BusVoltage = 0,
    BusCurrent = 1,
};

enum class MotorSpeedSignal : uint8_t
{
    MotorVelocity = 0,
    VehicleVelocity = 1,
};

enum class PhaseCurrentMeasurementSignal : uint8_t
{
    PhaseCurrentB = 0,
    PhaseCurrentC = 1,
};

enum class MotorVoltageVectorMeasurementSignal : uint8_t
{
    Vq = 0,
    Vd = 1,
};

enum class MotorCurrentVectorMeasurementSignal : uint8_t
{
    Iq = 0,
    Id = 1,
};

enum class BackEMFMeasurementPredictionSignal : uint8_t
{
    BEMFq = 0,
    BEMFd = 1,
};

enum class VoltageRail15VMeasurementSignal : uint8_t
{
    ReservedSupply15V = 0,
    Supply15V = 1,
};

enum class VoltageRail3V31V9MeasurementSignal : uint8_t
{
    Supply1V9 = 0,
    Supply3V3 = 1,
};

enum class Reserved0ASignal : uint8_t
{
    Reserved0A0 = 0,
    Reserved0A1 = 1,
};

enum class MotorTemperatureSignal : uint8_t
{
    MotorTemp = 0,
    HeatSinkTemp = 1,
};

enum class DspBoardTempMeasurementSignal : uint8_t
{
    DspBoardTemp = 0,
    ReservedDspBoardTemp = 1,
};

enum class Reserved0DSignal : uint8_t
{
    Reserved0D0 = 0,
    Reserved0D1 = 1,
};

enum class OdometerBusAhMeasurementSignal : uint8_t
{
    Odometer = 0,
    DCBusAh = 1,
};

enum class Ws22ModeCommandSignal : uint8_t
{
    Reserved = 0,
    Mode = 1,
    Start = 2,
};

enum class Ws22AcquisitionStatusSignal : uint8_t
{
    Word0 = 0,
    Word1 = 1,
    Word2 = 2,
    Word3 = 3,
};

enum class Ws22AcquisitionSamplesSignal : uint8_t
{
    Sample0 = 0,
    Sample1 = 1,
    Sample2 = 2,
    Sample3 = 3,
};

enum class ConfigTransferControlSignal : uint8_t
{
    Op = 0,
    ActmotTag = 1,
    SizeBytes = 2,
    RequestAddress = 3,
    BadSeqFramesReceived = 4,
    BlockAckStatus = 5,
    ParamAckBlockSize = 6,
    CloseKind = 7,
    InitBlockSize = 8,
    RequestSize = 9,
    BadSeqFramesPerBlock = 10,
    ParamAckTimeout = 11,
    InitTimeout = 12,
    BlockAckToggle = 13,
    ActmotMotorIndex = 14,
    RequestStream = 15,
    ParamAckRetries = 16,
    InitRetries = 17,
    RequestResponderSends = 18,
};

enum class ConfigTransferDataSignal : uint8_t
{
    Sequence = 0,
    Toggle = 1,
    Payload1 = 2,
    Payload2 = 3,
    Payload3 = 4,
    Payload4 = 5,
    Payload5 = 6,
    Payload6 = 7,
    Payload7 = 8,
};

enum class SlipSpeedMeasurementSignal : uint8_t
{
    SlipSpeed = 0,
    ReservedSlipSpeed = 1,
};

enum class MotorCurrentVelocityControlSignal : uint8_t
{
    MotorVelocitySetpoint = 0,
    MotorCurrentSetpoint = 1,
};

enum class BusCurrentControlSignal : uint8_t
{
    Reserved = 0,
    BusCurrentSetpoint = 1,
};

enum class ResetCommandSignal : uint8_t
{
    Unused = 0,
};

enum class BmsCellDataSignal : uint8_t
{
    LowCellVoltage = 0,
    HighCellVoltage = 1,
    LowCellVoltageId = 2,
    HighCellVoltageId = 3,
    LowThermistorId = 4,
    HighThermistorId = 5,
};

enum class BmsThermistorBroadcastSignal : uint8_t
{
    ThermistorData = 0,
};

enum class BmsCellDataBroadcastSignal : uint8_t
{
    CellId = 0,
    InstantVoltage = 1,
    InternalResistance = 2,
    OpenVoltage = 3,
    Checksum = 4,
};

enum class PeripheralControlSignal : uint8_t
{
    DistroControl = 0,
};

enum class OrionFunctionalRequestSignal : uint8_t
{
    PCI = 0,
    FcBlockSize = 1,
    CfData = 2,
    FfLength = 3,
    SfMode = 4,
    SfRejectedMode = 5,
    SfReadSub = 6,
    SfWriteSub = 7,
    SfSecurityLevel = 8,
    SfPid24 = 9,
    SfLivePid8 = 10,
    SfResetType = 11,
    SfInfoType = 12,
    SfPid16 = 13,
    SfB2 = 14,
    SfPid8 = 15,
    FcSeparationTime = 16,
    FfMode = 17,
    FfRejectedMode = 18,
    FfReadSub = 19,
    FfWriteSub = 20,
    FfSecurityLevel = 21,
    FfPid24 = 22,
    FfLivePid8 = 23,
    FfResetType = 24,
    FfInfoType = 25,
    FfPid16 = 26,
    FfB3 = 27,
    FfPid8 = 28,
    SfResponseCode = 29,
    SfSeed = 30,
    SfReadLength = 31,
    SfWriteChecksum = 32,
    SfKey = 33,
    SfB3 = 34,
    FfResponseCode = 35,
    FfSeed = 36,
    FfReadLength = 37,
    FfWriteLength = 38,
    FfKey = 39,
    FfB4 = 40,
    SfDetail = 41,
    SfReadAddress = 42,
    SfB4 = 43,
    FfDetail = 44,
    FfReadAddress = 45,
    FfWriteAddress = 46,
    FfB5 = 47,
    SfB5 = 48,
    FfB6 = 49,
    SfReadChecksum = 50,
    SfB6 = 51,
    FfB7 = 52,
    SfB7 = 53,
};

enum class OrionDiagRequestSignal : uint8_t
{
    PCI = 0,
    FcBlockSize = 1,
    CfData = 2,
    FfLength = 3,
    SfMode = 4,
    SfRejectedMode = 5,
    SfReadSub = 6,
    SfWriteSub = 7,
    SfSecurityLevel = 8,
    SfPid24 = 9,
    SfLivePid8 = 10,
    SfResetType = 11,
    SfInfoType = 12,
    SfPid16 = 13,
    SfB2 = 14,
    SfPid8 = 15,
    FcSeparationTime = 16,
    FfMode = 17,
    FfRejectedMode = 18,
    FfReadSub = 19,
    FfWriteSub = 20,
    FfSecurityLevel = 21,
    FfPid24 = 22,
    FfLivePid8 = 23,
    FfResetType = 24,
    FfInfoType = 25,
    FfPid16 = 26,
    FfB3 = 27,
    FfPid8 = 28,
    SfResponseCode = 29,
    SfSeed = 30,
    SfReadLength = 31,
    SfWriteChecksum = 32,
    SfKey = 33,
    SfB3 = 34,
    FfResponseCode = 35,
    FfSeed = 36,
    FfReadLength = 37,
    FfWriteLength = 38,
    FfKey = 39,
    FfB4 = 40,
    SfDetail = 41,
    SfReadAddress = 42,
    SfB4 = 43,
    FfDetail = 44,
    FfReadAddress = 45,
    FfWriteAddress = 46,
    FfB5 = 47,
    SfB5 = 48,
    FfB6 = 49,
    SfReadChecksum = 50,
    SfB6 = 51,
    FfB7 = 52,
    SfB7 = 53,
};

enum class OrionThirdPartyRequestSignal : uint8_t
{
    Raw = 0,
};

enum class OrionDiagResponseSignal : uint8_t
{
    PCI = 0,
    FcBlockSize = 1,
    CfData = 2,
    FfLength = 3,
    SfMode = 4,
    SfRejectedMode = 5,
    SfReadSub = 6,
    SfWriteSub = 7,
    SfSecurityLevel = 8,
    SfPid24 = 9,
    SfLivePid8 = 10,
    SfResetType = 11,
    SfInfoType = 12,
    SfPid16 = 13,
    SfB2 = 14,
    SfPid8 = 15,
    FcSeparationTime = 16,
    FfMode = 17,
    FfRejectedMode = 18,
    FfReadSub = 19,
    FfWriteSub = 20,
    FfSecurityLevel = 21,
    FfPid24 = 22,
    FfLivePid8 = 23,
    FfResetType = 24,
    FfInfoType = 25,
    FfPid16 = 26,
    FfB3 = 27,
    FfPid8 = 28,
    SfResponseCode = 29,
    SfSeed = 30,
    SfReadLength = 31,
    SfWriteChecksum = 32,
    SfKey = 33,
    SfB3 = 34,
    FfResponseCode = 35,
    FfSeed = 36,
    FfReadLength = 37,
    FfWriteLength = 38,
    FfKey = 39,
    FfB4 = 40,
    SfDetail = 41,
    SfReadAddress = 42,
    SfB4 = 43,
    FfDetail = 44,
    FfReadAddress = 45,
    FfWriteAddress = 46,
    FfB5 = 47,
    SfB5 = 48,
    FfB6 = 49,
    SfReadChecksum = 50,
    SfB6 = 51,
    FfB7 = 52,
    SfB7 = 53,
};

enum class OrionThirdPartyResponseSignal : uint8_t
{
    Raw = 0,
};

enum class Charger1CommunicationSignal : uint8_t
{
    ChargerControl = 0,
    PackCcl = 1,
    MaximumPackVoltage = 2,
};

enum class Charger2CommunicationSignal : uint8_t
{
    ChargerControl = 0,
    PackCcl = 1,
    MaximumPackVoltage = 2,
};

enum class Charger3CommunicationSignal : uint8_t
{
    ChargerControl = 0,
    PackCcl = 1,
    MaximumPackVoltage = 2,
};

// ─── Master Message Table ─────────────────────────────────────────────────────
// Keep this table's row order in sync with the MessageID enum above.

inline constexpr MessageDefinition artemis_messages[] = {
    {"BmsSafetyCritical", 0x200, 40, bms_safety_critical_signals, array_size(bms_safety_critical_signals)},
    {"BmsPackVoltageAndEnergy", 0x201, 80, bms_pack_voltage_and_energy_signals, array_size(bms_pack_voltage_and_energy_signals)},
    {"BmsTemperatureCurrentPower", 0x202, 80, bms_temperature_current_power_signals, array_size(bms_temperature_current_power_signals)},
    {"PowerDistroDisplay", 0x300, 100, power_distro_display_signals, array_size(power_distro_display_signals)},
    {"MotorControllerId", 0x400, 1000, motor_controller_id_signals, array_size(motor_controller_id_signals)},
    {"MotorControllerErrors", 0x401, 200, motor_controller_errors_signals, array_size(motor_controller_errors_signals)},
    {"MotorBusVoltageCurrent", 0x402, 200, motor_bus_voltage_current_signals, array_size(motor_bus_voltage_current_signals)},
    {"MotorSpeed", 0x403, 200, motor_speed_signals, array_size(motor_speed_signals)},
    {"PhaseCurrentMeasurement", 0x404, 200, phase_current_measurement_signals, array_size(phase_current_measurement_signals)},
    {"MotorVoltageVectorMeasurement", 0x405, 200, motor_voltage_vector_measurement_signals, array_size(motor_voltage_vector_measurement_signals)},
    {"MotorCurrentVectorMeasurement", 0x406, 200, motor_current_vector_measurement_signals, array_size(motor_current_vector_measurement_signals)},
    {"BackEMFMeasurementPrediction", 0x407, 200, back_emfmeasurement_prediction_signals, array_size(back_emfmeasurement_prediction_signals)},
    {"VoltageRail15VMeasurement", 0x408, 1000, voltage_rail15_vmeasurement_signals, array_size(voltage_rail15_vmeasurement_signals)},
    {"VoltageRail3V31V9Measurement", 0x409, 1000, voltage_rail3_v31_v9_measurement_signals, array_size(voltage_rail3_v31_v9_measurement_signals)},
    {"Reserved0A", 0x40A, 0, reserved0_a_signals, array_size(reserved0_a_signals)},
    {"MotorTemperature", 0x40B, 1000, motor_temperature_signals, array_size(motor_temperature_signals)},
    {"DspBoardTempMeasurement", 0x40C, 1000, dsp_board_temp_measurement_signals, array_size(dsp_board_temp_measurement_signals)},
    {"Reserved0D", 0x40D, 1000, reserved0_d_signals, array_size(reserved0_d_signals)},
    {"OdometerBusAhMeasurement", 0x40E, 1000, odometer_bus_ah_measurement_signals, array_size(odometer_bus_ah_measurement_signals)},
    {"WS22_ModeCommand", 0x40F, 0, ws22_mode_command_signals, array_size(ws22_mode_command_signals)},
    {"WS22_AcquisitionStatus", 0x410, 0, ws22_acquisition_status_signals, array_size(ws22_acquisition_status_signals)},
    {"WS22_AcquisitionSamples", 0x411, 0, ws22_acquisition_samples_signals, array_size(ws22_acquisition_samples_signals)},
    {"ConfigTransferControl", 0x412, 0, config_transfer_control_signals, array_size(config_transfer_control_signals)},
    {"ConfigTransferData", 0x414, 0, config_transfer_data_signals, array_size(config_transfer_data_signals)},
    {"SlipSpeedMeasurement", 0x417, 200, slip_speed_measurement_signals, array_size(slip_speed_measurement_signals)},
    {"MotorCurrentVelocityControl", 0x501, 100, motor_current_velocity_control_signals, array_size(motor_current_velocity_control_signals)},
    {"BusCurrentControl", 0x502, 100, bus_current_control_signals, array_size(bus_current_control_signals)},
    {"ResetCommand", 0x503, 0, reset_command_signals, array_size(reset_command_signals)},
    {"BmsCellData", 0x601, 80, bms_cell_data_signals, array_size(bms_cell_data_signals)},
    {"BmsThermistorBroadcast", 0x602, 0, bms_thermistor_broadcast_signals, array_size(bms_thermistor_broadcast_signals)},
    {"BmsCellDataBroadcast", 0x603, 20, bms_cell_data_broadcast_signals, array_size(bms_cell_data_broadcast_signals)},
    {"PeripheralControl", 0x700, 100, peripheral_control_signals, array_size(peripheral_control_signals)},
    {"Orion_FunctionalRequest", 0x7DF, 0, orion_functional_request_signals, array_size(orion_functional_request_signals)},
    {"Orion_DiagRequest", 0x7E3, 0, orion_diag_request_signals, array_size(orion_diag_request_signals)},
    {"Orion_ThirdPartyRequest", 0x7E5, 0, orion_third_party_request_signals, array_size(orion_third_party_request_signals)},
    {"Orion_DiagResponse", 0x7EB, 0, orion_diag_response_signals, array_size(orion_diag_response_signals)},
    {"Orion_ThirdPartyResponse", 0x7ED, 0, orion_third_party_response_signals, array_size(orion_third_party_response_signals)},
    {"Charger1Communication", 0x1806E5F4, 808, charger1_communication_signals, array_size(charger1_communication_signals)},
    {"Charger2Communication", 0x1806E7F4, 808, charger2_communication_signals, array_size(charger2_communication_signals)},
    {"Charger3Communication", 0x1806E9F4, 808, charger3_communication_signals, array_size(charger3_communication_signals)},
};

// ─── Message-Signal Associations ─────────────────────────────────────────────
// Maps each signal enum to its owning message, enforcing correct pairing in getSignal().

template<typename SignalEnum> struct MessageOf; // intentionally undefined

template<> struct MessageOf<BmsSafetyCriticalSignal> { static constexpr MessageID value = MessageID::BmsSafetyCritical; };
template<> struct MessageOf<BmsPackVoltageAndEnergySignal> { static constexpr MessageID value = MessageID::BmsPackVoltageAndEnergy; };
template<> struct MessageOf<BmsTemperatureCurrentPowerSignal> { static constexpr MessageID value = MessageID::BmsTemperatureCurrentPower; };
template<> struct MessageOf<PowerDistroDisplaySignal> { static constexpr MessageID value = MessageID::PowerDistroDisplay; };
template<> struct MessageOf<MotorControllerIdSignal> { static constexpr MessageID value = MessageID::MotorControllerId; };
template<> struct MessageOf<MotorControllerErrorsSignal> { static constexpr MessageID value = MessageID::MotorControllerErrors; };
template<> struct MessageOf<MotorBusVoltageCurrentSignal> { static constexpr MessageID value = MessageID::MotorBusVoltageCurrent; };
template<> struct MessageOf<MotorSpeedSignal> { static constexpr MessageID value = MessageID::MotorSpeed; };
template<> struct MessageOf<PhaseCurrentMeasurementSignal> { static constexpr MessageID value = MessageID::PhaseCurrentMeasurement; };
template<> struct MessageOf<MotorVoltageVectorMeasurementSignal> { static constexpr MessageID value = MessageID::MotorVoltageVectorMeasurement; };
template<> struct MessageOf<MotorCurrentVectorMeasurementSignal> { static constexpr MessageID value = MessageID::MotorCurrentVectorMeasurement; };
template<> struct MessageOf<BackEMFMeasurementPredictionSignal> { static constexpr MessageID value = MessageID::BackEMFMeasurementPrediction; };
template<> struct MessageOf<VoltageRail15VMeasurementSignal> { static constexpr MessageID value = MessageID::VoltageRail15VMeasurement; };
template<> struct MessageOf<VoltageRail3V31V9MeasurementSignal> { static constexpr MessageID value = MessageID::VoltageRail3V31V9Measurement; };
template<> struct MessageOf<Reserved0ASignal> { static constexpr MessageID value = MessageID::Reserved0A; };
template<> struct MessageOf<MotorTemperatureSignal> { static constexpr MessageID value = MessageID::MotorTemperature; };
template<> struct MessageOf<DspBoardTempMeasurementSignal> { static constexpr MessageID value = MessageID::DspBoardTempMeasurement; };
template<> struct MessageOf<Reserved0DSignal> { static constexpr MessageID value = MessageID::Reserved0D; };
template<> struct MessageOf<OdometerBusAhMeasurementSignal> { static constexpr MessageID value = MessageID::OdometerBusAhMeasurement; };
template<> struct MessageOf<Ws22ModeCommandSignal> { static constexpr MessageID value = MessageID::Ws22ModeCommand; };
template<> struct MessageOf<Ws22AcquisitionStatusSignal> { static constexpr MessageID value = MessageID::Ws22AcquisitionStatus; };
template<> struct MessageOf<Ws22AcquisitionSamplesSignal> { static constexpr MessageID value = MessageID::Ws22AcquisitionSamples; };
template<> struct MessageOf<ConfigTransferControlSignal> { static constexpr MessageID value = MessageID::ConfigTransferControl; };
template<> struct MessageOf<ConfigTransferDataSignal> { static constexpr MessageID value = MessageID::ConfigTransferData; };
template<> struct MessageOf<SlipSpeedMeasurementSignal> { static constexpr MessageID value = MessageID::SlipSpeedMeasurement; };
template<> struct MessageOf<MotorCurrentVelocityControlSignal> { static constexpr MessageID value = MessageID::MotorCurrentVelocityControl; };
template<> struct MessageOf<BusCurrentControlSignal> { static constexpr MessageID value = MessageID::BusCurrentControl; };
template<> struct MessageOf<ResetCommandSignal> { static constexpr MessageID value = MessageID::ResetCommand; };
template<> struct MessageOf<BmsCellDataSignal> { static constexpr MessageID value = MessageID::BmsCellData; };
template<> struct MessageOf<BmsThermistorBroadcastSignal> { static constexpr MessageID value = MessageID::BmsThermistorBroadcast; };
template<> struct MessageOf<BmsCellDataBroadcastSignal> { static constexpr MessageID value = MessageID::BmsCellDataBroadcast; };
template<> struct MessageOf<PeripheralControlSignal> { static constexpr MessageID value = MessageID::PeripheralControl; };
template<> struct MessageOf<OrionFunctionalRequestSignal> { static constexpr MessageID value = MessageID::OrionFunctionalRequest; };
template<> struct MessageOf<OrionDiagRequestSignal> { static constexpr MessageID value = MessageID::OrionDiagRequest; };
template<> struct MessageOf<OrionThirdPartyRequestSignal> { static constexpr MessageID value = MessageID::OrionThirdPartyRequest; };
template<> struct MessageOf<OrionDiagResponseSignal> { static constexpr MessageID value = MessageID::OrionDiagResponse; };
template<> struct MessageOf<OrionThirdPartyResponseSignal> { static constexpr MessageID value = MessageID::OrionThirdPartyResponse; };
template<> struct MessageOf<Charger1CommunicationSignal> { static constexpr MessageID value = MessageID::Charger1Communication; };
template<> struct MessageOf<Charger2CommunicationSignal> { static constexpr MessageID value = MessageID::Charger2Communication; };
template<> struct MessageOf<Charger3CommunicationSignal> { static constexpr MessageID value = MessageID::Charger3Communication; };

// ─── Accessors ────────────────────────────────────────────────────────────────

// Returns the bit position (0–7) of a bit-field enum value within its byte.
template<typename BitEnum>
constexpr uint8_t getBitPosition(BitEnum bit)
{
    return static_cast<uint8_t>(bit);
}

// Returns the MessageDefinition for a given MessageID.
constexpr const MessageDefinition& getMessageDefinition(MessageID id)
{
    return artemis_messages[static_cast<uint8_t>(id)];
}

// Returns the can ID for a given MessageID.
constexpr uint32_t getID(MessageID id)
{
    return artemis_messages[static_cast<uint8_t>(id)].id;
}

// Returns the SignalDefinition for a signal enum value.
template<typename SignalEnum>
constexpr const SignalDefinition& getSignal(SignalEnum sig)
{
    return artemis_messages[static_cast<uint8_t>(MessageOf<SignalEnum>::value)]
                            .signals[static_cast<uint8_t>(sig)];
}

// In-place reverse bytes for Big endian signals.
template<typename Byte>
void dataEndian(Byte * buffer, MessageID id)
{
    MessageDefinition definition = getMessageDefinition(id);
    for(size_t i = 0; i < definition.signalCount; i++)
    {
        SignalDefinition signal = definition.signals[i];
        if (signal.endian == Endianness::Big)
        {
            auto elem_start = signal.startBit / (8u * sizeof(buffer[0]));
            auto elem_end   = (signal.startBit + signal.length) / (8u * sizeof(buffer[0]));
            std::reverse(buffer + elem_start, buffer + elem_end);
        }
    }
}

template<typename Byte>
void dataEndianFlip(Byte * buffer, MessageID id)
{
    MessageDefinition definition = getMessageDefinition(id);
    for(size_t i = 0; i < definition.signalCount; i++)
    {
        SignalDefinition signal = definition.signals[i];
        auto elem_start = signal.startBit / (8u * sizeof(buffer[0]));
        auto elem_end   = (signal.startBit + signal.length) / (8u * sizeof(buffer[0]));
        std::reverse(buffer + elem_start, buffer + elem_end);
    }
}

constexpr uint8_t getNumCanBytesMessage(MessageID id)
{
    MessageDefinition message = getMessageDefinition(id);
    if (message.signalCount == 0) return 0;
    SignalDefinition lastSignal = message.signals[message.signalCount - 1];
    return static_cast<uint8_t>((lastSignal.startBit + lastSignal.length + 7) / 8);
}

template<typename Shift>
constexpr uint8_t sbit(Shift s)
{
    return 1u << static_cast<uint8_t>(s);
}
