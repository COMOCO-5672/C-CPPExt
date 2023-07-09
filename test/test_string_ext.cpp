#include "../common/string_ext.h"
#include <iostream>

int main()
{
    std::string trim_str = " 1111111 ";

    auto trim_start_str = string_ext::trim_start(trim_str);
    std::cout << "trim_start:" << trim_start_str << "end\n";

    auto trim_end_str = string_ext::trim_end(trim_str);
    std::cout << "trim_end_str:" << trim_end_str << "end\n";

    auto trimed_str = string_ext::trim(trim_str);
    std::cout << "trimed_str:" << trimed_str << "end\n";

    return 0;
}
