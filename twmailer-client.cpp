#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
            cerr << endl << "Fehler beim Empfangen von Daten oder Verbindung geschlossen.";
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

// Löscht den Socket-Buffer
void clearSocketBuffer(int clientSocket)
{
    char tempBuffer[1024];
    int bytesRead;
    do
    {
        // Versuche, Daten mit non-blocking call zu lesen
        bytesRead = recv(clientSocket, tempBuffer, sizeof(tempBuffer), MSG_DONTWAIT);
    }
    // Wiederhole, bis keine Daten mehr gelesen werden
    while (bytesRead > 0);
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
    // Flag für den Login
    bool loggedIn = false;
    // Benutzername
    string username;

    // Bis das Programm beendet wird
    while (true)
    {
        cout << "> ";
        getline(cin, command);

        if (command == "LOGIN")
        {
            if(loggedIn)
            {
                cerr << "You are already logged in." << endl << endl;
                continue;
            }

            string password;

            cout << "Username: ";
            getline(cin, username);

            // Username darf nur aus (a-z) und (0-9) bestehen
            if (username.length() > 8 || !isValidUsername(username))
            {
                if (username.length() > 8)
                {
                    cerr << "Username length exceeds 8 characters." << endl << endl;
                }
                else
                {
                    cerr << "Username contains invalid characters. Only (a-z, 0-9) are allowed." << endl << endl;
                }
                continue;
            }

            // Passwort wird nicht mehr angezeigt
            cout << "Password: ";
            system("stty -echo");
            getline(cin, password);
            system("stty echo");

            // Hardcoded User zu Testzwecken
            if(username == "test" && password == "1234")
            {
                cout << endl << "Login successful." << endl << endl;
                loggedIn = true;
                continue;
            }

            string loginCommand = "LOGIN\n" + username + "\n" + password + "\n";
            send(clientSocket, loginCommand.c_str(), loginCommand.length(), 0);

            string response = readLineFromSocket(clientSocket);

            if(response == "OK")
            {
                cout << endl << "Login successful." << endl << endl;
                loggedIn = true;
            }
            else if(response == "ERR")
            {
                cerr << endl << "Login failed." << endl << endl;
                clearSocketBuffer(clientSocket);
            }
            else
            {
                cerr << endl << "Unexpected response from server: " << response << endl << endl;
            }
        }
        else if (command == "SEND")
        {
            if(!loggedIn)
            {
                cerr << "You need to be logged in to send a message." << endl << endl;
                continue;
            }

            // Sender, Empfänger, Betreff, Nachricht
            string receiver;
            string subject;
            string messageBody;
            string line;

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
            clearSocketBuffer(clientSocket);

            // Gesamte Nachricht
            string fullMessage = "SEND\n" + username + "\n" + receiver + "\n" + subject + "\n" + messageBody + ".\n";
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
            if(!loggedIn)
            {
                cerr << "You need to be logged in to list messages." << endl << endl;
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

                if(count == 0)
                {
                    cout << count << endl << endl;
                    continue;
                }

                // Empfange und drucke die Betreffzeilen
                for (int i = 0; i < count; ++i)
                {
                    string subject = readLineFromSocket(clientSocket);
                    cout << i + 1 << ". " << subject << endl;
                }
                cout << endl;
            }
            else
            {
                cerr << "Unexpected response from server: " << countStr << endl << endl;
            }
        }
        else if (command == "READ")
        {
            if(!loggedIn)
            {
                cerr << "You need to be logged in to read messages." << endl << endl;
                continue;
            }

            // Nachrichtnummer
            int messageNumber;

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
            if(!loggedIn)
            {
                cerr << "You need to be logged in to delete messages." << endl << endl;
                continue;
            }

            // Nachrichtennummer
            int messageNumber;

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
            loggedIn = false;
            cout << username << " logged out." << endl << endl;
        }
        else
        {
            cout << "Unknown command." << endl << endl;
        }
    }
    return 0;
}