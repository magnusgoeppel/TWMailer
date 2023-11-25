#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

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


void handleClient(int clientSocket, string directory)
{
    // Command
    string command;

    while (true)
    {
        // Lösche den Inhalt des letzten Kommandos
        command.clear();

        // Empfange das Kommando
        command = readLineFromSocket(clientSocket);

        command += "\n"; // Füge den Zeilenumbruch am Ende hinzu

        if (command == "SEND\n")
        {
            // Sender, Empfänger, Betreff, Nachricht
            string sender;
            string receiver;
            string subject;
            string messageBody;
            string line;

            // Empfange Sender
            sender = readLineFromSocket(clientSocket);

            // Empfange Empfänger
            receiver = readLineFromSocket(clientSocket);

            // Empfange Betreff
            subject = readLineFromSocket(clientSocket);

            // Empfange Nachricht
            messageBody.clear();
            while (true)
            {
                line = readLineFromSocket(clientSocket);

                messageBody += line + "\n";

                if (line == ".")
                {
                    break;
                }
            }

            // Dateiname
            string filename = directory + "/" + receiver +".txt";

            // Verbindung zum Dateisystem
            ofstream outFile(filename, ios::app);


            // Wenn die Datei geöffnet werden konnte
            if (outFile.is_open())
            {
                // Schreibe die Nachricht in die Datei
                outFile << "From: " << sender << "\n";
                outFile << "Subject: " << subject << "\n";
                outFile << "Message: "<< messageBody;
                // Trenner für Nachrichten
                outFile << "----------------------------------------------------------------------\n";

                // Schließe die Datei
                outFile.close();
                // Sende OK an den Client
                send(clientSocket, "OK\n", 3, 0);
            }
            else
            {
                // Sende ERR an den Client
                send(clientSocket, "ERR\n", 4, 0);
            }
        }
        else if (command == "LIST\n")
        {
            // Benutzername vom Client empfangen
            string username = readLineFromSocket(clientSocket);
            // Dateiname für die Nachrichten des Benutzers
            string filename = directory + "/" + username + ".txt";

            // Verbindung zum Dateisystem
            ifstream inFile(filename);

            // Wenn die Datei nicht geöffnet werden konnte
            if (!inFile.is_open())
            {
                // Keine Nachrichten für den Benutzer
                send(clientSocket, "0\n", 2, 0);
                continue;
            }

            // Lese die Betreffzeilen aus der Datei
            vector<string> subjects;
            // Zeile
            string line;

            // Solange eine Zeile gelesen werden kann
            while (getline(inFile, line))
            {
                if (line.substr(0, 8) == "Subject:")
                {
                    // Betreffzeile
                    subjects.push_back(line.substr(9));
                }
            }
            // Schließe die Datei
            inFile.close();

            // Anzahl der Nachrichten
            string countStr = to_string(subjects.size()) + "\n";
            // Sende die Anzahl der Nachrichten
            send(clientSocket, countStr.c_str(), countStr.length(), 0);

            // Sende die Betreffzeilen
            for (int i = 0; i < subjects.size(); i++)
            {
                // Betreffszeile
                string subjectWithNewline = subjects[i] + "\n";
                // Sende die Betreffszeile
                send(clientSocket, subjectWithNewline.c_str(), subjectWithNewline.length(), 0);
            }
        }
        else if (command == "READ\n")
        {
            // Nachrichtenempfänger
            string username = readLineFromSocket(clientSocket);

            // Nachrichtennummer
            int messageNumber = stoi(readLineFromSocket(clientSocket));

            // Dateiname für die Nachrichten des Benutzers
            string filename = directory + "/" + username + ".txt";

            // Verbindung zum Dateisystem
            ifstream inFile(filename);

            // Wenn die Datei nicht geöffnet werden konnte
            if (!inFile.is_open())
            {
                cerr << "Fehler beim Öffnen der Datei: " << filename << endl;
                send(clientSocket, "ERR\n", 4, 0);
                continue;
            }

            // Zähler für die Nachrichten
            int currentIndex = 0;
            // aktuelle Zeile
            string line;
            // Nachricht
            string messageContent;
            // Flag, ob die Nachricht gelesen wird
            bool isReadingMessage = false;

            // Variablen für Sender, Empfänger und Betreff
            string sender;
            string receiver;
            string subject;

            // Solange eine Zeile gelesen werden kann
            while (getline(inFile, line))
            {
                // Wenn die Zeile den Absender der Nachricht enthält
                if (line.substr(0, 6) == "From: ")
                {
                    sender = line.substr(6) + "\n";

                    // Wenn die aktuelle Nachricht die gesuchte Nachricht ist
                    if (currentIndex == messageNumber)
                    {
                        // Wenn die nächste Zeile den Betreff der Nachricht enthält
                        if (getline(inFile, line) && line.substr(0, 9) == "Subject: ")
                        {
                            subject = line.substr(9) + "\n";

                            // Wenn die folgende Zeile den Beginn des Nachrichteninhalts kennzeichnet
                            if (getline(inFile, line) && line.substr(0, 9) == "Message: ")
                            {
                                // Füge die erste Zeile des Nachrichteninhalts hinzu.
                                messageContent += line.substr(9) + "\n";

                                // Lese alle Zeilen bis zum Nachrichtentrenner und füge sie zum Nachrichteninhalt hinzu
                                while (getline(inFile, line) && line != "----------------------------------------------------------------------")
                                {
                                    messageContent += line + "\n";
                                }
                                // Setze das Flag, dass die Nachricht gelesen wird
                                isReadingMessage = true;
                                // Beenden Sie die Schleife, da Sie die gesuchte Nachricht gefunden haben
                                break;
                            }
                            else
                            {
                                cerr << "Fehler beim Lesen der Nachricht." << endl;
                                break;
                            }
                        }
                    }
                    else
                    {
                        // Überspringen der Zeilen, die zu einer Nachricht gehören, die wir nicht ausgeben wollen
                        while (getline(inFile, line) && line != "----------------------------------------------------------------------")
                        {
                            // Nichts tun. Wir lesen nur die Zeilen bis zum nächsten Trenner.
                        }
                    }
                    // Erhöhe den Index nach jedem Durchgang durch eine Nachricht
                    ++currentIndex;
                }
            }

            // Wenn die Nachricht gelesen wurde, sende sie an den Client
            if (isReadingMessage)
            {
                // Senden Sie die Bestätigung an den Client
                send(clientSocket, "OK\n", 3, 0);

                // Senden Sie den Sender, Betreff und Inhalt an den Client
                send(clientSocket, sender.c_str(), sender.length(), 0);
                send(clientSocket, subject.c_str(), subject.length(), 0);
                send(clientSocket, messageContent.c_str(), messageContent.length(), 0);

            }
            // Sonst gib einen Fehler aus
            else
            {
                send(clientSocket, "ERR\n", 4, 0);
            }
            // Schließe die Datei
            inFile.close();
        }
        else if (command == "DEL\n")
        {
            // Empfange Benutzername und Nachrichtennummer vom Client
            string username = readLineFromSocket(clientSocket);
            int messageNumber = stoi(readLineFromSocket(clientSocket)) - 1;

            // Dateiname für die Nachrichten des Benutzers
            string filename = directory + "/" + username + ".txt";

            // Öffne die Datei zum Lesen
            ifstream inFile(filename);
            if (!inFile.is_open())
            {
                send(clientSocket, "ERR\n", 4, 0);
                continue;
            }

            // Lese alle Nachrichten in einen Vektor von Strings
            vector<string> messages;
            string line;
            string currentMessage;
            while (getline(inFile, line))
            {
                if (line == "----------------------------------------------------------------------")
                {
                    messages.push_back(currentMessage);
                    currentMessage.clear();
                }
                else
                {
                    currentMessage += line + "\n";
                }
            }
            inFile.close();

            // Überprüfe, ob die Nachrichtennummer gültig ist
            if (messageNumber < 0 || messageNumber >= messages.size())
            {
                send(clientSocket, "ERR\n", 4, 0);
                continue;
            }

            // Lösche die Nachricht aus dem Vektor
            if (messageNumber < messages.size())
            {
                // Verschiebe alle Elemente nach dem zu löschenden Element um eins nach vorne
                for (int i = messageNumber; i < messages.size() - 1; ++i)
                {
                    messages[i] = messages[i + 1];
                }
                // Entferne das letzte Element, da es jetzt ein Duplikat des vorletzten ist
                messages.pop_back();
            }

            // Öffne die Datei zum Überschreiben
            ofstream outFile(filename);
            if (!outFile.is_open())
            {
                send(clientSocket, "ERR\n", 4, 0);
                continue;
            }

            // Schreibe die aktualisierten Nachrichten zurück in die Datei
            for (int i = 0; i < messages.size(); ++i)
            {
                outFile << messages[i];
                outFile << "----------------------------------------------------------------------\n";
            }
            // Schließe die Datei
            outFile.close();

            // Sende die Bestätigung an den Client
            send(clientSocket, "OK\n", 3, 0);
        }
        // Beende die Verbindung
        else if (command == "QUIT\n")
        {
            // Schließe den Socket
            close(clientSocket);
            return;
        }
        else
        {
            send(clientSocket, "ERR\n", 4, 0);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: ./twmailer-server <port> <mail-spool-directoryname>" << endl;
        return EXIT_FAILURE;
    }

    // 1. Argument: Port (stoi: string -> int)
    int port = stoi(argv[1]);
    // 2. Argument: Mail-Spool-Verzeichnis
    string directory = argv[2];

    // Neuer Socket (AF_INET: IPv4, SOCK_STREAM: TCP, Protocol: Standardprotokoll)
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Speicher die IP und den Port des Servers
    sockaddr_in serverAddr;
    // Adresstyp: IPv4
    serverAddr.sin_family = AF_INET;
    // Port -> Netzwerk-Byte-Format
    serverAddr.sin_port = htons(port);
    // IP -> Netzwerk-Byte-Format
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // ServerSocket an die IP und den Port binden
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    // ServerSocket in den Listen-Modus versetzen, maximal 5 Verbindungen
    listen(serverSocket, 5);


    // Akzeptiere eine Verbindung
    int clientSocket = accept(serverSocket, NULL, NULL);
    // Verarbeite die Verbindung
    handleClient(clientSocket, directory);

    return 0;
}