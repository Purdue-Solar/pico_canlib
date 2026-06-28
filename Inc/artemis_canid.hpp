#pragma once
#if defined(STM32_PROCESSOR)
#include <cstdint>
#else
#include "pico/stdlib.h"
#endif
#include <algorithm>
#include <cmath>

/*
Example usage: 
If I want to use can ID 0x402 (motor bus voltage/current) in a different repo, 
I would do getID(MotorBusVoltageCurrent).id in my program. This way, my
program doesn't need to know what the can ID actually is, and we have one
source that is the gold standard. 

If I want to know about all of the signals for 0x300 (power distro + display), 
I would use getMessageDefinition(PowerDistroDisplay).signals. Then, if I wanted
to know the bits in a particular signal, let's say distro_display_main_bits,
I would do getSignal(distro_display_main_bits)
Finally, if I wanted to use a specific bit, I would do 
getSignal(distro_display_main_bits).bitindex[MainOverCurrentError].

Everything in this file is evaluated at compile time
*/

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

// ─── 0x200  BMS Safety Critical ──────────────────────────────────────────────

enum class BmsDtcFlags1 : uint8_t
{
    DischargeLimitEnforcementFault  = 0,
    ChargerSafetyRelayFault         = 1,
    InternalHardwareFault           = 2,
    InternalHeatsinkThermistorFault = 3,
    InternalSoftwareFault           = 4,
    HighestCellVoltageTooLowFault   = 5,
    LowestCellVoltageTooLowFault    = 6,
    PackTooHotFault                 = 7,
};

inline constexpr BitDefinition bms_dtc_flags1_bits[] = {
    {"Discharge limit enforcement fault",   static_cast<uint8_t>(BmsDtcFlags1::DischargeLimitEnforcementFault)},
    {"Charger safety relay fault",          static_cast<uint8_t>(BmsDtcFlags1::ChargerSafetyRelayFault)},
    {"Internal hardware fault",             static_cast<uint8_t>(BmsDtcFlags1::InternalHardwareFault)},
    {"Internal heatsink thermistor fault",  static_cast<uint8_t>(BmsDtcFlags1::InternalHeatsinkThermistorFault)},
    {"Internal software fault",             static_cast<uint8_t>(BmsDtcFlags1::InternalSoftwareFault)},
    {"Highest cell voltage too low fault",  static_cast<uint8_t>(BmsDtcFlags1::HighestCellVoltageTooLowFault)},
    {"Lowest cell voltage too low fault",   static_cast<uint8_t>(BmsDtcFlags1::LowestCellVoltageTooLowFault)},
    {"Pack too hot fault",                  static_cast<uint8_t>(BmsDtcFlags1::PackTooHotFault)},
};

enum class BmsDtcFlags21 : uint8_t
{
    InternalCommunicationFault     = 0,
    CellBalancingStuckOffFault     = 1,
    WeakCellFault                  = 2,
    LowCellVoltageFault            = 3,
    OpenWiringFault                = 4,
    CurrentSensorFault             = 5,
    HighestCellVoltageAbove5vFault = 6,
    CellAsicFault                  = 7,
};

inline constexpr BitDefinition bms_dtc_flags2_1_bits[] = {
    {"Internal communication fault",        static_cast<uint8_t>(BmsDtcFlags21::InternalCommunicationFault)},
    {"Cell balancing stuck off fault",      static_cast<uint8_t>(BmsDtcFlags21::CellBalancingStuckOffFault)},
    {"Weak cell fault",                     static_cast<uint8_t>(BmsDtcFlags21::WeakCellFault)},
    {"Low cell voltage fault",              static_cast<uint8_t>(BmsDtcFlags21::LowCellVoltageFault)},
    {"Open wiring fault",                   static_cast<uint8_t>(BmsDtcFlags21::OpenWiringFault)},
    {"Current sensor fault",                static_cast<uint8_t>(BmsDtcFlags21::CurrentSensorFault)},
    {"Highest cell voltage above 5v fault", static_cast<uint8_t>(BmsDtcFlags21::HighestCellVoltageAbove5vFault)},
    {"Cell ASIC Fault",                     static_cast<uint8_t>(BmsDtcFlags21::CellAsicFault)},
};

enum class BmsDtcFlags22 : uint8_t
{
    WeakPackFault               = 0,
    FanMonitorFault             = 1,
    ThermistorFault             = 2,
    ExternalCommunicationFault  = 3,
    RedundantPowerSupplyFault   = 4,
    HighVoltageIsolationFault   = 5,
    InputPowerSupplyFault       = 6,
    ChargeLimitEnforcementFault = 7,
};

