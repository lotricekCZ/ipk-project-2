# IPK25 Chat client
### Author: Jakub Ramašeuski (xramas01), 2025
## Table of contents
- [IPK25 Chat client](#ipk25-chat-client)
		- [Author: Jakub Ramašeuski (xramas01), 2025](#author-jakub-ramašeuski-xramas01-2025)
	- [Table of contents](#table-of-contents)
	- [Abstract](#abstract)
	- [Compilation](#compilation)
	- [Usage](#usage)
	- [Content structuring](#content-structuring)
	- [Theory](#theory)
    	- [Finite state automata](#finite-state-automata)
		- [Class diagram](#class-diagram)
		- [Dependencies](#dependencies)
	- [Testing](#testing)
	- [Known issues](#known-issues)
	- [valuable sources](#valuable-sources)
## Abstract

This is a client for IPK25 chat application used for communication on both TCP and UDP transport layers, written in C++, but also a testament to an object oriented design.

## Compilation
In a root of this directory, use `make -j${nproc}` or `make` and consequently `make clean` to clean after usage.

## Usage
### when launching
When launching the program, use
`./ipk25-chat --protocol tcp/udp --hostname HOSTNAME [--port uint16]
[--retransmissions uint8] [--timeout uint16] [--help]`
where:
- `--protocol` is either `tcp` or `udp`
- `--hostname` is the hostname of the server or IP address
- `--port` is the port of the server
- `--retransmissions` is the number of retransmissions (used in UDP)
- `--timeout` is the timeout in seconds (used in UDP)
- `--help` prints help message

### when running
When running the program, consider using `/help` to get help.

## Content structuring
## Theory
Just as much is project focused on establishing network connections, it is also focused on utilising finite state automata using oriented graphs.
In this section both networking and will be discussed.
### Networking
Both TCP and UDP are considered networking protocols. While sharing some similarities, they differ in what actions are performed in order to achieve the goal of sending and receiving messages and in what format the messages are sent and received.
#### TCP
In order to send message or communicate through TCP, a connection first must be established. This happens during initiation of TCPSender or any 
#### UDP
### Finite state automata
Due to the absence of regular expressions for non-ascii bytes, finite automata were made to both encode and decode
![](assets/UDP_decoder.svg)
![](assets/UDP_encoder.svg)

While UDP finite state automaton preserves same ammount of states as TCP finite state automaton, it is implemented as a "Queue" automaton. and as much as its transition functions rely on input alphabet comprised of Mealy inputs and outputs, it also utilises a queue of messages and a condition that some of these messages within the queue have triggered a timeout.
![](assets/UDP_Finite_state_automaton.svg)
## Class diagram
![Class diagram](assets/class_diagram.svg)
I should mention that this diagram was due to the extent of this project generated using [hpp2plantuml](https://github.com/thibaultmarin/hpp2plantuml).

## Dependencies

