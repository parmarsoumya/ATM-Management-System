# UML Diagrams

## Class Diagram

```mermaid
classDiagram

class Account {
    -int accountNumber
    -string name
    -string pin
    -double balance
    +deposit()
    +withdraw()
    +changePin()
}

class ATM {
    -vector accounts
    +login()
    +showBalance()
    +deposit()
    +withdraw()
    +changePin()
    +showMiniStatement()
}

ATM --> Account

### Sequence Diagram

Add below it:

```markdown
## Sequence Diagram

```mermaid
sequenceDiagram

actor User
participant ATM as C++ ATM Application
participant Driver as Linux Character Driver
participant Device as Simulated ATM Device

User->>ATM: Login
ATM-->>User: Authentication result

User->>ATM: Request withdrawal
ATM->>ATM: Check balance
ATM->>Driver: Send withdrawal command
Driver->>Device: Process device command
Device-->>Driver: Device status
Driver-->>ATM: Return status
ATM-->>User: Withdrawal result

### State Machine Diagram

Then:

```markdown
## ATM State Machine

```mermaid
stateDiagram-v2

[*] --> Idle
Idle --> Authenticated: Successful Login
Idle --> Idle: Invalid Login

Authenticated --> BalanceEnquiry
Authenticated --> Deposit
Authenticated --> Withdrawal
Authenticated --> PinChange

BalanceEnquiry --> Authenticated
Deposit --> Authenticated
Withdrawal --> Authenticated
PinChange --> Authenticated

Authenticated --> Idle: Logout

Save it.


