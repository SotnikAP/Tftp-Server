#include <boost/asio.hpp>
#include "tftpserver.h"

using namespace boost::asio;

int main()
{
    io_service service;
    tftp_server::TftpServer server( service );
}