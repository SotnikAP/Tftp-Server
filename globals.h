#pragma once

using operationCode = unsigned short;

namespace tftp
{
enum package { dataSize = 512 };
enum { packageSize = 516 };
enum file { maxFileSize = 4096 };
enum readOrWrite { nothing, read, write };
enum { tftpPort = 69 };
     enum
     {
          RRQ = 1,
          WRQ,
          data,
          confirmation,
          error
     };
}