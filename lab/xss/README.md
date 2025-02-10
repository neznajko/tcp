# XSS( Cross-Site Scripting )
Here I wanted to understand how *XSS* is working, to demonstrate
the problem *php* and *nodejs* are required:
 - start the vulnerable server in one terminal:
 ```bash
 php -S 0.0.0.0:8000
 ```
 - start the evil server in another terminal:
 ```bash
 node evil.js
 ```
 - visit the vulnerable page at http://localhost:8000/vul.php, there
 fill the form and submit, you'll be redirected to the Velcome Page:)

 - open another tab and visit the malicious page at http://localhost:8765
and click the link

 - You've been hacked:) look at the output of the evil server, it dumps
 your cookie token of the vulnerable site

