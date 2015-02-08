# PiBell-Server  
PiBell-Server is the J2EE rest-full backend application that accepts and caches 'Ring' requests from the PiBell-GUI application and serves them to the PiBell-Pi tool on the Raspberry Pi whenever requested.

#### PiBell-Server Dependencies
This project is maintained via gradle, which makes it very easy to build and deploy it. However, the following tools and frameworks need to be installed on your build machine:  
1) [Gradle][1] (at least version 2.x)  
2) Java 8  
3) Tomcat 7/8 (every other Java servlet container should also suffice, e.g., jetty)  

#### Building PiBell-Server
Once these dependencies have been installed, gradle should be added to your local path so that it is accessible from a command line or shell. I also assume that you've already checked out a copy of this repository. The following steps are then necessary to actually build PiBell-Server:  
1) Navigate to your git PiBell-Server folder, e.g., via  
`cd /home/your-username/PiBell/PiBell-Server`  
2) Run the gradle build process  
`gradle build`  
Gradle will then take care of downloading all necessary dependencies to compile the PiBell-Server war archive. The final created *war* file can then be found in the build/libs folder.  
**NOTE:** Gradle needs internet access to download all necessary dependencies.

#### Deploy PiBell-Server
For now, the easiest way to deploy the war file to your application server is simply to copy over the created war file from build/libs into the application servers deployment folder, e.g., in tomcat this would be *$CATALINA_BASE/webapps/* (see [tomcat help page][2] for more information).  
The restfull-services are then available via the following pattern:  
`http://your-domain-name:application-server-port/deployed-prefix/v1/rest/*`  
In case of tomcat on your localhost and no URI prefix, this becomes:  
`http://localhost:8080/v1/rest/*`  

**TODO** Include deployement of war file into gradle process.

[1]: http://gradle.org/
[2]: https://tomcat.apache.org/tomcat-7.0-doc/appdev/deployment.html#Deployment_With_Tomcat
