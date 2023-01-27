#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Ircserv.hpp"

class User {
    public:
        User();
        User(std::string str);

        std::string get_ip(void) const;
        int         get_fd(void) const;

        void        set_ip(const std::string ip);
        void        set_fd(int fd);

    private:
        std::string     _ip;
        std::string     _nick;
        std::string     _user;
        int             _fd;
        bool            _auth;
};

/*<------> Alternate names for our data types <------>*/
typedef std::pair<pollfd, User>     client;

#endif