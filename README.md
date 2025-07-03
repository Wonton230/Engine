# Engine
A beginner work in progress game engine for learning game engine design and systems.

## Setup

**The following properties must be changed in the Engine Project within Visual Studio:**

**Be sure that these settings apply to all configurations and platforms*

- Ensure Configuration Properties -> General -> Configuration Type is:

  ```Static library (.lib)```

- Ensure Configuration Properties -> General -> Intermediate Directory and Output Directory, are both:

  ```$(SolutionDir)Temporary\$(ProjectName)_$(PlatformShortName)_$(Configuration)\```

- Ensure Configuration Properties -> C/C++ -> Additional Include Directories for the Engine is:

  ```$(SolutionDir)Code/;$(SolutionDir)../Engine/Code/```

- Ensure Configuration Properties -> Librarian -> Additional Library Directories setting for Engine is:

  ```$(SolutionDir)Code/;$(SolutionDir)../Engine/Code/```

- Ensure Configuration Properties -> C/C++ -> Warning Level is: ```Level4```.

- Ensure Configuration Properties -> General -> C++ Language Standard is: 

  ```ISO C++17 Standard (/std:c++17)```


**In order to use this Engine in another project, these settings must apply to the other project:**

**Be sure that these settings apply to all configurations and platforms*

- Place your solution's directory adjacent to the top level Engine directory. Please refer to the diagram below:
  ```
  SomeDirectory/				
      YourProjectDirectory/
        YourProjectDirectory.sln
        Content/
        Run/ *Make sure this folder exists. This will be where build exe's get placed using the settings above
        Temporary/
      Engine/
        Docs/
        Code/
          Engine/
  ```

- For the General -> Intermediate Directory and Output Directory, these should both be:
  
  ```$(SolutionDir)Temporary\$(ProjectName)_$(PlatformShortName)_$(Configuration)\```
  
- Change Configuration Properties -> General -> Target Name to:

  ```$(ProjectName)_$(Configuration)_$(PlatformShortName)```

- Change Configuration Properties -> Debugging -> Command to:

  ```$(SolutionDir)Run/$(TargetFileName)```

- Change Configuration Properties -> Debugging -> Working Directory to:

  ```$(SolutionDir)Run/```

- Change Configuration Properties -> C/C++ -> Additional Include Directories to:

  ```$(SolutionDir)Code/;$(SolutionDir)../Engine/Code/```

- Change Configuration Properties -> Linker -> Additional Library Directories likewise:

  ```$(SolutionDir)Code/;$(SolutionDir)../Engine/Code/```
  
- Change Configuration Properties -> C/C++ -> Warning Level to: ```Level4```.

- Change Configuration Properties -> Build Events -> Post-Build Event -> Command Line to:

  ```xcopy /Y /F /I "$(TargetPath)" "$(SolutionDir)Run"```
  
- Change Configuration Properties -> Build Events -> Post-Build Event -> Description to:

  ```Copying $(TargetFileName) to $(SolutionDir)Run...```
  
- Change Configuration Properties -> General -> C++ Language Standard to:

  ```ISO C++17 Standard (/std:c++17)```
  
- Right click the References filter in your project and select "Add Reference". Check the box for "Engine".
