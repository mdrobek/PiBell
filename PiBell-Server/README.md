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
We're deploying the created web-application (war-file) with the help of gradle and the [gradle cargo plugin][2]. To do so, we first need to specify the deployment location, aka the server:  
1) Cd into the PiBell-Server folder  
  `cd /home/your-username/PiBell/PiBell-Server`  
2) Create a gradle.properties file with your favorite editor, e.g., vim  
  `vim gradle.properties`  
3) Add the following lines to this file and then save and exit your editor  
  `cargo.hostname=your-hostname.com`  
  `cargo.username=tomcat-admin-name`  
  `cargo.password=tomcat-admin-passwd**`  
  Where hostname is obviously the address of your server (localhost), username is the tomcat admin user as defined in the *tomcat-users.xml* file and password is the respective password (see [tomcat help page][3] for more information).  
4) The deploy process is then started via  
  `gradle deploy`  
  The command first builds the war file and afterwards uploads the war file to the server.  
  **NOTE:** The deploy process is currently limited to Tomcat 7 servers.
    
The restfull-services are then available via the following pattern:  
`http://your-domain-name:application-server-port/deployed-prefix/v1/rest/*`  
In case of tomcat on your localhost and no URI prefix, this becomes:  
`http://localhost:8080/v1/rest/*`  

[1]: http://gradle.org/
[2]: https://github.com/bmuschko/gradle-cargo-plugin
[3]: https://tomcat.apache.org/tomcat-7.0-doc/appdev/deployment.html#Deployment_With_Tomcat
