#include <boost/asio.hpp>
#include "tftpserver.h"

using namespace boost::asio;

void run_server(const std::string& ipAddress)
{
     
}

int main()
{
    io_service service;
    tftp_server::TftpServer server( service );

    system( "pause" );
}