#include "artemis_canid.hpp"

template<typename Byte>
void reverse_buffer(Byte* start, Byte* end) {
    Byte temp;
    while (start < end) {
        temp = *end;
        *end = *start;
        *start = temp;

        start++;
        end--;
    }
}

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
            reverse_buffer(buffer + elem_start, buffer + elem_end);
        }
    }
}

