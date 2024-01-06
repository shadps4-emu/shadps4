#include "elf.h"

int Core::Loader::Elf::Open(const std::string& file_name)
{
    if (!m_reader.load(file_name)) {
        printf("File %s is not found or it is not an ELF file\n", file_name.c_str());
        return 1;
    }
    return 0;
}
