#include <boost/asio.hpp>
#include "tftpserver.h"

using namespace boost::asio;
using namespace tftp_server;

TftpServer::TftpServer( boost::asio::io_service& service ) : service_( service ), socket_( service ), currentNumOfBlock_(0)
{
     try
     {
          socket_.open(ip::udp::v4() );
          socket_.bind(ip::udp::endpoint(ip::udp::v4(), tftp::tftpPort ) );

          startProcessing();
     }
     catch ( std::exception& e )
     {
          socket_.close();

          std::cout << "Exception: " << e.what() << "\n";
     }
}

void TftpServer::startProcessing()
{
     while ( true )
     {
          socket_.receive( buffer( buffer_, tftp::packageSize ) );
          process_package();
     }
}

void TftpServer::process_package()
{
     char operation[sizeof( operationCode )];
     memmove( operation, buffer_, sizeof( operationCode ) );
     operationCode currentOperation = ( operation[0] << 8 ) + operation[1];

     if ( currentOperation == tftp::RRQ )
     {
          /*if ( operationInProcess_ == tftp::nothing )
          {*/
               operationInProcess_ = tftp::read;
               process_RrqPackage();
          //}
          //else
          //{
          //     throw std::runtime_error( "RRQ package while processing another RRQ/WRQ package" );
          //}
     }
     else if ( currentOperation == tftp::WRQ )
     {
          if ( operationInProcess_ == tftp::nothing )
          {
               operationInProcess_ = tftp::write;
               process_WrqPackage();
          }
          else
          {
               throw std::runtime_error( "WRQ package recieved while processing another RRQ/WRQ package" );
          }
     }
     else if ( currentOperation == tftp::data )
     {
          process_dataPackage();
     }
     else if ( currentOperation == tftp::confirmation )
     {
          process_confirmationPackage();
     }
     else if ( currentOperation == tftp::error )
     {
          process_errorPackage();
     }
     else
     {
          throw std::runtime_error( "Package not recognized" );
     }
}

void TftpServer::process_RrqPackage()
{
     operationInProcess_ = tftp::read;
     char filename[tftp::dataSize];
     char mode[tftp::dataSize];
     strcpy_s( filename, buffer_ + sizeof( operationCode ) );
     strcpy_s( mode, buffer_ + sizeof( operationCode ) + strlen( filename ) + 1 );
     currentFile_ = TftpFile( filename, operationInProcess_ );

     send_dataPackage();
}

void TftpServer::process_WrqPackage()
{
     operationInProcess_ = tftp::write;
     char filename[tftp::dataSize];
     char mode[tftp::dataSize];
     strcpy_s( filename, buffer_ + sizeof( operationCode ) );
     strcpy_s( mode, buffer_ + sizeof( operationCode ) + strlen( filename ) + 1 );
     currentFile_ = TftpFile( filename, operationInProcess_ );
}

void TftpServer::process_dataPackage()
{

     operationInProcess_ = tftp::nothing;
}

bool TftpServer::process_confirmationPackage()
{
     currentNumOfBlock_++;
     send_dataPackage();
     return true;
}

void TftpServer::process_errorPackage()
{
     /// @ToDo: ќформить в константу
     Sleep( 2000 );
     //
     operationInProcess_ = tftp::nothing;
}

void TftpServer::send_dataPackage()
{
     Sleep( 2500 );
     operationCode dataPackage = tftp::data;
     memmove( buffer_, &dataPackage, sizeof( operationCode ) );
     memmove( buffer_ + 2, &currentNumOfBlock_, sizeof( size_t ) );
     currentFile_.readBlockFromFile( buffer_ + 4, currentNumOfBlock_ );
     socket_.send_to( buffer( buffer_, tftp::packageSize ), udp::endpoint( ip::udp::v4(), tftp::tftpPort ) );
}
