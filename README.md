## TWMailer Client und Server

## Einführung
Diese Anleitung beschreibt die Installation und Verwendung des TWMailer-Client- und Server-Programms. Der TWMailer ermöglicht es Benutzern, über einen Client mit einem Server zu interagieren, um sich anzumelden, Nachrichten zu senden, zu listen, zu lesen und zu löschen. Die Serveranwendung nutzt LDAP für die Authentifizierung und speichert Nachrichten in einem spezifizierten Verzeichnis.

## Voraussetzungen
- Linux-Betriebssystem
- g++ Compiler
- LDAP-Bibliotheken für C++


## Installation
### Kompilieren des Clients und des Servers:
```bash
g++ -o twmailer-client twmailer-client.cpp 
g++ -o twmailer-server twmailer-server.cpp -lldap -llber
```

### Einrichtung des Mail-Spool-Verzeichnisses:

Erstellen Sie ein Verzeichnis, in dem der Server die E-Mail-Nachrichten speichern kann.

```bash
mkdir /pfad/zum/mail-spool
```

### Server
Starten Sie den Server mit dem folgenden Befehl:

```bash
./twmailer-server <Port> <Mail-Spool-Verzeichnis>
```
- <Port>: Der Port, auf dem der Server laufen soll.
- <Mail-Spool-Verzeichnis>: Der Pfad zum Verzeichnis, in dem die Nachrichten gespeichert werden.

### Client
Starten Sie den Client mit dem folgenden Befehl:

```bash
./twmailer-client <Server-IP> <Port>
```
- <Server-IP>: Die IP-Adresse des Servers.
- <Port>: Der Port, auf dem der Server läuft.

### Nach dem Start des Clients können Sie die folgenden Befehle verwenden:

- LOGIN: Melden Sie sich mit Ihrem Benutzernamen und Passwort an. Für Testzwecke ist ein Benutzer hartcodiert: Benutzername `test` und Passwort `1234`.
- SEND: Senden Sie eine Nachricht.
- LIST: Listet alle Ihre Nachrichten auf.
- READ: Lesen Sie eine spezifische Nachricht.
- DEL: Löschen Sie eine spezifische Nachricht.
- QUIT: Melden Sie sich ab.

