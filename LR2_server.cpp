#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <map>

#define MAX_BUFFER_SIZE 64

int main()
{
    //zapros imeni kanala i sozdanie duplex jrientir na message kanal 
    std::string pipe_name;
    std::cout << "Enter pipe name: ";
    std::cin >> pipe_name;
    std::string full_pipe_name = "\\\\.\\pipe\\" + pipe_name;
    HANDLE _hPipe = CreateNamedPipe(full_pipe_name.c_str(), //name kanala
                                    PIPE_ACCESS_DUPLEX, // razresh dostup na reaad and write
                                    PIPE_TYPE_MESSAGE | 
									PIPE_READMODE_MESSAGE,
                                    PIPE_UNLIMITED_INSTANCES, //chislo exempl kanalov ne ogranicheno
                                    MAX_BUFFER_SIZE,
                                    MAX_BUFFER_SIZE,
                                    0, //time out ozhidaniya
                                    NULL); //atribut bezopasnosti
  if (_hPipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "Error:"<< GetLastError()<<'\n';
    }

    //waiting for connection and zaprosi clienta
    std::string command (MAX_BUFFER_SIZE, '\0');
    std::string keyword,key,value,response {};
    DWORD bytes;
    std::map<std::string, std::string> dict {};
    while (true)
    {
        std::cout << "Waiting for client...\n";
        if (!ConnectNamedPipe(_hPipe, NULL))
        {
           std::cout << "Error:"<< GetLastError()<<'\n';;
           CloseHandle(_hPipe);
        }
        else
        {
           std::cout << "connection succeeded"<<'\n';;
        }
       while(true)
       {

            std::cout << "Waiting for command...\n";
            auto frsuccess = ReadFile(_hPipe, &command[0], command.size(), &bytes, NULL);
            if (!frsuccess)
            {
                std::cout<<"Error:"<<GetLastError();
            }
            command.resize(command.find('\0'));

            std::istringstream parser {command};
            parser >> std::ws >> cmnd;
            if (cmnd == "set") //sohranenie po key
                {
                    parser >> key >> value;
                    dict[key] = value;
                    response = "acknowledged";
                }
            else if (cmnd == "get") //poluchenie znach po key
                {
                    parser >> key;
                    if (dict.find(key) != dict.end())
                        response = "found " + dict[key];
                    else
                        response = "missing";
                }
            else if (cmnd == "list") //list of keys
                {
                    for (auto i = dict.begin(); i != dict.end(); ++ i)
                        response += i->first + " ";
                }
            else if (cmnd == "delete") //delet znach po key
                {
                    parser >> key;
                    auto del = dict.find(key);
                    if (del != dict.end())
                    {
                        dict.erase(del);
                        response = "deleted";
                    }
                    else
                        response = "missing";
                }
            else if (cmnd == "quit") //end seans
            {
                DisconnectNamedPipe(pipe_y);
                break;
            }
             WriteFile(_hPipe, response.c_str(), response.size(), &bytes, NULL);
    }

         //sleduet li ostanovit
         char next;
         bool flag = true;
         std::cout << "Do you want to stop server?(y,n)\n"; //y-yes, n-no
         while (std::cin>>next)
         {
            if (next == 'y')
            {
                CloseHandle(_hPipe);
                flag = false;
                break;
            }
            else if (next == 'n')
                break;
            else
            {
                std::cout << "(y/n): ";
                continue;
            }
        }
        if (!flag)
            break;


    }

}
