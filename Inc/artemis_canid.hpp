#pragma once
#include "pico/stdlib.h"

enum class artemis_canid : uint32_t {
    tempAndSOC = 0x200,
    currentLimit = 0x201,
    battDiagnostic = 0x202,
    setMotorCurrent = 0x402,
    motorVelocity = 0x403,
    heatSinkTemp = 0x40B,
    steeringWheel = 0x301,
    motorCurrentCanID = 0x501u,
    motorVelocityCanID = 0x502u,
    powerDistroToSteering = 0x301u,
    steeringToPowerDistro = 0x701u
};

constexpr uint32_t canIDHelper(artemis_canid c)
{
    return static_cast<uint32_t>(c);
}

enum class Endianness
{
    Little,
    Big
};

enum class CanType
{
    FD,
    BX
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

static constexpr BitDefinition bms_dtc_flags1_bits[] = {
    {"Discharge limit enforcement fault",   0},
    {"Charger safety relay fault",          1},
    {"Internal hardware fault",             2},
    {"Internal heatsink thermistor fault",  3},
    {"Internal software fault",             4},
    {"Highest cell voltage too low fault",  5},
    {"Lowest cell voltage too low fault",   6},
    {"Pack too hot fault",                  7},
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

static constexpr BitDefinition bms_dtc_flags2_1_bits[] = {
    {"Internal communication fault",        0},
    {"Cell balancing stuck off fault",      1},
    {"Weak cell fault",                     2},
    {"Low cell voltage fault",              3},
    {"Open wiring fault",                   4},
    {"Current sensor fault",                5},
    {"Highest cell voltage above 5v fault", 6},
    {"Cell ASIC Fault",                     7},
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

static constexpr BitDefinition bms_dtc_flags2_2_bits[] = {
    {"Weak pack fault",                 0},
    {"Fan monitor fault",               1},
    {"Thermistor fault",                2},
    {"External communication fault",    3},
    {"Redundant power supply fault",    4},
    {"High voltage isolation fault",    5},
    {"Input power supply fault",        6},
    {"Charge limit enforcement fault",  7},
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

static constexpr BitDefinition bms_relay_state1_bits[] = {
    {"Discharge relay enabled",     0},
    {"Charge relay enabled",        1},
    {"Charger safety enabled",      2},
    {"DTC indicator active",        3},
    {"Multi-Purpose input status",  4},
    {"Always-on status",            5},
    {"Is-ready status",             6},
    {"Is-charging status",          7},
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

static constexpr BitDefinition bms_relay_state2_bits[] = {
    {"Multi-purpose input 2 status",    0},
    {"Multi-purpose input 3 status",    1},
    {"Reserved",                        2},
    {"Multi-Purpose Output 2 status",   3},
    {"Multi-Purpose Output 3 status",   4},
    {"Multi-Purpose Output 4 status",   5},
    {"Multi-Purpose enable status",     6},
    {"Multi-Purpose Output 1 status",   7},
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

static constexpr BitDefinition bms_failsafe_bits[] = {
    {"Voltage failsafe active",             0},
    {"Current failsafe active",             1},
    {"Relay failsafe active",               2},
    {"Cell balancing active",               3},
    {"Charge interlock failsafe active",    4},
    {"Thermistor B-value table invalid",    5},
    {"Input power supply failsafe active",  6},
    {"Reserved",                            7},
};

static constexpr SignalDefinition bms_signals[] = {
    {"DTC Flags 1",         0,  8, Endianness::Big, bms_dtc_flags1_bits,   8},
    {"DTC Flags 2.1",       8,  8, Endianness::Big, bms_dtc_flags2_1_bits, 8},
    {"DTC Flags 2.2",       16, 8, Endianness::Big, bms_dtc_flags2_2_bits, 8},
    {"Relay State 1",       24, 8, Endianness::Big, bms_relay_state1_bits, 8},
    {"Relay State 2",       32, 8, Endianness::Big, bms_relay_state2_bits, 8},
    {"Failsafe Statuses",   40, 8, Endianness::Big, bms_failsafe_bits,     8},
    {"Pack SOC",            48, 8, Endianness::Big, nullptr,               0},
    {"Adaptive Pack SOC",   56, 8, Endianness::Big, nullptr,               0},
};

// ─── 0x300  Power Distro Display ─────────────────────────────────────────────

enum class DistroDisplayMisc : uint8_t
{
    MainHardwareDetectedFault = 0,
    AuxHardwareDetectedFault  = 1,
    MainPowerMonitorI2cError  = 2,
    AuxPowerMonitorI2cError   = 3,
    PowerDistroWatchdog       = 4,
    Empty5                    = 5,
    Empty6                    = 6,
    Empty7                    = 7,
};

static constexpr BitDefinition distro_display_misc_bits[] = {
    {"Main hardware detected fault",    0},
    {"Aux hardware detected fault",     1},
    {"Main Power monitor i2c error",    2},
    {"Aux Power monitor i2c error",     3},
    {"PowerDistroWatchdog",             4},
    {"Empty",                           5},
    {"Empty",                           6},
    {"Empty",                           7},
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

static constexpr BitDefinition distro_display_main_bits[] = {
    {"Main over voltage error",     0},
    {"Main under voltage error",    1},
    {"Main over current error",     2},
    {"Main under current error",    3},
    {"Main over voltage warning",   4},
    {"Main under voltage warning",  5},
    {"Main over current warning",   6},
    {"Main under current warning",  7},
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

static constexpr BitDefinition distro_display_aux_bits[] = {
    {"Aux over voltage error",      0},
    {"Aux under voltage error",     1},
    {"Aux over current error",      2},
    {"Aux under current error",     3},
    {"Aux over voltage warning",    4},
    {"Aux under voltage warning",   5},
    {"Aux over current warning",    6},
    {"Aux under current warning",   7},
};

static constexpr SignalDefinition distro_display_signals[] = {
    {"Display Flags Power Monitor", 0,  8, Endianness::Big, distro_display_misc_bits, 8},
    {"Display Flags Main Battery",  8,  8, Endianness::Big, distro_display_main_bits, 8},
    {"Display Flags Aux Battery",   16, 8, Endianness::Big, distro_display_aux_bits,  8},
};

// ─── 0x400  Motor Controller ID ──────────────────────────────────────────────

static constexpr SignalDefinition mc_id_signals[] = {
    {"Prohelion ID",    0,  32, Endianness::Big, nullptr, 0},
    {"Serial Number",   32, 32, Endianness::Big, nullptr, 0},
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

static constexpr BitDefinition mc_limit_flags_bits[] = {
    {"Output Voltage PWM",      0},
    {"Motor Current",           1},
    {"Velocity",                2},
    {"Bus Current",             3},
    {"Bus Voltage Upper Limit", 4},
    {"Bus Voltage Lower Limit", 5},
    {"IPM/Motor Temperature",   6},
    {"Reserved",                7},
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

static constexpr BitDefinition mc_error_flags1_bits[] = {
    {"Hardware over current",           0},
    {"Software over current",           1},
    {"DC Bus over voltage",             2},
    {"Bad motor position hall sequence",3},
    {"Watchdog caused last reset",      4},
    {"Config read error",               5},
    {"15V Rail under voltage lock out", 6},
    {"Desaturation Fault",              7},
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

static constexpr BitDefinition mc_error_flags2_bits[] = {
    {"Speed (115% max RPM)", 0},
    {"Reserved",             1},
    {"Reserved",             2},
    {"Reserved",             3},
    {"Reserved",             4},
    {"Reserved",             5},
    {"Reserved",             6},
    {"Reserved",             7},
};

static constexpr SignalDefinition mc_error_signals[] = {
    {"MC Limit Flags",      0,  8,  Endianness::Big, mc_limit_flags_bits,  8},
    {"Reserved",            8,  8,  Endianness::Big, nullptr,              0},
    {"MC Error Flags 1",    16, 8,  Endianness::Big, mc_error_flags1_bits, 8},
    {"MC Error Flags 2",    24, 8,  Endianness::Big, mc_error_flags2_bits, 8},
    {"Active Motor Index",  32, 16, Endianness::Big, nullptr,              0},
    {"CAN TX Error Count",  48, 8,  Endianness::Big, nullptr,              0},
    {"CAN RX Error Count",  56, 8,  Endianness::Big, nullptr,              0},
};

// ─── 0x402  Motor Bus Voltage / Current ──────────────────────────────────────

static constexpr SignalDefinition mc_bus_signals[] = {
    {"Bus Voltage", 0,  32, Endianness::Big, nullptr, 0},
    {"Bus Current", 32, 32, Endianness::Big, nullptr, 0},
};

// ─── 0x403  Motor Speed ───────────────────────────────────────────────────────

static constexpr SignalDefinition mc_speed_signals[] = {
    {"Motor Velocity",   0,  32, Endianness::Big, nullptr, 0},
    {"Vehicle Velocity", 32, 32, Endianness::Big, nullptr, 0},
};

// ─── 0x40B  Motor Temperature ────────────────────────────────────────────────

static constexpr SignalDefinition mc_temp_signals[] = {
    {"Motor Temp",     0,  32, Endianness::Big, nullptr, 0},
    {"Heat Sink Temp", 32, 32, Endianness::Big, nullptr, 0},
};

// ─── 0x501  Motor Current / Velocity Control ─────────────────────────────────

static constexpr SignalDefinition motor_control_signals[] = {
    {"Motor Velocity Setpoint", 0,  32, Endianness::Big, nullptr, 0},
    {"Motor Current Setpoint",  32, 32, Endianness::Big, nullptr, 0},
};

// ─── 0x502  Bus Current Control ──────────────────────────────────────────────

static constexpr SignalDefinition bus_control_signals[] = {
    {"Reserved",             0,  32, Endianness::Big, nullptr, 0},
    {"Bus Current Setpoint", 32, 32, Endianness::Big, nullptr, 0},
};

// ─── 0x600  Peripheral Control ───────────────────────────────────────────────

enum class DistroControl : uint8_t
{
    LeftLights   = 0,
    RightLights  = 1,
    HazardLights = 2,
    Horn         = 3,
    Empty4       = 4,
    Empty5       = 5,
    Empty6       = 6,
    Empty7       = 7,
};

static constexpr BitDefinition distro_control_bits[] = {
    {"Left Lights",   0},
    {"Right Lights",  1},
    {"Hazard Lights", 2},
    {"Horn",          3},
    {"Empty",         4},
    {"Empty",         5},
    {"Empty",         6},
    {"Empty",         7},
};

static constexpr SignalDefinition distro_control_signals[] = {
    {"Distro Control", 0, 8, Endianness::Big, distro_control_bits, 8},
};

// ─── 0x700  BMS Temperature / Current Limits / Power ─────────────────────────

static constexpr SignalDefinition bms_power_signals[] = {
    {"Pack DCL",             0,  8,  Endianness::Big, nullptr, 0},
    {"Pack CCL",             8,  8,  Endianness::Big, nullptr, 0},
    {"Low Temperature",      16, 8,  Endianness::Big, nullptr, 0},
    {"High Temperature",     24, 8,  Endianness::Big, nullptr, 0},
    {"Current Limits Status",32, 16, Endianness::Big, nullptr, 0},
    {"Pack kW Power",        48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x701  BMS Pack Voltage and Energy ──────────────────────────────────────

static constexpr SignalDefinition bms_voltage_signals[] = {
    {"Pack Amphours",     0,  16, Endianness::Big, nullptr, 0},
    {"Adaptive Amphours", 16, 16, Endianness::Big, nullptr, 0},
    {"Pack Current",      32, 16, Endianness::Big, nullptr, 0},
    {"Pack Voltage",      48, 16, Endianness::Big, nullptr, 0},
};

// ─── 0x702  BMS Cell Data ────────────────────────────────────────────────────

static constexpr SignalDefinition bms_cell_signals[] = {
    {"Low Cell Voltage",     0,  16, Endianness::Big, nullptr, 0},
    {"High Cell Voltage",    16, 16, Endianness::Big, nullptr, 0},
    {"Low Cell Voltage ID",  32, 8,  Endianness::Big, nullptr, 0},
    {"High Cell Voltage ID", 40, 8,  Endianness::Big, nullptr, 0},
    {"Low Thermistor ID",    48, 8,  Endianness::Big, nullptr, 0},
    {"High Thermistor ID",   56, 8,  Endianness::Big, nullptr, 0},
};

// ─── 0x703  BMS Thermistor Broadcast ─────────────────────────────────────────

static constexpr SignalDefinition bms_thermistor_signals[] = {
    {"Thermistor Data", 0, 64, Endianness::Big, nullptr, 0},
};

// ─── 0x704  BMS Cell Data Broadcast ──────────────────────────────────────────

static constexpr SignalDefinition bms_cell_broadcast_signals[] = {
    {"Cell ID",             0,  8,  Endianness::Big, nullptr, 0},
    {"Instant Voltage",     8,  16, Endianness::Big, nullptr, 0},
    {"Internal Resistance", 24, 16, Endianness::Big, nullptr, 0},
    {"Open Voltage",        40, 16, Endianness::Big, nullptr, 0},
    {"Checksum",            56, 8,  Endianness::Big, nullptr, 0},
};

// ─── 0x1806E5F4 / 0x1806E7F4 / 0x1806E9F4  Charger Communication ─────────────

static constexpr SignalDefinition charger_signals[] = {
    {"Maximum Pack Voltage", 48, 16, Endianness::Big, nullptr, 0},
    {"Pack CCL",             32, 16, Endianness::Big, nullptr, 0},
    {"Charger Control",      24, 8,  Endianness::Big, nullptr, 0},
};

// ─── Master Message Table ─────────────────────────────────────────────────────

static constexpr MessageDefinition artemis_messages[] = {
    {"BMS Safety Critical",             0x200,       40,   bms_signals,                8},
    {"Power Distro Display",            0x300,       0,    distro_display_signals,     3},
    {"Motor Controller ID",             0x400,       1000, mc_id_signals,              2},
    {"Motor Controller Errors",         0x401,       200,  mc_error_signals,           7},
    {"Motor Bus Voltage/Current",       0x402,       200,  mc_bus_signals,             2},
    {"Motor Speed",                     0x403,       200,  mc_speed_signals,           2},
    {"Motor Temperature",               0x40B,       1000, mc_temp_signals,            2},
    {"Motor Current/Velocity Control",  0x501,       100,  motor_control_signals,      2},
    {"Bus Current Control",             0x502,       100,  bus_control_signals,        2},
    {"Peripheral Control",              0x600,       0,    distro_control_signals,     1},
    {"BMS Temperature/Current/Power",   0x700,       80,   bms_power_signals,          6},
    {"BMS Pack Voltage and Energy",     0x701,       80,   bms_voltage_signals,        4},
    {"BMS Cell Data",                   0x702,       80,   bms_cell_signals,           6},
    {"BMS Thermistor Broadcast",        0x703,       0,    bms_thermistor_signals,     1},
    {"BMS Cell Data Broadcast",         0x704,       20,   bms_cell_broadcast_signals, 5},
    {"Charger 1 Communication",         0x1806E5F4,  808,  charger_signals,            3},
    {"Charger 2 Communication",         0x1806E7F4,  808,  charger_signals,            3},
    {"Charger 3 Communication",         0x1806E9F4,  808,  charger_signals,            3},
};
