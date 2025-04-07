# SecureBox test task

The repository is created in order to implement, share and discuss the code written for the job interview test task.

## Quick start

The project uses CMake build tools for project configuration.

### Installation guide

You will need a C++ compiler which supports the C++20 standard.

* clone the repository
* `make install`
* `make prepare`
* ececute the command `cmake --build . --config Release --target secret_box -j 10 --` to build the project
* the executable can be found in the {project_root}/build/bin/Release/secure_box

### Usage example

The program receives two mandatory parameters of type `unsigned int`. The output shows whether the randomply generated matrix was successfully unlocked.

To run the program execute the following comamnd from the build folder `.//bin/Release/secure_box 10 10`

## Project structure

```
├── app
├── src
│   ├── SecureBox
│   │   └── includes
│   └── SecureBoxHack
│       └── includes
└── tests
```

* app folder contains the main executable target
* src contains two libraries targets
  * SecretBox contains the code of the original SecretBox task without modifications
  * SecureBoxHack contains the code for cracking the SecretBox unlock algorithm
* tests contains the unit tests for the cracker

# Dependencies

* Gtest opensource library for the unit tests