inline constexpr BitDefinition bms_dtc_flags2_2_bits[] = {
    {"Weak pack fault",                 static_cast<uint8_t>(BmsDtcFlags22::WeakPackFault)},
    {"Fan monitor fault",               static_cast<uint8_t>(BmsDtcFlags22::FanMonitorFault)},
    {"Thermistor fault",                static_cast<uint8_t>(BmsDtcFlags22::ThermistorFault)},
    {"External communication fault",    static_cast<uint8_t>(BmsDtcFlags22::ExternalCommunicationFault)},
    {"Redundant power supply fault",    static_cast<uint8_t>(BmsDtcFlags22::RedundantPowerSupplyFault)},
    {"High voltage isolation fault",    static_cast<uint8_t>(BmsDtcFlags22::HighVoltageIsolationFault)},
    {"Input power supply fault",        static_cast<uint8_t>(BmsDtcFlags22::InputPowerSupplyFault)},
    {"Charge limit enforcement fault",  static_cast<uint8_t>(BmsDtcFlags22::ChargeLimitEnforcementFault)},
};

enum class BmsRelayState1 : uint8_t
{
    DischargeRelayEnabled   = 0,
    ChargeRelayEnabled      = 1,
    ChargerSafetyEnabled    = 2,
    DtcIndicatorActive      = 3,
    MultiPurposeInputStatus = 4,
    AlwaysOnStatus          = 5,
    IsReadyStatus           = 6,
    IsChargingStatus        = 7,
};

inline constexpr BitDefinition bms_relay_state1_bits[] = {
    {"Discharge relay enabled",     static_cast<uint8_t>(BmsRelayState1::DischargeRelayEnabled)},
    {"Charge relay enabled",        static_cast<uint8_t>(BmsRelayState1::ChargeRelayEnabled)},
    {"Charger safety enabled",      static_cast<uint8_t>(BmsRelayState1::ChargerSafetyEnabled)},
    {"DTC indicator active",        static_cast<uint8_t>(BmsRelayState1::DtcIndicatorActive)},
    {"Multi-Purpose input status",  static_cast<uint8_t>(BmsRelayState1::MultiPurposeInputStatus)},
    {"Always-on status",            static_cast<uint8_t>(BmsRelayState1::AlwaysOnStatus)},
    {"Is-ready status",             static_cast<uint8_t>(BmsRelayState1::IsReadyStatus)},
    {"Is-charging status",          static_cast<uint8_t>(BmsRelayState1::IsChargingStatus)},
};

enum class BmsRelayState2 : uint8_t
{
    MultiPurposeInput2Status  = 0,
    MultiPurposeInput3Status  = 1,
    Reserved                  = 2,
    MultiPurposeOutput2Status = 3,
    MultiPurposeOutput3Status = 4,
    MultiPurposeOutput4Status = 5,
    MultiPurposeEnableStatus  = 6,
    MultiPurposeOutput1Status = 7,
};

inline constexpr BitDefinition bms_relay_state2_bits[] = {
    {"Multi-purpose input 2 status",    static_cast<uint8_t>(BmsRelayState2::MultiPurposeInput2Status)},
    {"Multi-purpose input 3 status",    static_cast<uint8_t>(BmsRelayState2::MultiPurposeInput3Status)},
    {"Reserved",                        static_cast<uint8_t>(BmsRelayState2::Reserved)},
    {"Multi-Purpose Output 2 status",   static_cast<uint8_t>(BmsRelayState2::MultiPurposeOutput2Status)},
    {"Multi-Purpose Output 3 status",   static_cast<uint8_t>(BmsRelayState2::MultiPurposeOutput3Status)},
    {"Multi-Purpose Output 4 status",   static_cast<uint8_t>(BmsRelayState2::MultiPurposeOutput4Status)},
    {"Multi-Purpose enable status",     static_cast<uint8_t>(BmsRelayState2::MultiPurposeEnableStatus)},
    {"Multi-Purpose Output 1 status",   static_cast<uint8_t>(BmsRelayState2::MultiPurposeOutput1Status)},
};

enum class BmsFailsafe : uint8_t
{
    VoltageFailsafeActive          = 0,
    CurrentFailsafeActive          = 1,
    RelayFailsafeActive            = 2,
    CellBalancingActive            = 3,
    ChargeInterlockFailsafeActive  = 4,
    ThermistorBValueTableInvalid   = 5,
    InputPowerSupplyFailsafeActive = 6,
    Reserved                       = 7,
};

