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

        if (command == "LOGIN")
        {
            string username;
            string password;

            cout << "Username: ";
            getline(cin, username);
            cout << "Password: ";
            getline(cin, password);

            string loginCommand = "LOGIN\n" + username + "\n" + password + "\n";
            send(clientSocket, loginCommand.c_str(), loginCommand.length(), 0);

            string response = readLineFromSocket(clientSocket);

            if(response == "OK")
            {
                cout << "Login successful." << endl << endl;
            }
            else if(response == "ERR")
            {
                cerr << "Login failed." << endl << endl;
            }
            else
            {
                cerr << "Unexpected response from server: " << response << endl << endl;
            }
        }

        if (command == "SEND")
        {
            // Sender, Empfänger, Betreff, Nachricht
            string sender;
            string receiver;
            string subject;
            string messageBody;
            string line;

            cout << "Sender: ";
            getline(cin, sender);

            if (sender.length() > 8 || !isValidUsername(sender))
            {
                if (sender.length() > 8)
                {
                    cerr << "Sender length exceeds 8 characters." << endl << endl;
                }
                else
                {
                    cerr << "Sender contains invalid characters. Only (a-z, 0-9) are allowed." << endl << endl;
                }
                continue;
            }
            cout << "Receiver: ";
            getline(cin, receiver);

            if (receiver.length() > 8 || !isValidUsername(receiver))
            {
                if (receiver.length() > 8)
                {
                    cerr << "Receiver length exceeds 8 characters." << endl << endl;
                }
                else
                {
                    cerr << "Receiver contains invalid characters. Only (a-z, 0-9) are allowed." << endl << endl;
                }
                continue;
            }

            cout << "Subject (max. 80 chars): ";
            getline(cin, subject);

            if (subject.length() > 80)
            {
                cerr << "Subject length exceeds 80 characters." << endl << endl;
                continue;
            }

            cout << "Message (end with a single '.'): " << endl;

            // Einlesen der Nachricht
            while (true)
            {
                getline(cin, line);

                if (line == ".")
                {
                    break;
                }
                messageBody += line + "\n";
            }
            // Gesamte Nachricht
            string fullMessage = "SEND\n" + sender + "\n" + receiver + "\n" + subject + "\n" + messageBody + ".\n";
            // Sendet der Nachricht an den Server
            send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);

            string response = readLineFromSocket(clientSocket);

            if (response == "OK")
            {
                cout << "Message sent successfully." << endl << endl;
            }
            else if (response == "ERR")
            {
                cerr << "Error sending the message." << endl << endl;
            }
            else
            {
                cerr << "Unexpected response from the server: " << response << endl << endl;
            }
        }
        else if (command == "LIST")
        {
            // Aufzulistender Benutzer
            string username;
            cout << "Username: ";
            getline(cin, username);

            if (username.length() > 8 || !isValidUsername(username))
            {
                if (username.length() > 8)
                {
                    cerr << "Receiver length exceeds 8 characters." << endl << endl;
                }
                else
                {
                    cerr << "Receiver contains invalid characters. Only (a-z, 0-9) are allowed." << endl << endl;
                }
                continue;
            }

            // Command + Username
            string fullCommand = "LIST\n" + username + "\n";
            // Sendet den Befehl an den Server
            send(clientSocket, fullCommand.c_str(), fullCommand.length(), 0);

            // Empfange die Anzahl der Nachrichten
            string countStr = readLineFromSocket(clientSocket);

            if (!countStr.empty() && isdigit(countStr[0]))
            {
                // String -> Integer
                int count = stoi(countStr);

                // Empfange und drucke die Betreffzeilen
                for (int i = 0; i < count; i++)
                {
                    string subject = readLineFromSocket(clientSocket);
                    cout << i + 1 << ". " << subject << endl;
                }
                cout << endl;
            }
            else
            {
                cerr << "Invalid value for countStr: '" << countStr << "'" << endl << endl;
            }
        }
        else if (command == "READ")
        {
            // Nachrichtenempfänger
            string username;
            // Nachrichtnummer
            int messageNumber;

            // Einlesen des Benutzernamens
            cout << "Username: ";
            getline(cin, username);

            if (username.length() > 8 || !isValidUsername(username))
            {
                if (username.length() > 8)
                {
                    cerr << "Receiver length exceeds 8 characters." << endl << endl;
                }
                else
                {
                    cerr << "Receiver contains invalid characters. Only (a-z, 0-9) are allowed." << endl << endl;
                }
                continue;
            }

            // Einlesen der Nachrichtennummer
            cout << "Message Number: ";
            cin >> messageNumber;
            // Ignoriere den verbleibenden Zeilenumbruch
            cin.ignore();

            // Sende Befehl, Benutzername und Nachrichtennummer an den Server
            // message number -1 um das Indexieren zu erleichtern
            string fullCommand = "READ\n" + username + "\n" + to_string(messageNumber-1) + "\n";
            send(clientSocket, fullCommand.c_str(), fullCommand.length(), 0);

            // Empfange die Antwort des Servers
            string response = readLineFromSocket(clientSocket);

            if (response == "OK")
            {
                // Empfange die Nachricht
                string sender = readLineFromSocket(clientSocket);
                string subject = readLineFromSocket(clientSocket);
                string messageContent = readMessageFromSocket(clientSocket);

                // Enferne den letzten Punkt aus der Nachricht
                string dot = readLineFromSocket(clientSocket);

                cout << "----------------------------------------------------------------------" << endl;
                // Gib die Nachricht aus
                cout << "From: " << sender << endl;
                cout << "Subject: " << subject << endl;
                cout  << "Message: " << endl << messageContent << endl;
            }
            else if (response == "ERR")
            {
                cerr << "Message does not exist" << endl << endl;
            }
            else
            {
                cerr << "Unexpected response from server: " << response << endl << endl;
            }
        }
        else if (command == "DEL")
        {
            // Nachrichtenempfänger und Nachrichtennummer
            string username;
            int messageNumber;

            cout << "Username: ";
            getline(cin, username);

            if (username.length() > 8 || !isValidUsername(username))
            {
                if (username.length() > 8)
                {
                    cerr << "Receiver length exceeds 8 characters." << endl << endl;
                }
                else
                {
                    cerr << "Receiver contains invalid characters. Only (a-z, 0-9) are allowed." << endl << endl;
                }
                continue;
            }

            cout << "Message Number: ";
            cin >> messageNumber;
            cin.ignore();

            // Sende Befehl, Benutzername und Nachrichtennummer an den Server
            string fullCommand = "DEL\n" + username + "\n" + to_string(messageNumber) + "\n";
            send(clientSocket, fullCommand.c_str(), fullCommand.length(), 0);

            // Empfange die Antwort des Servers
            string response = readLineFromSocket(clientSocket);

            if (response == "OK")
            {
                cout << "Message successfully deleted." << endl << endl;
            }
            else if (response == "ERR")
            {
                cerr << "Message could not be deleted." << endl << endl;
            }
            else
            {
                cerr << "Unexpected response from server: " << response << endl << endl;
            }
        }
        else if (command == "QUIT")
        {
            // Sendet den "QUIT"-Befehl an den Server (5: Länge des Befehls, 0: keine Flags)
            send(clientSocket, "QUIT\n", 5, 0);
            break;
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