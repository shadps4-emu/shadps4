#ifdef _MSC_VER
#define _SCL_SECURE_NO_WARNINGS
#define ELFIO_NO_INTTYPES
#endif

#include <cstdio>
#include <iostream>
#include <string>

#include <elfio/elfio.hpp>
using namespace ELFIO;

namespace Core::Loader {

class Elf {
public:
    int Open(const std::string& file_name);

private:
    elfio m_reader;
};
} // namespace Core::Loader