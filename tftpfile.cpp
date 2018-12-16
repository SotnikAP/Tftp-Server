#include "tftpfile.h"

#include <exception>

void TftpFile::readBlockFromFile( char* buff, int blockNumber ) const
{
     readFile_->read( buff, tftp::package::dataSize );
}

void TftpFile::writeBlockInFile( char* buff, int blockNumber )
{
     writeFile_->write( buff, tftp::package::dataSize );
}



int TftpFile::getNumberOfBlocks() const
{
     return numOfBlocks_;
}


TftpFile::TftpFile()
{

}


TftpFile::TftpFile( const std::string& filename, tftp::readOrWrite operation ) : operation_( operation )
{
     bool openSucced = false;
     operation_ == tftp::readOrWrite::read ?
          openSucced = openRead( filename ) :
          openSucced = openWrite( filename );

     if ( !openSucced )
     {
          throw std::runtime_error( "Can't open the file" );
     }



}

bool TftpFile::openRead( const std::string& filename )
{
     if ( isFileExists( filename ) )
     {
          readFile_ = std::make_shared<std::ifstream>( filename, std::ifstream::binary );
          countBlocks();
          return 1;
     }
     return 0;
}

bool TftpFile::openWrite( const std::string& filename )
{
     writeFile_ = std::make_shared<std::ofstream>( filename, std::ofstream::binary );

     if ( writeFile_->is_open() )
     {
          return 1;
     }

     return 0;
}

bool TftpFile::isFileExists( const std::string& filename ) const
{
     boost::filesystem::path file = boost::filesystem::current_path() / filename;
     return exists( file );
}

void TftpFile::countBlocks()
{
    /* const std::streampos cur_pos = readFile_->tellg();

     readFile_->seekg( 0, readFile_->end );
     int length = readFile_->tellg();

     readFile_->seekg( cur_pos, readFile_->cur );

     numOfBlocks_ = length / tftp::package::dataSize;*/

     readFile_->seekg( 0, readFile_->end );
     int lenght = readFile_->tellg();
     readFile_->seekg( 0, readFile_->beg );

     numOfBlocks_ = lenght / tftp::dataSize;
}