inline constexpr BitDefinition bms_failsafe_bits[] = {
    {"Voltage failsafe active",             static_cast<uint8_t>(BmsFailsafe::VoltageFailsafeActive)},
    {"Current failsafe active",             static_cast<uint8_t>(BmsFailsafe::CurrentFailsafeActive)},
    {"Relay failsafe active",               static_cast<uint8_t>(BmsFailsafe::RelayFailsafeActive)},
    {"Cell balancing active",               static_cast<uint8_t>(BmsFailsafe::CellBalancingActive)},
    {"Charge interlock failsafe active",    static_cast<uint8_t>(BmsFailsafe::ChargeInterlockFailsafeActive)},
    {"Thermistor B-value table invalid",    static_cast<uint8_t>(BmsFailsafe::ThermistorBValueTableInvalid)},
    {"Input power supply failsafe active",  static_cast<uint8_t>(BmsFailsafe::InputPowerSupplyFailsafeActive)},
    {"Reserved",                            static_cast<uint8_t>(BmsFailsafe::Reserved)},
};

inline constexpr SignalDefinition bms_signals[] = {
    {"DTC Flags 1",       0,  8, Endianness::Big, bms_dtc_flags1_bits,   array_size(bms_dtc_flags1_bits)},
    {"DTC Flags 2.1",     8,  8, Endianness::Big, bms_dtc_flags2_1_bits, array_size(bms_dtc_flags2_1_bits)},
    {"DTC Flags 2.2",     16, 8, Endianness::Big, bms_dtc_flags2_2_bits, array_size(bms_dtc_flags2_2_bits)},
    {"Relay State 1",     24, 8, Endianness::Big, bms_relay_state1_bits, array_size(bms_relay_state1_bits)},
    {"Relay State 2",     32, 8, Endianness::Big, bms_relay_state2_bits, array_size(bms_relay_state2_bits)},
    {"Failsafe Statuses", 40, 8, Endianness::Big, bms_failsafe_bits,     array_size(bms_failsafe_bits)},
    {"Pack SOC",          48, 8, Endianness::Big, nullptr,               0},
    {"Adaptive Pack SOC", 56, 8, Endianness::Big, nullptr,               0},
};

// ─── 0x300  Power Distro Display ─────────────────────────────────────────────

enum class DistroDisplayMisc : uint8_t
{
    DistroBad                 = 0,
    MainHardwareDetectedFault = 1,
    AuxHardwareDetectedFault  = 2,
    MainPowerMonitorI2cError  = 3,
    AuxPowerMonitorI2cError   = 4,
    PowerDistroPrecharge      = 5,
    ContactorFault            = 6,
    Empty7                    = 7,
};

inline constexpr BitDefinition distro_display_misc_bits[] = {
    {"Power distro error exists",       static_cast<uint8_t>(DistroDisplayMisc::DistroBad)},
    {"Main hardware detected fault",    static_cast<uint8_t>(DistroDisplayMisc::MainHardwareDetectedFault)},
    {"Aux hardware detected fault",     static_cast<uint8_t>(DistroDisplayMisc::AuxHardwareDetectedFault)},
    {"Main Power monitor i2c error",    static_cast<uint8_t>(DistroDisplayMisc::MainPowerMonitorI2cError)},
    {"Aux Power monitor i2c error",     static_cast<uint8_t>(DistroDisplayMisc::AuxPowerMonitorI2cError)},
    {"PowerDistro Precharge",           static_cast<uint8_t>(DistroDisplayMisc::PowerDistroPrecharge)},
    {"Contactor failed",                static_cast<uint8_t>(DistroDisplayMisc::ContactorFault)},
    {"Empty",                           static_cast<uint8_t>(DistroDisplayMisc::Empty7)},
};

enum class DistroDisplayMain : uint8_t
{
    MainOverVoltageError    = 0,
    MainUnderVoltageError   = 1,
    MainOverCurrentError    = 2,
    MainUnderCurrentError   = 3,
    MainOverVoltageWarning  = 4,
    MainUnderVoltageWarning = 5,
    MainOverCurrentWarning  = 6,
    MainUnderCurrentWarning = 7,
};

