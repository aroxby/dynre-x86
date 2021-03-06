#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <Zydis/Zydis.h>
#include "register_names.h"
using namespace std;

void dumpBytes(const ZyanU8 *data, int len) {
    cout << setfill('0') << hex;
    for(int i = 0; i < len; i++) {
        cout << setw(2) << int(data[i]);
    }
    cout << ' ';
}

template <typename HexInteger>
string signedHex(HexInteger value) {
    stringstream stream;
    if(value < 0) {
        stream << '-';
        value = -value;
    }
    stream << setfill('0') << hex << value;
    string out = stream.str();
    return out;
}

int main() {
    ZyanU8 data[] =
    {
        0x51, 0x8D, 0x45, 0xFF, 0x50, 0xFF, 0x75, 0x0C, 0xFF, 0x75,
        0x08, 0xFF, 0x15, 0xA0, 0xA5, 0x48, 0x76, 0x85, 0xC0, 0x0F,
        0x88, 0xFC, 0xDA, 0x02, 0x00,
    };

    // Initialize decoder context
    ZydisDecoder decoder;
    ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64);

    // Initialize formatter. Only required when you actually plan to do instruction
    // formatting ("disassembling"), like we do here
    ZydisFormatter formatter;
    ZydisFormatterInit(&formatter, ZYDIS_FORMATTER_STYLE_INTEL);

    // Loop over the instructions in our buffer.
    // The runtime-address (instruction pointer) is chosen arbitrary here in order to better
    // visualize relative addressing
    ZyanU64 runtime_address = 0x007FFFFFFF400000;
    ZyanUSize offset = 0;
    const ZyanUSize length = sizeof(data);
    ZydisDecodedInstruction instruction;
    while (ZYAN_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, data + offset, length - offset,
        &instruction)))
    {
        // Print current instruction pointer.
        printf("%p ", runtime_address + offset);

        // Format & print the binary instruction structure to human readable format
        char buffer[256];
        ZydisFormatterFormatInstruction(&formatter, &instruction, buffer, sizeof(buffer),
            runtime_address + offset);
        puts(buffer);


        const char *name = ZydisMnemonicGetString(instruction.mnemonic);
        cout << setw(16) << setfill('0') << hex << runtime_address + offset << ' ';
        dumpBytes(data + offset, instruction.length);
        cout << name << ' ';

        // TODO: Rewrite output to support negative hex values
        for(int i = 0; i < instruction.operand_count; i++) {
            auto operand = instruction.operands[i];
            if (operand.visibility == ZYDIS_OPERAND_VISIBILITY_HIDDEN) {
                continue;
            }

            switch (operand.type)
            {
            case ZYDIS_OPERAND_TYPE_REGISTER:
                cout << zydis_register_names[operand.reg.value] << ' ';
                break;
            case ZYDIS_OPERAND_TYPE_MEMORY:
            {
                // Zydis shows some registers that are never explicitly used
                ZydisRegisterClass base_cls = ZydisRegisterGetClass(operand.mem.base);
                if(base_cls == ZYDIS_REGCLASS_IP) {
                    operand.mem.base = ZYDIS_REGISTER_NONE;
                }

                const char *segment = zydis_register_names[operand.mem.segment];
                const char *base = zydis_register_names[operand.mem.base];
                const char *index = zydis_register_names[operand.mem.index];
                const int scale = operand.mem.scale;
                const ZyanI64 displacement = operand.mem.disp.value;

                if(!(instruction.attributes & ZYDIS_ATTRIB_HAS_SEGMENT)) {
                    segment = "";
                }

                cout << '[' << segment << ':' << base
                    << " + " << index << ':' << signedHex(displacement)
                    << " * " << scale + 1 << "] ";
            }
                break;
            case ZYDIS_OPERAND_TYPE_POINTER:
                printf("[%x:%x] ", operand.ptr.segment, operand.ptr.offset);
                cout << '[' << setw(16) << setfill('0') << hex << operand.ptr.segment;
                cout << operand.ptr.offset << "] ";
                break;
            case ZYDIS_OPERAND_TYPE_IMMEDIATE:
                cout << setw(16) << setfill('0') << hex << operand.imm.value.s << ' ';
                break;
            default:
                cout << "? ";
            }
        }

        cout << (instruction.meta.branch_type ? '^' : '|') << endl;

        offset += instruction.length;
    }
}
