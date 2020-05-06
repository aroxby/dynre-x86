#include <cstdio>
#include <Zydis/Zydis.h>

int main()
{
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
        printf("%p ", runtime_address + offset);
        printf("(%i) ", instruction.length);
        printf("%s ", name);

        for(int i = 0; i < instruction.operand_count; i++) {
            auto operand = instruction.operands[i];

            switch (operand.type)
            {
            case ZYDIS_OPERAND_TYPE_REGISTER:
                printf("R");
                break;
            case ZYDIS_OPERAND_TYPE_MEMORY:
                printf("M");
                break;
            case ZYDIS_OPERAND_TYPE_POINTER:
                printf("P");
                break;
            case ZYDIS_OPERAND_TYPE_IMMEDIATE:
                printf("I");
                break;
            default:
                printf("?");
            }
        }

        printf("%c\n", instruction.meta.branch_type ? '^' : '|');

        offset += instruction.length;
    }
}
