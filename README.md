# TCP Homework
Some examples from the book UNIX Network Programming, to compile a file called  
prognom.c type  
`TARGET=prognom make [phony]  `
Where phony targets are the following:  
- clean: remove object and executable files  
- etags\ctags: creates TAGS\tags file  
- debug: dumps additional information  
## daytime  
From the server side type:  
`sudo ./daytime_server`  
or whatever coz if you don't have root priviliges *bind* will repport an  
error and exit.  
From the client side:  
`./daytime_client <server address>  `
As they are the programs works only within AF_INET family so make sure  
you are using IPV4 addressing format:)  
