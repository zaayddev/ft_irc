#include "../Includes/User.hpp"

std::string User::get_ip(void) const {
    return (_ip);
}



void    User::set_ip(const std::string ip) {
    _ip = ip;
}
void    User::set_fd(int fd) {
    _fd = fd;
}