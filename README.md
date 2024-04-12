# Parking Management System

The project develops a parking management system capable of defining parking lots and registering vehicle entries and exits, as well as enabling their consultation and billing. Interactions with the program occur through a set of lines composed of a letter (command) and a number of arguments depending on the command to be executed. 

Memory allocation is a critical aspect of this project. In the provided code, memory allocation plays a significant role in various functions, such as initializing the parking system, creating new parks and vehicles, and managing logs of vehicle entries and exits. When the system terminates all memory allocated is free'ed.

## Specifications of the problem

Each parking lot is identified by a name, a maximum capacity, and a billing regime. There can only be 20 total parks in the system at the same time. The name of the parking lot can contain whitespace and is delimited by quotes when it contains whitespace characters. The billing amount is calculated in 15-minute intervals, with a rate X for the first 4 periods and a rate Y for subsequent periods, up to a daily maximum of Z. The daily maximum rate is applied to each complete 24-hour period a vehicle remains in the parking lot. On February 29th, the parking lot is closed, and no charges are applied for that day. Each vehicle is identified by a license plate consisting of three pairs of characters separated by '-'. The entry of a vehicle reduces the number of available spaces in the parking lot, and the exit frees up a space. Entry and exit records must follow chronological order, and a vehicle can only be registered in another parking lot after exiting the current one.

## Prerequisites

 **gcc:** The GNU Compiler Collection.
 
 **make**


## Installation

To install and run the system, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/guilhermedcampos/park-management-system.git
    ```

2. Navigate to the project directory and compile the source code:

    ```bash
    cd src
    gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -g -pg --no-inline -o proj *.c
    ```
3. Run the server in a terminal:

    ```bash
    ./proj
    ```

## Contents

    - src/project.c: Main source file with the main functions.
    - src/project.h: Header file with all  the structs and main functions prototypes.
    - src/projectaux.c: Auxiliar source file with the auxiliar functions.
    - src/projectaux.h: Header file with all the auxiliar functions prototypes.
    - src/constants.h: Header file with all constants.
    - private/makefile: Makefile for running the program on all the inputs files in private.
    - public/makefile: Makefile for running the program on all the inputs files in publ ic.


## Command Syntax

Once the program is running, these are the possible commands. Their actions are listed in the table below:

    Command	Action
        q	Terminates the program

        p	Creates a parking lot or lists existing lots

            p [ <parking_name> <max_cap> <billingX> <billingY> <billingZ> ]
        
        e	Registers a vehicle entry

            e  <parking_name> <plate> <date> <hour> 
        
        s	Registers a vehicle exit

            s <parking_name> <plate> <date> <hour> 
        
        v	Lists vehicle entries and exits

            v <plate>
        
        f	Shows parking lot billing

            f <parking_name> [ <date> ]
        
        r	Removes a parking lot from the system

            r <parking_name>

## Examples of usage

    p
    p park1 200 0.20 0.30 12.00
    p "park 1" 400 0.25 0.40 20.00
    
    e park1 AA-00-AA 01-03-2024 08:34
    
    s park1 AA-00-AA 01-03-2024 10:59

    v AA-00-AA

    f park1

    f park1 01-03-2024

    r "park 1"

## Tests

The repository includes a total of 50 tests located in the public/ and private/ folder. 
Each file tests different commands and logics, generaly increasing in complexity.

To run a specific test, navigate to either private/ and public/ and run:

```bash
../src/proj < test.in > test.myout
```

To compare the expected output (*.out) with our output (*.myout):

```bash
diff test.in test.myout
```