# Extra exercise

## Description

The goal of this small program is to create a **server** that counts the number of times a given number appears in a **vector**. When the user starts the server, it displays the vector. After that, clients can send **requests** to the server by providing a number (or a flag).

When the server starts, it creates a process called **the register**. This process is responsible for **logging** search queries made in the array. The server then enters a loop, waiting to receive requests from clients. Upon **receiving a request**, it creates a **new process** to search for the requested value. This process also **sends the response** back to the client and logs the search details to the register. Communication between the process and the register is handled using **anonymous pipes**.

When the server shuts down, it waits for all the processes it created to finish before terminating the register process.

## Usage

Run `make` to compile the code and generate the following **executables**:
- `server` : starts the server (no arguments required).
- `client` : sends a request to the server

To **send a request** to the server, run these commands:
- `./client <value>` searches for the given value in the vector.
- `-/client -f` shuts down the server