inline constexpr BitDefinition distro_display_main_bits[] = {
    {"Main over voltage error",     static_cast<uint8_t>(DistroDisplayMain::MainOverVoltageError)},
    {"Main under voltage error",    static_cast<uint8_t>(DistroDisplayMain::MainUnderVoltageError)},
    {"Main over current error",     static_cast<uint8_t>(DistroDisplayMain::MainOverCurrentError)},
    {"Main under current error",    static_cast<uint8_t>(DistroDisplayMain::MainUnderCurrentError)},
    {"Main over voltage warning",   static_cast<uint8_t>(DistroDisplayMain::MainOverVoltageWarning)},
    {"Main under voltage warning",  static_cast<uint8_t>(DistroDisplayMain::MainUnderVoltageWarning)},
    {"Main over current warning",   static_cast<uint8_t>(DistroDisplayMain::MainOverCurrentWarning)},
    {"Main under current warning",  static_cast<uint8_t>(DistroDisplayMain::MainUnderCurrentWarning)},
};

enum class DistroDisplayAux : uint8_t
{
    AuxOverVoltageError    = 0,
    AuxUnderVoltageError   = 1,
    AuxOverCurrentError    = 2,
    AuxUnderCurrentError   = 3,
    AuxOverVoltageWarning  = 4,
    AuxUnderVoltageWarning = 5,
    AuxOverCurrentWarning  = 6,
    AuxUnderCurrentWarning = 7,
};

inline constexpr BitDefinition distro_display_aux_bits[] = {
    {"Aux over voltage error",      static_cast<uint8_t>(DistroDisplayAux::AuxOverVoltageError)},
    {"Aux under voltage error",     static_cast<uint8_t>(DistroDisplayAux::AuxUnderVoltageError)},
    {"Aux over current error",      static_cast<uint8_t>(DistroDisplayAux::AuxOverCurrentError)},
    {"Aux under current error",     static_cast<uint8_t>(DistroDisplayAux::AuxUnderCurrentError)},
    {"Aux over voltage warning",    static_cast<uint8_t>(DistroDisplayAux::AuxOverVoltageWarning)},
    {"Aux under voltage warning",   static_cast<uint8_t>(DistroDisplayAux::AuxUnderVoltageWarning)},
    {"Aux over current warning",    static_cast<uint8_t>(DistroDisplayAux::AuxOverCurrentWarning)},
    {"Aux under current warning",   static_cast<uint8_t>(DistroDisplayAux::AuxUnderCurrentWarning)},
};

inline constexpr SignalDefinition distro_display_signals[] = {
    {"Display Flags Power Monitor", 0,  8, Endianness::Big, distro_display_misc_bits, array_size(distro_display_misc_bits)},
    {"Display Flags Main Battery",  8,  8, Endianness::Big, distro_display_main_bits, array_size(distro_display_main_bits)},
    {"Display Flags Aux Battery",   16, 8, Endianness::Big, distro_display_aux_bits,  array_size(distro_display_aux_bits)},
};

// ─── 0x400  Motor Controller ID ──────────────────────────────────────────────

