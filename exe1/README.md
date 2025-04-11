# HY-457 - Assignment 1

## Student Info

Name: Dimitrios Makrogiannis
AM: 4676

---

## Structure

This assignment is divided into three tasks. Each task is implemented in a separate directory:

- `monitor/` → Task 1: Execution Monitoring Environment
- `analyze/` → Task 2: Network Traffic Analyzer
- `vault/` → Task 3: Secret Sharing System

---

## Task 1: Monitor

- Computes MD5 & SHA256 hashes using OpenSSL.
- Extracts strings (like UNIX `strings`).
- Monitors the target program with `ptrace()`.
- Tracks system calls, file access (read/write), and `sendto()` for HTTP.
- Generates full report as shown in the assignment.

Fully implemented.

---

## Task 2: Analyze

- Processes `.pcap` file using the `pcap` library.
- Supports IPv4 packets.
- Extracts Ethernet, IP, TCP, UDP headers.
- Filters outgoing traffic based on given IP (supports CIDR too).
- Aggregates and displays bytes, protocols, ports per destination IP.

Fully implemented.

**Note:** I do **not** explicitly check for:

- Multicast
- Broadcast
- Local multicast

---

## Task 3: Vault

- Implements a Secret Sharing system using a 2nd-degree polynomial.
- `vault split <password>` stores password to `password.txt` and writes 10 shares to `points.txt`.
- `vault join` reads from `points.txt`, picks 3 random shares, and reconstructs the password.
- Output matches the format shown in the assignment.

Fully implemented.

---

## Build & Test

Each task has its own `Makefile` with:

- `make` → compiles the system
- `make run` → runs a test
- `make clean` → removes build files

## Notes

- All code is written in C and tested on a CSD-compatible environment.
- Repositories (if used) are private as instructed.
- No code was shared publicly or privately.
