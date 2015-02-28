# PiBell-GUI  
PiBell-GUI is a small prototypical java UI implementation that allows to ring the PiBell. It does that by sending a 'Ring' request to the PiBell-Server backend, which is then picked up by the PiBell-Pi client. I'm describing how to run the UI in the following paragraphs. However, I assume that you have checked out this git repository already. You could also simply copy the content of the *bin* folder to run the PiBell-GUI.  

#### How to run  
1) Make sure you have Java 8 installed on your system.  
2) Check if the *JAVA_HOME* variable is set on your system, and if not, set it:   
  2a) For Windows: Open a command window via *CTRL-r -> cmd*, then type  
  `echo %JAVA_HOME%`  
  2b) For Linux: Open a terminal via *CTRL + ALT + t*, then type  
  `echo $JAVA_HOME`  
3) Navigate to whereever your PiBell-GUI bin folder is located, e.g., via  
  `cd /home/your-username/PiBell/PiBell-GUI/bin`  
4) You should now be able to simply start the PiBell-GUI by  
  4a) Windows: PiBell-GUI.bat  
  4b) Linux: ./PiBell-GUI.sh  
**NOTE:** Be aware that either script expects the previously mentioned *JAVA_HOME* to be set.


#### How to Build
PiBell-GUI is a [gradle][1] project, which simplifies the build process tremendously. Please make sure gradle is installed on your system. The **easiest** way to build the executable is then by using the task 'fatJar' as follows:  
1) Cd into the PiBell-GUI directory:  
  `cd /home/your-username/PiBell/PiBell-GUI`  
2) Tell gradle to assemble a full 'fat' jar file that contains everything necessary to run the PiBell-GUI:  
  `gradle fatJar`
3) Gradle then builds the fat jar and copies it into the bin folder located in the PiBell-GUI directory. That's it.
4) Run the fatjar 'PiBell-GUI.jar' either by double click or via one of the provided script files as explained in the previous section.  

A second approach of building PiBell-GUI via gradle is to actually build it as an application:  
1) Again, Cd into the PiBell-GUI directory:  
  `cd /home/your-username/PiBell/PiBell-GUI`  
2) Tell gradle to build an application folder  
  `gradle installApp`  
3) The created PiBell-GUI application is located in the build/install folder and can be started via one of the scripts in its bin folder.


[1]: http://gradle.org/
