- [ ] Write C code to create an executable named (`cgrab`) that takes in four parameters, the name of the file, the hostname / IP address of the grab server, the port number for the grab server, and the authorization token to use.
+ You will likely want to use the `INFO` command first to help you out with knowing the size of the file that will be transferred.  It is possible to do it without it, just not advisable.
+ Remember the formatting of the packets and commands.
+ If the file is successfully downloaded, it should be saved to a sub-directory named `scans`. If needed, that directory should be created. In the event that the file already exists, it should be overwritten by default without prompting.

- [ ] Write a shell script (`batchgrab`) that does the following: (1) Opens a file (input argument) that contains the results of a `CLRTOSCAN` response, one file per line; (2) Iterate through each item and fetch each file.

- [ ] It is up to you if you want to make use of the `MD5` checksum to verify things.  It might not be a bad idea to do that manually to ensure that things have been properly downloaded.

- [ ] There are multiple example files for the shell script that are present in the class repository. You will likely need to modify them to use an appropriate port number for your group (see later) as well as potentially modify the IP addresses depending on where you are testing your client. The examples can be found in `cp1/part1/dispatch/clrtoscan`. Note that `set4.txt` adds in comments and one bad authorization into the mix.

- [ ]  Make sure to have an appropriate `.gitignore` to avoid including objects or the compiled binary.
