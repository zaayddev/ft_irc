#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Ircserv.hpp"

class User {
    public:
        User();
        User(std::string str);

    private:
        int     _fd;
};

/*<------> Alternate names for our data types <------>*/
typedef std::pair<pollfd, User>     client;

#endif