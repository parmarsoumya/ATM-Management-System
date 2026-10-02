# Stage 3 – System Design and Architecture

## 1. System Overview

The ATM Management System is a Linux-based application developed
using C++. It provides basic ATM operations such as authentication,
balance enquiry, deposit, withdrawal, PIN change and transaction
management.

The system will also include a Linux character device driver that
simulates the hardware interface of an ATM.

## 2. System Architecture

The system consists of the following major components:

1. User
2. ATM C++ Application
3. File Storage
4. Linux System Call Interface
5. ATM Character Device Driver
6. Simulated ATM Hardware Interface

The C++ application operates in user space. The character device
driver operates in kernel space.

## 3. Architecture Flow

User
↓
ATM C++ Application
↓
Linux System Calls
↓
/dev/atm_device
↓
ATM Character Device Driver
↓
Simulated ATM Hardware Interface

## 4. Component Responsibilities

### ATM Application

Responsible for:
- User login
- Account management
- Balance enquiry
- Deposit
- Withdrawal
- PIN change
- Transaction history
- User interface

### File Storage

Responsible for:
- Account information
- Transaction records

### Linux System Call Interface

Provides communication between the C++ user-space application
and the Linux device driver.

### Character Device Driver

Responsible for:
- Creating the ATM device interface
- Receiving commands from user space
- Returning device status
- Simulating ATM hardware interaction

### Simulated Hardware Interface

Represents the hardware portion of the ATM for educational
purposes.

## 5. Hardware/Software Architecture

The software side consists of the C++ ATM application, Linux
system calls and the Linux character device driver.

The hardware side is represented by a simulated ATM hardware
interface such as a cash dispenser.

This project does not control real ATM hardware.

## 6. Data Structures

The C++ application uses an Account data structure/class containing:

- Account number
- Account holder name
- PIN
- Balance

Transaction records contain information about ATM operations.

The device driver maintains its own device state and communication
buffer.

## 7. Operating Environment

Operating System:
Ubuntu 26.04 LTS

Kernel:
7.0.0-30-generic

C++ Compiler:
G++ 15.2.0

Version Control:
Git

Repository:
GitHub

## 8. Development Tools

- Ubuntu Linux
- G++
- GCC
- Linux kernel headers
- Make
- Git
- GitHub
- Terminal
- Text editor

## 9. Security and Scope

This is an educational ATM simulation. It is not intended for
real banking transactions.

The project does not connect to a real bank or real ATM hardware.

## 10. Implementation Plan

Phase 1:
Complete the ATM application.

Phase 2:
Develop the Linux character device driver.

Phase 3:
Compile and load the driver.

Phase 4:
Create and test the ATM device interface.

Phase 5:
Connect the C++ application with the device interface.

Phase 6:
Perform integration and system testing.

Phase 7:
Complete documentation and final demonstration.

## Git Branching Strategy

The main branch contains stable project versions.

Feature branches will be used for major development tasks such as:

- Linux device driver
- Driver integration
- Testing and improvements

Completed features will be tested before being merged into the main branch.

## Stage 4 Implementation Plan

1. Create the Linux character-device driver.
2. Compile the driver using the Linux kernel build system.
3. Load and test the kernel module.
4. Create and verify the ATM device interface.
5. Test read/write communication.
6. Add communication between the C++ ATM application and the driver.
7. Integrate the driver with the ATM transaction workflow.
8. Test the complete prototype.
