# URL Shortener ![C](https://img.shields.io/badge/-C-yellow) ![Database](https://img.shields.io/badge/-MongoDB-blue) ![Linux](https://img.shields.io/badge/Linux-Ubuntu-critical)

## Description
Program converts a long URL (original) to a shorter url. Using MongoDB atlas, we are able to request long url's given a key or convert url from long to short. 

## System DESIGN
----
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
---
### Second Phase:
- [ ] Add **pThreads**
- [ ] test threaded server
- [ ] Configure Parent Thread to distrubute loads
- [ ] test with big load ~100 queries / inserts
- [ ] Open url link (*optional*)
---
### Resources:
* [mongoc man page](https://s3.amazonaws.com/mciuploads/mongo-c-driver/man-pages/libbson/latest/index.html).<br />  
* [mongocDB c driver](http://mongoc.org/libmongoc/current/installing.html).
