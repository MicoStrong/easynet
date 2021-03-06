#ifndef SOCKET_FAMILY_HPP
#define SOCKET_FAMILY_HPP

#include <mutex>
#include <cstring>
#include <utility>
#include "socket_options.hpp"

extern "C" {
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/un.h>
//#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
}

namespace net {
    using AddrIPv4  = sockaddr_in;
    using AddrIPv6  = sockaddr_in6;
    using AddrUnix  = sockaddr_un;
    using AddrStore = sockaddr_storage;

    enum class Domain {
        UNIX      = AF_UNIX,
        LOCAL     = AF_UNIX,
        IPv4      = AF_INET,
        IPv6      = AF_INET6,
        IPX       = AF_IPX,
        NETLINK   = AF_NETLINK,
        X25       = AF_X25,
        AX25      = AF_AX25,
        ATMPVC    = AF_ATMPVC,
        APPLETALK = AF_APPLETALK,
        PACKET    = AF_PACKET,
        ALG       = AF_ALG
    };


    enum class Type {
        TCP       = SOCK_STREAM,
        UDP       = SOCK_DGRAM,
        SEQPACKET = SOCK_SEQPACKET,
        RAW       = SOCK_RAW,
        RDM       = SOCK_RDM,
        NONBLOCK  = SOCK_NONBLOCK,
        CLOEXEC   = SOCK_CLOEXEC
    };

    enum class Shut { READ = SHUT_RD, WRITE = SHUT_WR, READWRITE = SHUT_RDWR };


    enum class Recv {
        NONE    = 0,
        PEEK    = MSG_PEEK,
        OOB     = MSG_OOB,
        WAITALL = MSG_WAITALL
    };
    inline constexpr Recv operator|(Recv a, Recv b) noexcept
    {
        return static_cast<Recv>(static_cast<int>(a) | static_cast<int>(b));
    }

    enum class Send {
        NONE     = 0,
        EOR      = MSG_EOR,
        OOB      = MSG_OOB,
        NOSIGNAL = MSG_NOSIGNAL
    };
    inline constexpr Send operator|(Send a, Send b) noexcept
    {
        return static_cast<Send>(static_cast<int>(a) | static_cast<int>(b));
    }


    namespace methods {

        /**
        * @function getErrorMsg
        * @desc Returns the standard human readable error message corresponding to
        * given errorNumber.
        *
        * @param {int} errorNumber Error number whose string to return.
        * @returns {string} Standard error string corresponding to given
        * errorNumber.
        */
        inline std::string getErrorMsg(const int errorNumber)
        {
            static std::mutex m;
            std::lock_guard<std::mutex> lock(m);
            const char *errMsg = strerror(errorNumber);
            std::string returnString(errMsg);
            return returnString;
        }

        /**
        * @function construct
        * @desc Fills the given AddrIPv4 structure object with given ip address and
        * port.
        *
        * @param {AddrIPv4} _addrStruct Ipv4 structure object that needs to be
        * filled with given ip address and port.
        * @param {char []} _addr Ip address which needs to be filled in the AddrIPv4
        * structure object.
        * @param {int} _port Port number which needs to be filled in the AddrIPv4
        * structure object.
        * @returns {int} 1 if sucessful, 0 if given ip address does not represent a
        * valid ip address, -1 if some error occurred.
        */
        inline int construct(AddrIPv4 &_addrStruct, const char _addr[],
                             const int _port) noexcept
        {
            if (_port < 0 || _port > 65535) {
                return 0;
            }

            std::memset(&_addrStruct, 0, sizeof(_addrStruct));
            _addrStruct.sin_family = AF_INET;
            _addrStruct.sin_port   = htons(_port);

            return inet_pton(AF_INET, _addr, &_addrStruct.sin_addr);
        }


        /**
        * @function construct
        * @desc Fills the given AddrIPv6 structure object with given ip address and
        * port.
        *
        * @param {AddrIPv6} _addrStruct - Ipv6 structure object that needs to be
        * filled with given ip address and port.
        * @param {char []} _addr Ip address which needs to be filled in the AddrIPv6
        * structure object.
        * @param {int} _port Port number which needs to be filled in the AddrIPv6
        * structure object.
        * @returns {int} 1 if sucessful, 0 if given ip address does not represent a
        * valid ip address, -1 if some error occurred.
        */
        inline int construct(AddrIPv6 &_addrStruct, const char _addr[],
                             const int _port) noexcept
        {
            // TODO: replace code with call to getaddrinfo()
            if (_port < 0 || _port > 65535) {
                return 0;
            }

            std::memset(&_addrStruct, 0, sizeof(_addrStruct));
            _addrStruct.sin6_family = AF_INET6;
            _addrStruct.sin6_port   = htons(_port);

            return inet_pton(AF_INET6, _addr, &_addrStruct.sin6_addr);
        }


        /**
        * @function construct
        * @desc Fills the given AddrUnix structure object with given address.
        *
        * @param {AddrUnix} _addrStruct structure object that needs to be filled
        * with given path.
        * @param {char []} _addr Path which needs to be filled in the AddrUnix
        * structure object.
        * @returns {int} Always returns 1.
        */
        inline int construct(AddrUnix &_addrStruct, const char _addr[]) noexcept
        {
            std::memset(&_addrStruct, 0, sizeof(_addrStruct));
            _addrStruct.sun_family = AF_UNIX;
            std::strncpy(_addrStruct.sun_path, _addr, 108);
            return 1;
        }
    }
}

#endif
