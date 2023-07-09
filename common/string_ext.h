#include <string>
#include <vector>

namespace string_ext {

// Delete start space
inline std::string trim_start(const std::string &str)
{
    std::string::size_type pos = str.find_first_not_of(' ');
    // no find
    if (pos == std::string::npos)
        return str;

    return str.substr(pos, str.length() - pos);
}

// Delete end space
inline std::string trim_end(const std::string &str)
{
    std::string::size_type pos = str.find_last_not_of(' ');
    if (pos == std::string::npos)
        return str;

    return str.substr(0, str.length() - pos);
}

// Delete the Spaces at the beginning and end of the string
inline std::string trim(const std::string &str)
{
    std::string::size_type pos = str.find_first_not_of(' ');
    // no find
    if (pos == std::string::npos)
        return str;

    std::string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != std::string::npos)
        return str.substr(pos, pos2 - pos + 1);

    return str.substr(pos);
}

// Split according to special characters
inline int spilt(const std::string &str, std::vector<std::string> &rect, std::string sep = ".")
{
    if (str.empty())
        return 0;

    std::string tmp;
    // find first sep position
    std::string::size_type pos_begin = str.find_first_not_of(sep);
    std::string::size_type common_pos = 0;

    while (pos_begin != std::string::npos) {
        // Gets the next separator location coordinates
        common_pos = str.find(sep, pos_begin);

        if (common_pos != std::string::npos) {
            tmp = str.substr(pos_begin, common_pos - pos_begin);
            pos_begin = common_pos + sep.length();
        } else {
            tmp = str.substr(pos_begin);
            pos_begin = common_pos;
        }

        if (!tmp.empty()) {
            rect.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

} // namespace string_ext
