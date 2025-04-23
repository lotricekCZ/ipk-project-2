CHANGELOG:

1.0.1 (2025-04-23)

- Made TCPDecoder case sensitive.
- Code cleanup, there were some unused variables.
  - set and unused variables (like unused lambda expressions) were removed.
  - by using -Wextra compile flag I found some more unused function arguments. I dealt with these according to [https://stackoverflow.com/a/4647698/8847199](https://stackoverflow.com/a/4647698/8847199)
  - currently with -Wextra there are only missing field initializers and unused function arguments are only in Argumentum.

1.0.0

I consider this to be the first stable release of the project, however let's recap the functionality and quirks in this release:
### Usage
The core functionality is within these 3k lines of code and is implemented. However, longer messages are not supported.
### Networking
Both TCP and UDP are supported and should work unless there is a network issue or an address resolution fails.
### Finite state automata
All automata are implemented (TCP is missing in the README as it is implemented exactly as the assignment suggests). No automaton responsible for sending and receiving messages does track whether the channel we already aren't in a channel we're trying to join.
 - TCP FSM however shows some quirks I was not capable of tracking down (e.g. the local error message may be displayed after joining different channel and sending a message and joining different channel). Also due to staying to its early design, it may be possible to send a message without being authenticated due to the fact that the message is not sent on a transition, but immediately after its construction.
 - UDP FSM seems to have no issues, despite its complexity. Same can be said for UDP decoders based on the automata.
 - Changes within code had been made compared to the specification, these changes however were not backpropagated.
### Messages
All messages are implemented, but as stated before, the content is capped to the 60k characters limit without possibility of splicing it to smaller chunks.
### IO handling
The IO handling is implemented and should work as described. In the event of unrecognised / command, the program will display local error.
### Memory management
All the memory is freed correctly except for 5291 bytes which are marked as still reachable and may be a bug in a dynamic library linkage.