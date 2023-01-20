# CShard
Reimplementation in C++ of the closed source Shard engine for the course Introduction To Game Engine Architecture

# How to use

First thing to do is to download the code:
```
git clone https://github.com/AsperTheDog/CShard --recursive
```
Notice the `--recursive`. This is important, that way all of the necessary dependencies are properly downloaded
In the CShard folder you will find the solution file. 

# Prerequisites

Unfortunately glew is not easy to build. There are some tools that our computer needs before we are able to compile it:

### WSL

Since glew requires us to run a makefile we need wsl.
To install wsl simply open a command console and type
```
wsl --install
```

### linux dependencies

Once wsl has been installed you have to install python3 and make to linux
Examples using apt:
```
sudo apt-get install python3
sudo apt-get install make
```

Additionally, if python is ran as python3 this package must be installed for the make to work correctly
```
sudo apt-get install python-is-python3
```

Once all of these are up and running the visual studio project should build and run without any problems
