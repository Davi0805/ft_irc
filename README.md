# ft_irc - In-Memory IRC Server

## Overview

**ft_irc** is a minimalist IRC server developed in **C++98**, implementing a server compatible with IRC clients for real-time communication. The project's goal is to support multiple clients simultaneously using non-blocking operations while adhering to IRC protocol requirements.

## Architecture and Patterns

The project follows a **layered architecture**, separating responsibilities to improve organization and maintainability:

- **Commands** – Implementation of IRC-supported commands.
- **Core** – Central server logic, including initialization and the main loop.
- **Handlers** – Processing of events and messages received from clients.
- **Models** – Data structures for users and channels.
- **Services** – Specific server functionalities following the **Service Pattern**.
- **Utils** – Utility functions that support server operations.

### Design Patterns Used

- **Command Pattern** – Commands are implemented as classes derived from an abstract class, enabling extensibility and code organization.
- **Non-Blocking I/O** – Uses `poll()` to manage multiple connections without blocking.
- **Service Pattern** – Organizes functionalities into service modules for better modularity.
- **Observer Pattern** – Manages message forwarding between clients and channels.
- **Singleton Pattern** – Ensures a single instance of the server is running.

## Running the Server

```bash
./ircserv <port> <password>
```

- **port:** TCP port for IRC connections.
- **password:** Password required for client authentication.

## Technologies and Restrictions

- **Language:** C++98
- **Allowed libraries:** Only POSIX system calls.
- **Connection management:** `poll()` to prevent blocking.
- **Use of Boost and external libraries is prohibited.**

## Next Steps

- Implement support for bots and file transfers.
- Improve logging and server monitoring.
- Add automated tests.

## References

- [RFC 1459 - Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
- [ft_irc Project on GitHub](https://github.com/Davi0805/ft_irc)

