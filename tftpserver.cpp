#include <boost/asio.hpp>
#include "tftpserver.h"

using namespace boost::asio;
using namespace tftp_server;

TftpServer::TftpServer( boost::asio::io_service& service ) : service_( service ), 
     socket_( service, udp::endpoint( udp::v4(), tftp::tftpPort ) ), currentNumOfBlock_(1)
{
     try
     {
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
          Sleep( 1000 );
          socket_.receive_from( buffer( buffer_, tftp::packageSize ), remote_endpoint_ );
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
               operationInProcess_ = tftp::read;
               process_RrqPackage();
     }
     else if ( currentOperation == tftp::WRQ )
     {
               operationInProcess_ = tftp::write;
               process_WrqPackage();
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
     clearBuff();
     send_WRQconfirmation();
}

void TftpServer::send_confirmation()
{
     std::cout << "Sending confirmation\n";

     operationCode dataPackage = ntohs( static_cast<int>( tftp::confirmation ) );
     memmove( buffer_, &dataPackage, sizeof( operationCode ) );
     currentNumOfBlock_ = ntohs( currentNumOfBlock_ );
     memmove( buffer_ + sizeof( operationCode ), &currentNumOfBlock_, sizeof( unsigned short ) );

     socket_.send_to( buffer( buffer_, tftp::packageSize ), remote_endpoint_ );
     clearBuff();
}

void TftpServer::send_WRQconfirmation()
{
     std::cout << "Sending confirmation\n";

     operationCode dataPackage = ntohs( static_cast<int>( tftp::confirmation ) );
     memmove( buffer_, &dataPackage, sizeof( operationCode ) );
     currentNumOfBlock_ = ntohs( 0 );
     memmove( buffer_ + sizeof( operationCode ), &currentNumOfBlock_, sizeof( unsigned short ) );

     socket_.send_to( buffer( buffer_, tftp::packageSize ), remote_endpoint_ );
     clearBuff();
}

void TftpServer::process_dataPackage()
{
     std::cout << "Processing data package\n";

     char buf[tftp::dataSize];
     memmove( &currentNumOfBlock_, buffer_ + sizeof( operationCode ), sizeof( unsigned short ) );
     currentNumOfBlock_ = ntohs( currentNumOfBlock_ );

     std::cout << "Block #" << currentNumOfBlock_ << std::endl;

     memmove( buf, buffer_ + sizeof( operationCode ) + sizeof( unsigned short ), tftp::dataSize );
     currentFile_.writeBlockInFile( buf, currentNumOfBlock_ );
     clearBuff();
     send_confirmation();
}

void TftpServer::process_confirmationPackage()
{
     std::cout << "Processing confirmation\n";

     unsigned short numOfBlock;
     memmove( &numOfBlock, buffer_ + sizeof( operationCode ), sizeof( unsigned short ) );
     numOfBlock = ntohs( numOfBlock );

     std::cout << "Block #" << currentNumOfBlock_ << "confirmed. ++blockNum \n";

     if ( numOfBlock == currentNumOfBlock_ )
     {
          if ( ( currentNumOfBlock_ < currentFile_.getNumberOfBlocks() ) )
          {
               currentNumOfBlock_++;
               send_dataPackage();
          }
          else
          {
               std::cout << "Last block send\n";
               return;
          }
     }
     else
     {
          send_dataPackage();
     }
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
     std::cout << "Sending data package #" << currentNumOfBlock_ << std::endl;

     operationCode dataPackage = ntohs( static_cast<int>( tftp::data ) );
     memmove( buffer_, &dataPackage, sizeof( operationCode ) );
     unsigned short currentBlock = ntohs( currentNumOfBlock_ );
     memmove( buffer_ + sizeof( operationCode ), &currentBlock, sizeof( unsigned short ) );
     currentFile_.readBlockFromFile( 
          buffer_ + sizeof( operationCode ) + sizeof( unsigned short ),
          currentNumOfBlock_ );
     socket_.send_to( buffer( buffer_, tftp::packageSize ), remote_endpoint_ );
     clearBuff();
}

void TftpServer::clearBuff()
{
     for ( int i = 0; i < 516; ++i )
     {
          buffer_[i] = 0;
     }
}
