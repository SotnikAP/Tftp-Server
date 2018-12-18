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

     /// @brief ����������� ���� ������ � ���������� ���������
     /// @exceptions std::runtime_error - ����� �� �������
     ///                                - ������ RRQ/WRQ ����� �� ��������� ��������� �����������
     void process_package();

     /// @brief ��������� RRQ �������
     void process_RrqPackage();

     /// @brief ��������� WRQ �������
     void process_WrqPackage();

     /// @brief ��������� ������ � �������
     void process_dataPackage();

     /// @brief ��������� ������-�������������
     void process_confirmationPackage();

     void send_WRQconfirmation();

     /// @brief ��������� ������ � �������
     void process_errorPackage();

     /// @brief �������� ������ � �������
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

