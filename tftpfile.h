#pragma once

#include <string>
#include <memory>
#include <fstream>

#include <boost/filesystem.hpp>

#include "globals.h"


/// @brief Класс файла TFTP
///        Открывает файл на чтение/запись
///        Содержит число блоков размера 512
///        Осуществляет передачу и запись файлов по пакетам
class TftpFile
{
public:
     TftpFile();
     TftpFile( const std::string& filename, tftp::readOrWrite operation );

     /// @breif Записать блок данных из файла в buff
     void readBlockFromFile( char* buff, int blockNumber ) const;

     /// @brief Считать блок данных из buff в файл
     void writeBlockInFile( char* buff, int blockNumber );

     /// @brief Сколько 512-байтных блоков нужно отправить для завершения
     int getNumberOfBlocks() const;

private:
     /// @brief Проверка существования файла в каталоге
     bool isFileExists( const std::string& filename ) const;

     /// @brief Открывает файл на чтение
     bool openRead( const std::string& filename );

     /// @brief Открывает файл на запись
     bool openWrite( const std::string& filename );
     
     /// @brief Записывает в numOfBlocks_ число 512-байтных блоков + 1
     void countBlocks();

private:
     std::shared_ptr<std::ofstream> writeFile_;
     std::shared_ptr<std::ifstream> readFile_;
     std::string filename_;
     size_t numOfBlocks_;
     tftp::readOrWrite operation_;

};