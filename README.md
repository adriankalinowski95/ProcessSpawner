# Process spawner

## Overview

**Process spawner** is a comprehensive project designed to enable the remote management of processes. It provides functionalities for creating subprocesses, collecting data from them, facilitating two-way communication between subprocesses and the core system, and monitoring processes through a web-based application. This project serves as a robust foundation for systems requiring efficient and scalable remote process management.

## Features

- **Create New Subprocesses:** Dynamically spawn new subprocesses as needed.
- **Data Collection from Subprocesses:** Gather and aggregate data generated by subprocesses.
- **Bidirectional Communication:** Enable two-way data exchange where subprocesses can request data from the core, and the core can retrieve data from subprocesses.
- **Periodic Data Collection:** Implement cyclic data gathering to ensure up-to-date information is maintained.
- **Web-Based Management Interface:** Control and manage processes through an intuitive web browser application.

## Technologies Used

### Client

- **Angular 17:** Front-end framework for building dynamic user interfaces.
- **Angular Material:** UI component library for Angular.
- **REST APIs:** For communication between the client and core services.

### Core

- **.NET Core:** Backend framework for building scalable and high-performance applications.
- **Entity Framework Core:** ORM for database interactions.
- **gRPC / Protobuf:** For efficient and high-performance communication between services.
- **REST APIs:** For exposing core functionalities to clients.
- **AutoMapper:** For object-object mapping.
- **FluentValidation:** For model validation.
- **Dependency Injection:** For managing dependencies and promoting modularity.
- **JWT (JSON Web Tokens):** For secure authentication and authorization.
- **SQLite:** Lightweight, file-based database.
- **Quartz:** Scheduling library for executing jobs.
- **Newtonsoft.Json:** For JSON serialization and deserialization.

### Application

- **CMake:** Cross-platform build system.
- **Conan:** C/C++ package manager.
- **C++ 20:** Modern C++ programming language.
- **gRPC / Protobuf:** For service communication.
- **nlohmann_json:** JSON library for C++.
- **Boost:** Collection of portable C++ source libraries.

## Prerequisites

Ensure the following software is installed on your macOS system:

- **C++ 23**
- **Conan 2.10.1**
- **CMake 3.31.2**
- **.NET 9.0.100**
- **Node.js 23.3.0**

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/remote-process-manager.git
cd remote-process-manager
```

### 2. Build the Project
Navigate to the main directory of the Process Spawner and execute the build script:
```bash
./build_project.sh
```

Optional: To build individual components separately, navigate to each project directory and run their respective build scripts:
```bash
cd core
./build_script.sh

cd ../client
./build_script.sh

cd ../application
./build_script.sh
```

## Running the Application

**Note**: The order of starting services is crucial for the proper functioning of the application.

### 1. Start the Core Service
Navigate to the `core` directory and execute the start script:

### 2. Start the Client Application
Navigate to the `client` directory and execute the start script:

``` bash
cd ../client
./start_script.sh
```

### 3. Start the Application Manager
Navigate to the `application` directory and execute the start script. This script allows parameterization of the `process_manager`. To view available options, run:

``` bash
cd ../application
./start_script.sh --help
```

**Note**: The order of provided parameters does not affect the execution.

## Project Structure

```text
process_spawner/
├── build_project.sh
├── core/
│   ├── build_script.sh
│   ├── start_script.sh
│   └── ...
├── client/
│   ├── build_script.sh
│   ├── start_script.sh
│   └── ...
├── application/
│   ├── build_script.sh
│   ├── start_script.sh
│   └── ...
├── README.md
└── ...
 ```


## Building and Running Scripts

### Building the Project

#### 1. Navigate to the Main Directory:
``` bash
cd remote-process-manager
```

#### 2. Run the Build Script:
``` bash
./build_project.sh
```

#### 3. (Optional) Build Individual Components:
``` bash
cd core
./build_script.sh

cd ../client
./build_script.sh

cd ../application
./build_script.sh
``` 

### Running the Application
##### 1. Start Core Service:
``` bash
cd core
./start_script.sh
```

##### 2. Start Client Application:
``` bash
cd ../client
./start_script.sh
```

**Warning**
You can start only **1** process manager per one system.
##### 3. Start Application Manager:
``` bash
cd ../application
./start_script.sh
```

## Contributing
Contributions are welcome! Please follow these steps:
##### 1. Fork the Repository.
##### 2. Create a New Branch for Your Feature or Bugfix.
##### 3. Commit Your Changes with Clear and Descriptive Messages.
##### 4. Push Your Branch to Your Forked Repository.
##### 5. Open a Pull Request Detailing Your Changes.


## License
This project is licensed under the MIT License.

## Contact
For any inquiries or support, please contact adrian.kalinowski95@gmail.com