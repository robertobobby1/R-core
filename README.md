# R-core

# Installation

### Linux

This will try to install the necessary dependencies of the project and then build the project itself, creating the binary files and the binary intermediates in the bin folder at the root of the project.

```
git clone --recurse-submodule https://github.com/robertobobby1/R-core.git
cd R-core
Scripts/mac-linux-build-app
```

##### Common linux problems

- Your system doesn't contain certain c++ basic libraries, while compiling using the script debug the necessary libraries your system doesn't have.
- The compilation uses [premake](https://premake.github.io/) to generate the make files, if the script runs into an error such as "premake5: Error executing binary file" you should download and build the premake5 binary yourself because your system probably has a different architecture.
  - There are two different linux binaries premake5-linux and premake5-linux-2, if the default one fails try the second one, if this also fails build it yourself from the source code of premake in github, follow their building process

### Windows

1. Run the following command:
   ```
   git clone --recurse-submodule https://github.com/robertobobby1/R-core.git
   ```
2. Open the file explorer and run the windows [windows-build-deps.bat](https://github.com/robertobobby1/R-core/blob/development/Scripts/windows-build-deps.bat "windows-build-deps.bat"), this will create a sln file at the root of the project. Open this file with visual studio and build all the solution, this builds all the external dependencies of the project into a libs folder.
3. After, repeat the same process using the [windows-build-app.bat](https://github.com/robertobobby1/R-core/blob/development/Scripts/windows-build-app.bat "windows-build-app.bat"), which will create a new sln file that will build the R-core library and a "Testing app" which will be the executable. You can start programming in this "Testing app".

### Common problems

- R-core contains some complicated services that need to access the graphics of the computer, in some special cases such as virtual machines or containers the graphics may not be working, the library will log the problem thrown by the graphics engine.

# Usage

### Application core

- Application
  - The main application of R-core takes care of running in a multithreaded way all of the different services that have been added to the app.
  - Initializes all of the main application utils such as logging or memory management
- Services
  - This are the abstract way to represent a process in the library, each service will run in a thread independently implemented in its "Run" method.
  - They may run UI methods from the "ImGui" library in the "OnGuiRender" method. This must be called from the main thread due to it sending graphics to the GPU, that is why this can only be done here, the main application is the one that must run this.
  - As the services run in different threads there is a risk of mutual exclusion problems, this is why we normalize the way services communicate with each other, using some callback utils functions inside the Service base clase we can add and call this callback functions to share data between the services securely.
- Dependency injection
  - Some services may need other services to be running first to work, let's say we have a service that needs the information of the disk and then needs to show it in the UI, this service will depend on the disk reader and on the GUI renderer.
  - This needs to be added into the service in it's own constructor so that the application and the dependency manager may correctly handle the dependency tree on startup.
  - The Dependency manager will make sure that there is no duplication of services using some of the Services's booleans as configuration.

### Service class

##### Params

- m_dataMutex => mutex used to protect the callbacks to share data between services
- m_dependencies => vector where the services should add their dependencies in the constructor
- m_dependencyCallbacks => It's the array where the callbacks are stored and will be called
- m_id => It's the identifier of the Service

##### Param functions

- IsUniqueService() => When a service may not be instantiated more than once
- IsGuiService() => When a service contains gui functions and uses the OnGuiUpdate function

##### Functions

- Init => Start up function for services
- Run => Function that runs in a different thread from main, runs after start up
- OnGuiUpdate => Function that runs inside a loop every frame, it computes the UI changes
- AddDependencyCallback => Adds a dependency callback to the service thread safe
- CallDepCallbacks => Function used to call the subscribed dependency callbacks
- GetDep => static function to cast the dependency service

##### Already existing services

- RCGlfw => Startup service for glfw graphics
- Window => Service that starts a window where you can compute graphics
- Server => A simple multiplatform server that listens and handles requests
- GuiRenderer => A wrapper for UI rendering, this uses ImGui but it may be extended to use a different library
- SkeletonGui => An overwrite of GuiRenderer that will have a base UI already
- LogGui => A service used to show the logging of the app in the UI
- InjectorGui => A service used to show the dependency tree and info of services in the UI
- ApplicationGui => A service that contains as dependency all application related UI's
- FileSystem => A service that will compute using the configuration the folder and files of the disk
- FileSystemGui => Using the Filesystem service it shows in the UI the filesystem in disk
- HttpServer => A service that is a small framework, that uses an external library to set a http server and handles different mechanisms typical in http frameworks, contains:
  - Middlewares
  - Interceptors
  - Controllers

### Examples

To understand better how it works go over the already implemented examples, specifically check [TestingApp.cpp](https://github.com/robertobobby1/R-core/blob/development/Testing/src/TestingApp.cpp) where you can find good examples on how the dependency injection works:

1. Instantiates: ApplicationGui, FileSystemGui and HttpServer
2. ApplicationGui instantiates FileSystemGui, LogGui and InjectorGui
3. FileSystemGui instantiates FileSystem and SkeletonGui
4. LogGui instantiates SkeletonGui
5. InjectorGui instantiate SkeletonGui
6. A second FileSystemGui instantiates with same dependencies again!
7. HttpServer instantiates nothing
8. Application then handles the dependencies as follows:
   1. 1 ApplicationGui
   2. 1 FileSystemGui
   3. 1 HttpServer
   4. 1 LogGui
   5. 1 InjectorGui
   6. 2 FileSystem => 1 FileSystem
   7. 2 SkeletonGui => 1 SkeletonGui as active GuiRenderer