inline constexpr SignalDefinition mc_id_signals[] = {
    {"Prohelion ID",  0,  32, Endianness::Little, nullptr, 0},
    {"Serial Number", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x401  Motor Controller Errors ──────────────────────────────────────────

enum class McLimitFlags : uint8_t
{
    OutputVoltagePwm     = 0,
    MotorCurrent         = 1,
    Velocity             = 2,
    BusCurrent           = 3,
    BusVoltageUpperLimit = 4,
    BusVoltageLowerLimit = 5,
    IpmMotorTemperature  = 6,
    Reserved             = 7,
};

inline constexpr BitDefinition mc_limit_flags_bits[] = {
    {"Output Voltage PWM",      static_cast<uint8_t>(McLimitFlags::OutputVoltagePwm)},
    {"Motor Current",           static_cast<uint8_t>(McLimitFlags::MotorCurrent)},
    {"Velocity",                static_cast<uint8_t>(McLimitFlags::Velocity)},
    {"Bus Current",             static_cast<uint8_t>(McLimitFlags::BusCurrent)},
    {"Bus Voltage Upper Limit", static_cast<uint8_t>(McLimitFlags::BusVoltageUpperLimit)},
    {"Bus Voltage Lower Limit", static_cast<uint8_t>(McLimitFlags::BusVoltageLowerLimit)},
    {"IPM/Motor Temperature",   static_cast<uint8_t>(McLimitFlags::IpmMotorTemperature)},
    {"Reserved",                static_cast<uint8_t>(McLimitFlags::Reserved)},
};

enum class McErrorFlags1 : uint8_t
{
    HardwareOverCurrent          = 0,
    SoftwareOverCurrent          = 1,
    DcBusOverVoltage             = 2,
    BadMotorPositionHallSequence = 3,
    WatchdogCausedLastReset      = 4,
    ConfigReadError              = 5,
    Rail15vUnderVoltageLockOut   = 6,
    DesaturationFault            = 7,
};

inline constexpr BitDefinition mc_error_flags1_bits[] = {
    {"Hardware over current",            static_cast<uint8_t>(McErrorFlags1::HardwareOverCurrent)},
    {"Software over current",            static_cast<uint8_t>(McErrorFlags1::SoftwareOverCurrent)},
    {"DC Bus over voltage",              static_cast<uint8_t>(McErrorFlags1::DcBusOverVoltage)},
    {"Bad motor position hall sequence", static_cast<uint8_t>(McErrorFlags1::BadMotorPositionHallSequence)},
    {"Watchdog caused last reset",       static_cast<uint8_t>(McErrorFlags1::WatchdogCausedLastReset)},
    {"Config read error",                static_cast<uint8_t>(McErrorFlags1::ConfigReadError)},
    {"15V Rail under voltage lock out",  static_cast<uint8_t>(McErrorFlags1::Rail15vUnderVoltageLockOut)},
    {"Desaturation Fault",               static_cast<uint8_t>(McErrorFlags1::DesaturationFault)},
};

enum class McErrorFlags2 : uint8_t
{
    SpeedOver115Percent = 0,
    Reserved1           = 1,
    Reserved2           = 2,
    Reserved3           = 3,
    Reserved4           = 4,
    Reserved5           = 5,
    Reserved6           = 6,
    Reserved7           = 7,
};

inline constexpr BitDefinition mc_error_flags2_bits[] = {
    {"Speed (115% max RPM)", static_cast<uint8_t>(McErrorFlags2::SpeedOver115Percent)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved1)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved2)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved3)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved4)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved5)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved6)},
    {"Reserved",             static_cast<uint8_t>(McErrorFlags2::Reserved7)},
};

inline constexpr SignalDefinition mc_error_signals[] = {
    {"MC Limit Flags",     0,  8,  Endianness::Big, mc_limit_flags_bits,  array_size(mc_limit_flags_bits)},
    {"Reserved",           8,  8,  Endianness::Big, nullptr,              0},
    {"MC Error Flags 1",   16, 8,  Endianness::Big, mc_error_flags1_bits, array_size(mc_error_flags1_bits)},
    {"MC Error Flags 2",   24, 8,  Endianness::Big, mc_error_flags2_bits, array_size(mc_error_flags2_bits)},
    {"Active Motor Index", 32, 16, Endianness::Big, nullptr,              0},
    {"CAN TX Error Count", 48, 8,  Endianness::Big, nullptr,              0},
    {"CAN RX Error Count", 56, 8,  Endianness::Big, nullptr,              0},
};

// ─── 0x402  Motor Bus Voltage / Current ──────────────────────────────────────

