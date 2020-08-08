# URL Shortener ![C](https://img.shields.io/badge/-C-yellow) ![Database](https://img.shields.io/badge/-MongoDB-blue) ![Linux](https://img.shields.io/badge/Linux-Ubuntu-critical) [![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/Marko-Sanchez/url-shortener.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/Marko-Sanchez/url-shortener/context:cpp)

## Description
Program converts a long URL (original) to a shorter url. Using MongoDB atlas, we are able to request long url's given a key or convert url from long to short. Two programs are available a single thread ( main ) and multi-threaded program, either can be selected to run pogram with. 

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
= [x] Complete
---
### Second Phase:
- [x] Add **pThreads**
- [ ] Test threaded server
- [ ] Configure Parent Thread to distrubute loads
- [ ] test with big load ~100 queries / inserts
- [ ] Open url link (*optional*)
---
### Resources:
* [mongoc man page](https://s3.amazonaws.com/mciuploads/mongo-c-driver/man-pages/libbson/latest/index.html).<br />  
* [mongocDB c driver](http://mongoc.org/libmongoc/current/installing.html).
