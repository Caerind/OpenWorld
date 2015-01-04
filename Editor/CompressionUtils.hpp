#ifndef OWE_COMPRESSIONUTILS_HPP
#define OWE_COMPRESSIONUTILS_HPP

#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#include <zlib.h>

namespace owe
{

class CompressionUtils
{
    public:
        static void uncompressLine(std::string& line);
        static std::string compressLine(std::string const& line);

    private:
        static bool base64_encode(std::string& data);
        static bool base64_decode(std::string& data);
        static bool decompressString(std::string& data);
        static bool compressString(std::string& data);

        static const std::string base64_table;

        static inline bool is_valid_base64(unsigned char c)
        {
            return (isalnum(c) || (c == '+') || (c == '/'));
        }
};

} // owe

#endif // OWE_COMPRESSIONUTILS_HPP
