#ifndef OW_COMPRESSIONUTILS_HPP
#define OW_COMPRESSIONUTILS_HPP

#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#include <zlib.h>

namespace ow
{

class CompressionUtils
{
    public:
        static bool base64_encode(std::string& data);
        static bool base64_decode(std::string& data);
        static bool decompressString(std::string& data);
        static bool compressString(std::string& data);

    private:
        static const std::string base64_table;

        static inline bool is_valid_base64(unsigned char c)
        {
            return (isalnum(c) || (c == '+') || (c == '/'));
        }
};

} // ow

#endif // OW_COMPRESSIONUTILS_HPP
