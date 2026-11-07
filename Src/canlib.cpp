#include "canlib.h"

canlib::canlib() : baud(500000)
{
}

canlib::canlib(int baud) : baud(baud)
{
}