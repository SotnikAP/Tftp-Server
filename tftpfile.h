#pragma once

#include <string>
#include <memory>
#include <fstream>

#include <boost/filesystem.hpp>

#include "globals.h"


/// @brief ����� ����� TFTP
///        ��������� ���� �� ������/������
///        �������� ����� ������ ������� 512
///        ������������ �������� � ������ ������ �� �������
class TftpFile
{
public:
     TftpFile();
     TftpFile( const std::string& filename, tftp::readOrWrite operation );

     /// @breif �������� ���� ������ �� ����� � buff
     void readBlockFromFile( char* buff, int blockNumber ) const;

     /// @brief ������� ���� ������ �� buff � ����
     void writeBlockInFile( char* buff, int blockNumber );

     /// @brief ������� 512-������� ������ ����� ��������� ��� ����������
     int getNumberOfBlocks() const;

private:
     /// @brief �������� ������������� ����� � ��������
     bool isFileExists( const std::string& filename ) const;

     /// @brief ��������� ���� �� ������
     bool openRead( const std::string& filename );

     /// @brief ��������� ���� �� ������
     bool openWrite( const std::string& filename );
     
     /// @brief ���������� � numOfBlocks_ ����� 512-������� ������ + 1
     void countBlocks();

private:
     std::shared_ptr<std::ofstream> writeFile_;
     std::shared_ptr<std::ifstream> readFile_;
     std::string filename_;
     size_t numOfBlocks_;
     tftp::readOrWrite operation_;

};