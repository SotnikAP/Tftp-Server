#pragma once
#include "tftpfile.h"
#include <iostream>

namespace tftp_server
{
using namespace boost::asio::ip;
class TftpServer
{
public:
     explicit TftpServer( boost::asio::io_service& service );

     void startProcessing();

     /// @brief Определение типа пакета и дальнейшая обработка
     /// @exceptions std::runtime_error - Пакет не опознан
     ///                                - Пришёл RRQ/WRQ пакет до окончания обработки предыдущего
     void process_package();

     /// @brief Обработка RRQ пакетов
     void process_RrqPackage();

     /// @brief Обработка WRQ пакетов
     void process_WrqPackage();

     /// @brief Обработка пакета с данными
     void process_dataPackage();

     /// @brief Обработка пакета-подтверждения
     void process_confirmationPackage();

     void send_WRQconfirmation();

     /// @brief Обработка пакета с ошибкой
     void process_errorPackage();

     /// @brief Отправка пакета с данными
     void send_dataPackage();

     void send_confirmation();

     void clearBuff();

private:
     boost::asio::io_service& service_;
     udp::socket socket_;
     char buffer_[tftp::packageSize];

     udp::endpoint remote_endpoint_;

     TftpFile currentFile_;
     size_t currentNumOfBlock_ = 0;

     tftp::readOrWrite operationInProcess_ = tftp::nothing;
};
}

