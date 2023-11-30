#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

// Liest eine Zeile aus dem Socket
string readLineFromSocket(int socket)
{
    // Nachricht
    string result;
    // Einzelnes Zeichen
    char buffer;

    while (true)
    {
        // Empfange ein Zeichen
        int bytesRead = recv(socket, &buffer, 1, 0);

        if (bytesRead <= 0)
        {
            cerr << "Fehler beim Empfangen von Daten oder Verbindung geschlossen." << endl;
            break;
        }
        // Bei einem Zeilenumbruch wird das einlesen beendet
        if (buffer == '\n')
        {
            break;
        }
        // Füge das Zeichen der Nachricht hinzu
        result += buffer;
    }
    // Zurückgeben der Nachricht
    return result;
}

string readMessageFromSocket(int socket)
{
    // Nachricht
    string result;
    // Einzelnes Zeichen
    char buffer;
    // Flag für den letzten Punkt
    bool lastCharWasDot = false;

    while (true)
    {
        // Empfange ein Zeichen
        int bytesRead = recv(socket, &buffer, 1, 0);

        if (bytesRead <= 0)
        {
            cerr << "Fehler beim Empfangen von Daten oder Verbindung geschlossen." << endl;
            break;
        }

        // Beim Empfangen eines Punktes wird überprüft, ob der letzte Punkt der Nachricht war
        if (buffer == '.' && lastCharWasDot)
        {
            break;
        }

        // Setze das Flag für den letzten Punkt
        if (buffer == '\n')
        {
            lastCharWasDot = true;
        }
        else
        {
            lastCharWasDot = false;
        }
        // Füge das Zeichen der Nachricht hinzu
        result += buffer;
    }
    // Zurückgeben der Nachricht
    return result;
}

// Überprüft ob der String nur aus (a-z) und (0-9) besteht
bool isValidUsername(const string str)
{
    for (int i = 0; i < str.length(); ++i)
    {
        char c = str[i];

        if (!(c >= 'a' && c <= 'z') && !(c >= '0' && c <= '9'))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: ./twmailer-client <ip> <port>" << endl;
        return 1;
    }

    // 1. Argument: IP
    string ip = argv[1];
    // 2. Argument: Port (stoi: string -> int)
    int port = stoi(argv[2]);

    // Neuer Socket (AF_INET: IPv4, SOCK_STREAM: TCP, Protocol: Standardprotokoll)
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Speicher die IP und den Port des Servers
    sockaddr_in serverAddr;
    // Adresstyp: IPv4
    serverAddr.sin_family = AF_INET;
    // Port -> Netzwerk-Byte-Format
    serverAddr.sin_port = htons(port);
    // IP -> Netzwerk-Byte-Format
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    // Client verbindet sich mit dem Server
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    // Command
    string command;

    // Bis das Programm beendet wird
    while (true)
    {
        cout << "> ";
        getline(cin, command);

        if (command == "SEND")
        {
            // ...
        }
        else if (command == "LIST")
        {
            // ...
        }
        else if (command == "READ")
        {
            // ...
        }
        else if (command == "DEL")
        {
            // ...
        }
        else if (command == "QUIT")
        {
            // ...
        }
        else
        {
            cout << "Unknown command." << endl << endl;
        }
    }
    // Schließt den Socket
    close(clientSocket);

    return 0;
}