inline constexpr SignalDefinition mc_bus_signals[] = {
    {"Bus Voltage", 0,  32, Endianness::Little, nullptr, 0},
    {"Bus Current", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x403  Motor Speed ───────────────────────────────────────────────────────

inline constexpr SignalDefinition mc_speed_signals[] = {
    {"Motor Velocity",   0,  32, Endianness::Little, nullptr, 0},
    {"Vehicle Velocity", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x40B  Motor Temperature ────────────────────────────────────────────────

inline constexpr SignalDefinition mc_temp_signals[] = {
    {"Motor Temp",     0,  32, Endianness::Little, nullptr, 0},
    {"Heat Sink Temp", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x501  Motor Current / Velocity Control ─────────────────────────────────

inline constexpr SignalDefinition motor_control_signals[] = {
    {"Motor Velocity Setpoint", 0,  32, Endianness::Little, nullptr, 0},
    {"Motor Current Setpoint",  32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x502  Bus Current Control ──────────────────────────────────────────────

inline constexpr SignalDefinition bus_control_signals[] = {
    {"Reserved",             0,  32, Endianness::Little, nullptr, 0},
    {"Bus Current Setpoint", 32, 32, Endianness::Little, nullptr, 0},
};

// ─── 0x600  Peripheral Control ───────────────────────────────────────────────

enum class DistroControl : uint8_t
{
    LeftLights   = 0,
    RightLights  = 1,
    HazardLights = 2,
    Horn         = 3,
    Brake       = 4,
    Empty5       = 5,
    Empty6       = 6,
    Empty7       = 7,
};

inline constexpr BitDefinition distro_control_bits[] = {
    {"Left Lights",   static_cast<uint8_t>(DistroControl::LeftLights)},
    {"Right Lights",  static_cast<uint8_t>(DistroControl::RightLights)},
    {"Hazard Lights", static_cast<uint8_t>(DistroControl::HazardLights)},
    {"Horn",          static_cast<uint8_t>(DistroControl::Horn)},
    {"Brake",         static_cast<uint8_t>(DistroControl::Brake)},
    {"Empty",         static_cast<uint8_t>(DistroControl::Empty5)},
    {"Empty",         static_cast<uint8_t>(DistroControl::Empty6)},
    {"Empty",         static_cast<uint8_t>(DistroControl::Empty7)},
};

inline constexpr SignalDefinition distro_control_signals[] = {
    {"Distro Control", 0, 8, Endianness::Big, distro_control_bits, array_size(distro_control_bits)},
};

// ─── 0x202  BMS Temperature / Current Limits / Power ─────────────────────────

inline constexpr SignalDefinition bms_power_signals[] = {
    {"Pack DCL",             0,  8,  Endianness::Big, nullptr, 0},
    {"Pack CCL",             8,  8,  Endianness::Big, nullptr, 0},
    {"Low Temperature",      16, 8,  Endianness::Big, nullptr, 0},
    {"High Temperature",     24, 8,  Endianness::Big, nullptr, 0},
    {"Current Limits Status",32, 16, Endianness::Big, nullptr, 0},
    {"Adaptive Amphours",    48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x201  BMS Pack Voltage and Energy ──────────────────────────────────────

inline constexpr SignalDefinition bms_voltage_signals[] = {
    {"Pack kW Power",     0, 16,  Endianness::Big, nullptr, 0},
    {"Pack Amphours",     16, 16, Endianness::Big, nullptr, 0},
    {"Pack Current",      32, 16, Endianness::Big, nullptr, 0},
    {"Pack Voltage",      48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x702  BMS Cell Data ────────────────────────────────────────────────────

inline constexpr SignalDefinition bms_cell_signals[] = {
    {"Low Cell Voltage",     0,  16, Endianness::Big, nullptr, 0},
    {"High Cell Voltage",    16, 16, Endianness::Big, nullptr, 0},
    {"Low Cell Voltage ID",  32, 8,  Endianness::Big, nullptr, 0},
    {"High Cell Voltage ID", 40, 8,  Endianness::Big, nullptr, 0},
    {"Low Thermistor ID",    48, 8,  Endianness::Big, nullptr, 0},
    {"High Thermistor ID",   56, 8,  Endianness::Big, nullptr, 0},
};

// ─── 0x703  BMS Thermistor Broadcast ─────────────────────────────────────────

inline constexpr SignalDefinition bms_thermistor_signals[] = {
    {"Thermistor Data", 0, 64, Endianness::Big, nullptr, 0},
};

// ─── 0x704  BMS Cell Data Broadcast ──────────────────────────────────────────

inline constexpr SignalDefinition bms_cell_broadcast_signals[] = {
    {"Cell ID",             0,  8,  Endianness::Big, nullptr, 0},
    {"Instant Voltage",     8,  16, Endianness::Big, nullptr, 0},
    {"Internal Resistance", 24, 16, Endianness::Big, nullptr, 0},
    {"Open Voltage",        40, 16, Endianness::Big, nullptr, 0},
    {"Checksum",            56, 8,  Endianness::Big, nullptr, 0},
};

// ─── 0x1806E5F4 / 0x1806E7F4 / 0x1806E9F4  Charger Communication ─────────────

inline constexpr SignalDefinition charger_signals[] = {
    {"Maximum Pack Voltage", 48, 16, Endianness::Big, nullptr, 0},
    {"Pack CCL",             32, 16, Endianness::Big, nullptr, 0},
    {"Charger Control",      24, 8,  Endianness::Big, nullptr, 0},
};

// ─── Message and Signal ID Enums ─────────────────────────────────────────────

enum class MessageID : uint8_t
{
    BmsSafetyCritical           = 0,
    BmsPackVoltageAndEnergy     = 1,
    BmsTemperatureCurrentPower  = 2,
    PowerDistroDisplay          = 3,
    MotorControllerId           = 4,
    MotorControllerErrors       = 5,
    MotorBusVoltageCurrent      = 6,
    MotorSpeed                  = 7,
    MotorTemperature            = 8,
    MotorCurrentVelocityControl = 9,
    BusCurrentControl           = 10,
    PeripheralControl           = 11,
    BmsCellData                 = 12,
    BmsThermistorBroadcast      = 13,
    BmsCellDataBroadcast        = 14,
    Charger1Communication       = 15,
    Charger2Communication       = 16,
    Charger3Communication       = 17,
};

enum class BmsSignal : uint8_t
{
    DtcFlags1        = 0,
    DtcFlags21       = 1,
    DtcFlags22       = 2,
    RelayState1      = 3,
    RelayState2      = 4,
    FailsafeStatuses = 5,
    PackSoc          = 6,
    AdaptivePackSoc  = 7,
};

enum class BmsVoltageSignal : uint8_t
{
    PackKwPower      = 0,
    PackAmphours     = 1,
    PackCurrent      = 2,
    PackVoltage      = 3,
};

enum class DistroDisplaySignal : uint8_t
{
    DisplayFlagsPowerMonitor = 0,
    DisplayFlagsMainBattery  = 1,
    DisplayFlagsAuxBattery   = 2,
};

enum class McIdSignal : uint8_t
{
    ProhelionId  = 0,
    SerialNumber = 1,
};

enum class McErrorSignal : uint8_t
{
    McLimitFlags     = 0,
    Reserved         = 1,
    McErrorFlags1    = 2,
    McErrorFlags2    = 3,
    ActiveMotorIndex = 4,
    CanTxErrorCount  = 5,
    CanRxErrorCount  = 6,
};

enum class McBusSignal : uint8_t
{
    BusVoltage = 0,
    BusCurrent = 1,
};

enum class McSpeedSignal : uint8_t
{
    MotorVelocity   = 0,
    VehicleVelocity = 1,
};

enum class McTempSignal : uint8_t
{
    MotorTemp    = 0,
    HeatSinkTemp = 1,
};

enum class MotorControlSignal : uint8_t
{
    MotorVelocitySetpoint = 0,
    MotorCurrentSetpoint  = 1,
};

enum class BusControlSignal : uint8_t
{
    Reserved           = 0,
    BusCurrentSetpoint = 1,
};

enum class DistroControlSignal : uint8_t
{
    DistroControl = 0,
};

enum class BmsPowerSignal : uint8_t
{
    PackDcl             = 0,
    PackCcl             = 1,
    LowTemperature      = 2,
    HighTemperature     = 3,
    CurrentLimitsStatus = 4,
    AdaptiveAmphours    = 5,
};

enum class BmsCellSignal : uint8_t
{
    LowCellVoltage    = 0,
    HighCellVoltage   = 1,
    LowCellVoltageId  = 2,
    HighCellVoltageId = 3,
    LowThermistorId   = 4,
    HighThermistorId  = 5,
};

enum class BmsThermistorSignal : uint8_t
{
    ThermistorData = 0,
};

enum class BmsCellBroadcastSignal : uint8_t
{
    CellId             = 0,
    InstantVoltage     = 1,
    InternalResistance = 2,
    OpenVoltage        = 3,
    Checksum           = 4,
};

enum class ChargerSignal : uint8_t
{
    MaximumPackVoltage = 0,
    PackCcl            = 1,
    ChargerControl     = 2,
};

// ─── Master Message Table ─────────────────────────────────────────────────────
// Keep this table's row order in sync with the MessageID enum above.

inline constexpr MessageDefinition artemis_messages[] = {
    {"BMS Safety Critical",            0x200,      40,   bms_signals,                array_size(bms_signals)},
    {"BMS Pack Voltage and Energy",    0x201,      80,   bms_voltage_signals,        array_size(bms_voltage_signals)},
    {"BMS Temperature/Current/Power",  0x202,      80,   bms_power_signals,          array_size(bms_power_signals)},
    {"Power Distro Display",           0x300,      100,    distro_display_signals,     array_size(distro_display_signals)},
    {"Motor Controller ID",            0x400,      1000, mc_id_signals,              array_size(mc_id_signals)},
    {"Motor Controller Errors",        0x401,      200,  mc_error_signals,           array_size(mc_error_signals)},
    {"Motor Bus Voltage/Current",      0x402,      200,  mc_bus_signals,             array_size(mc_bus_signals)},
    {"Motor Speed",                    0x403,      200,  mc_speed_signals,           array_size(mc_speed_signals)},
    {"Motor Temperature",              0x40B,      1000, mc_temp_signals,            array_size(mc_temp_signals)},
    {"Motor Current/Velocity Control", 0x501,      100,  motor_control_signals,      array_size(motor_control_signals)},
    {"Bus Current Control",            0x502,      100,  bus_control_signals,        array_size(bus_control_signals)},
    {"Peripheral Control",             0x600,      100,  distro_control_signals,     array_size(distro_control_signals)},
    {"BMS Cell Data",                  0x701,      80,   bms_cell_signals,           array_size(bms_cell_signals)},
    {"BMS Thermistor Broadcast",       0x702,      0,    bms_thermistor_signals,     array_size(bms_thermistor_signals)},
    {"BMS Cell Data Broadcast",        0x703,      20,   bms_cell_broadcast_signals, array_size(bms_cell_broadcast_signals)},
    {"Charger 1 Communication",        0x1806E5F4, 808,  charger_signals,            array_size(charger_signals)},
    {"Charger 2 Communication",        0x1806E7F4, 808,  charger_signals,            array_size(charger_signals)},
    {"Charger 3 Communication",        0x1806E9F4, 808,  charger_signals,            array_size(charger_signals)},
};

// ─── Message-Signal Associations ─────────────────────────────────────────────
// Maps each signal enum to its owning message, enforcing correct pairing in getSignal().

template<typename SignalEnum> struct MessageOf; // intentionally undefined — instantiating with an unknown type is an error

template<> struct MessageOf<BmsSignal>             { static constexpr MessageID value = MessageID::BmsSafetyCritical; };
template<> struct MessageOf<BmsVoltageSignal>      { static constexpr MessageID value = MessageID::BmsPackVoltageAndEnergy; };
template<> struct MessageOf<DistroDisplaySignal>   { static constexpr MessageID value = MessageID::PowerDistroDisplay; };
template<> struct MessageOf<McIdSignal>            { static constexpr MessageID value = MessageID::MotorControllerId; };
template<> struct MessageOf<McErrorSignal>         { static constexpr MessageID value = MessageID::MotorControllerErrors; };
template<> struct MessageOf<McBusSignal>           { static constexpr MessageID value = MessageID::MotorBusVoltageCurrent; };
template<> struct MessageOf<McSpeedSignal>         { static constexpr MessageID value = MessageID::MotorSpeed; };
template<> struct MessageOf<McTempSignal>          { static constexpr MessageID value = MessageID::MotorTemperature; };
template<> struct MessageOf<MotorControlSignal>    { static constexpr MessageID value = MessageID::MotorCurrentVelocityControl; };
template<> struct MessageOf<BusControlSignal>      { static constexpr MessageID value = MessageID::BusCurrentControl; };
template<> struct MessageOf<DistroControlSignal>   { static constexpr MessageID value = MessageID::PeripheralControl; };
template<> struct MessageOf<BmsPowerSignal>        { static constexpr MessageID value = MessageID::BmsTemperatureCurrentPower; };
template<> struct MessageOf<BmsCellSignal>         { static constexpr MessageID value = MessageID::BmsCellData; };
template<> struct MessageOf<BmsThermistorSignal>   { static constexpr MessageID value = MessageID::BmsThermistorBroadcast; };
template<> struct MessageOf<BmsCellBroadcastSignal>{ static constexpr MessageID value = MessageID::BmsCellDataBroadcast; };
template<> struct MessageOf<ChargerSignal>         { static constexpr MessageID value = MessageID::Charger1Communication; };

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
constexpr const uint32_t getID(MessageID id)
{
    return artemis_messages[static_cast<uint8_t>(id)].id;
}

// Returns the SignalDefinition for a signal enum value.
// The message is derived automatically from the signal's type — passing a signal
// enum belonging to a different message is a compile-time error.
template<typename SignalEnum>
constexpr const SignalDefinition& getSignal(SignalEnum sig)
{
    return artemis_messages[static_cast<uint8_t>(MessageOf<SignalEnum>::value)]
                            .signals[static_cast<uint8_t>(sig)];
}

// Returns the can ID for a given MessageID.
template<typename Byte>
void dataEndian(Byte * buffer, MessageID id)
{
    MessageDefinition definition = getMessageDefinition(id);
    for(int i = 0; i < definition.signalCount; i++)
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

constexpr const uint8_t getNumCanBytesMessage(MessageID id)
{
    MessageDefinition message = getMessageDefinition(id);
    SignalDefinition lastSignal = message.signals[message.signalCount-1];
    return (lastSignal.startBit + lastSignal.length + 7) / 8; // ceil( (startBit + length) / 8)
}

template<typename Shift>
inline uint8_t sbit(Shift s)
{
    return 1u << static_cast<uint8_t>(s);
}
