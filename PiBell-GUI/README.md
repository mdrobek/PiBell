# PiBell-GUI  
PiBell-GUI is a small prototypical java UI implementation that allows to ring the PiBell. It does that by sending a 'Ring' request to the PiBell-Server backend, which is then picked up by the PiBell-Pi client. I'm describing how to run the UI in the following paragraphs. However, I assume that you have checked out this git repository already. You could also simply copy the content of the *bin* folder.  

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
**NOTE:** Be aware that either script expects the previously checked *JAVA_HOME* to be set.


##### TODO's
Migrate this project into a Gradle project.
