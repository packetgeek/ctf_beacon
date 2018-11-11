# ctf_beacon

## Purpose

This program periodically sends out a UDP broadcast beacon, containing a given string.  Main idea is to check if contestants are paying attention to the network. Syntax is:

    ./ctf_beacon <IP address> <port> "<string>" <seconds between beacons>

## Example

    ./ctf_beacon 192.168.2.255 9999 "flag ABCD-1234-xy2z" 5

## Compiling

Compile this program via:

    gcc -o ctf_beacon ctf_beacon.c

## Notes

* Doesn't require root permissions to run (unless port < 1024).
* This was written at 2 a.m. so please excuse the errors.

## Ideas for future versions

* instead of a set sleep span, how about sleep span plus a random number?
* embed a timestamp

## Adapted from/inspired by

* Noah Davids - http://noahdavids.org/self_published/beacon.html
* Michael J. Donahoo - http://cs.ecs.baylor.edu/~donahoo/practical/CSockets/code/BroadcastSender.c
