# URL Shortener ![C](https://img.shields.io/badge/-C-yellow) ![Database](https://img.shields.io/badge/-MongoDB-blue) ![Linux](https://img.shields.io/badge/Linux-Ubuntu-critical) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Marko-Sanchez/url-shortener.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Marko-Sanchez/url-shortener/context:cpp)

## Description
Program converts a long URL (original) to a shorter url. Using MongoDB atlas, we are able to request long url's given a key or convert url from long to short. Two programs are available a single thread ( main ) and multi-threaded program, either can be selected to run the pogram with. 
## Installation and Running
To be able to run the program; The first step is installing a mongoc c driver. After installation, the database type used in this project is MongoDB Atlas.  <br />
Creating an account is necessary to access the DB, after fallowing the process outlined in the tutorial obtain you API key and input into Macro.  <br />
` #define API_KEY <string> `  <br />
Finally, in your terminal runing make, will create an executible ./url-shortener, prompting you to enter desired command.  <br />
You can dwell into the source code to be able to insert and query into your desired Collection.  <br />

**In Summary:**
1. Install mongoc driver.
2. Make an account on MongoDB.
3. after cloning, execute make.
4. Run desired commands
<br />
<b>Note:</b>
The single threaded program can be run simply by changing the name of it to url-shortner.c, while changing the multi-threaded program name to something else.  <br />

## OBJECTIVES
### Initial Task:
- [x] Choose Database
- [x] Configure for Vim
- [x] Connect to DB
- [x] Create Insert function
- [x] Create Query function
- [x] Write a Simple Hash Function
- [x] Complete
----
### First Phase:
- [x] Organize functions, into a seperate .c file
- [x] Build / prepare  dynamic user input
- [x] Multiples Test Uploading to DB 
- [x] Test Downloading from DB
- [x] Complete
---
### Second Phase:
- [x] Add **pThreads**
- [x] solve any memory issues
- [x] Test threaded server
- [x] Make client connection thread safe
- [x] Clean code
- [ ] test with big load ~100 queries / inserts
- [ ] Open url link (*optional*)
---
### Resources:
* [mongoc man page](https://s3.amazonaws.com/mciuploads/mongo-c-driver/man-pages/libbson/latest/index.html).<br />  
* [mongocDB c driver](http://mongoc.org/libmongoc/current/installing.html).
