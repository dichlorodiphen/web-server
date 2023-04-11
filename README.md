# Web Server

This project is a web server implemented in C. It was inspired by a programming assignment at the end of chapter 2 of *Computer Networking: A Top-Down Approach* by Jim Kurose.

The purpose of it is to refamiliarize myself with network programming in C, so I intend it to be more of a proof of concept than a production-grade server. For the same reason, the scope of the project will be highly limited.

## Design

### server.c

`server.c` is the main file, containing the logic for thread creation and management. Threads are created on request intake and added to a mutex-protected global thread list. They are deleted upon response generation.

### parser.c

`parser.c` contains logic for parsing HTTP requests to `http_request_t` objects.

### dispatcher.c

`dispatcher.c` contains logic for delegating control to the various HTTP request handlers in `request_handlers/`.

### dal.c

`dal.c` (data access layer) contains the logic for interacting with the server's file storage.

