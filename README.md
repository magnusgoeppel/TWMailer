## TWMailer

## Introduction
This guide describes the installation and use of the TWMailer client and server programs. TWMailer allows users to interact with a server through a client to log in, send messages, list messages, read messages, and delete messages. The server application uses LDAP for authentication and stores messages in a specified directory.

## Prerequisites
- Linux operating system
- g++ compiler
- LDAP libraries for C++
- A configured and reachable LDAP server (otherwise, login is only possible with the test user)

## Installation
### Compiling the Client and Server:
```bash
g++ -o twmailer-client twmailer-client.cpp 
g++ -o twmailer-server twmailer-server.cpp -lldap -llber
```

## Server Configuration
Before starting the server, you must configure the LDAP server URI and the baseDN directly in the code of the `twmailer-server`. Open `twmailer-server.cpp` in a text editor and set the appropriate values for your environment:

```cpp
// Example configuration
18 const char *ldapUri = "ldap://example.com:389";
55 string baseDN = "dc=example,dc=com";
```

### Starting the Server
Start the server with the following command:

```bash
./twmailer-server <Port> messages
```
- `<Port>`: The port on which the server should run.
- `messages`: The directory where the messages are stored. In this case, the `messages` directory is used as the storage location for email messages.

### Client
Start the client with the following command:

```bash
./twmailer-client <Server-IP> <Port>
```
- `<Server-IP>`: The IP address of the server.
- `<Port>`: The port on which the server is running.

### After starting the client, you can use the following commands:

- **LOGIN**: Log in with your username and password. For testing purposes, a user is hardcoded: Username `test` and password `1234`.
- **SEND**: Send a message.
- **LIST**: List all your messages.
- **READ**: Read a specific message.
- **DEL**: Delete a specific message.
- **QUIT**: Log out.
