#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string>

#define MAX_BUFFER_SIZE 64

int main()
{   //zapros imeni kanala i podkluch
    std::string pipe_name;
    std::cout<<"Enter pipe name:";
    std::cin >> pipe_name;
    std::string full_pipe_name = "\\\\.\\pipe\\" + pipe_name;

    HANDLE _hPipe = CreateFile(full_pipe_name.c_str(),
                               GENERIC_READ | 
							   GENERIC_WRITE,
                               0,
                               nullptr,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               nullptr);

    if (_hPipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error:"<< GetLastError()<<std::endl;
    }

    
    std::string command (64,'\0');
//zapros stroki komandi
    DWORD write;
    DWORD read;
    
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin >> std::ws, command);
        WriteFile(_hPipe, command.c_str(), command.size(), &write, NULL);
        if (command == "quit") 
        {
          CloseHandle(_hPipe);
          break;
        }
        else
        {
            std::string buffer (MAX_BUFFER_SIZE, '\0');
            ReadFile(_hPipe, &buf[0], buf.size(), &read, NULL);
            std::cout << buffer <<'\n';
        }
        command.replace(0,command.size(),command.size(),'\0');
    }
}
