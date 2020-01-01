#include <stdio.h>
#include <string.h>
#include "csapp.h"
#include "cache.h"

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

CacheList myCacheList;

int parse_url(const char *url, char *host, char *port, char *path){
  char dest[8];
  //positions for where port and path begin in url. Default to -1 to state no position found
  int portPos = -1;
  int pathPos = -1;
  int i;
  strncpy(dest, url, 8);
  dest[7] = '\0';
  if (strcasecmp(dest, "http://") == 0){
    //loop through string after http://
    for (i = 7; i < strlen(url); i++){
      if (url[i] == ':' && portPos == -1){
        portPos = i + 1;
      }  
      if (url[i] == '/' && pathPos == -1){
        pathPos = i;
        break;
      }
    }
    if (portPos != -1){
      //port found, so host is everything between http:// and the port
      strncpy(host, url + 7, portPos - 8);
      host[portPos - 8] = '\0';
    } else if (pathPos != -1){
      //no port found, but a path was found
      strncpy(host, url + 7, pathPos - 7);
      host[pathPos - 7] = '\0';
      strcpy(port, "80");
      port[2] = '\0';
    } else {
      //no port or path found, rest of the url is the host, port is "80", path is "/"
      strncpy(host, url + 7, strlen(url) - 7);
      strcpy(port, "80");
      strcpy(path, "/");
      host[strlen(url) - 7] = '\0';
      path[1] = '\0';
      path[2] = '\0';
    }
    if (pathPos != -1){
      //path found
      strncpy(path, url + pathPos, strlen(url) - pathPos);
      path[strlen(url) - pathPos] = '\0';
      if (portPos != -1){
        //path is rest of url, port is between host and path
        strncpy(port, url + portPos, pathPos - portPos);
        port[pathPos - portPos] = '\0';
      } else {
        //port doesn't exist, default to 80
        strcpy(port, "80");
        port[2] = '\0';
      }
    } else if (portPos != -1){
      strncpy(port, url + portPos, strlen(url) - portPos);
      port[strlen(url) - portPos] = '\0';
      strcpy(path, "/");
      path[1] = '\0';
    }
    return 1; 
  }
  return 0;
}


void doit(int fd, char *host, char *port, char *path){
  char buf[MAXLINE], buf_web[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE], content[MAXLINE];
  char headers[MAXLINE], firstLine[MAXLINE], type[MAXLINE], code[MAXLINE], status[MAXLINE];
  char lengthHeader[MAXLINE], lengthString[MAXLINE];
  int hostFound = 0;
  int lengthFound = 0;
  int noCache = 0;
  int size = 0;
  int fd_web;
  int n = 0;
  CachedItem *node;
  rio_t rio_web, rio_client;

  // Read request line and headers
  rio_readinitb(&rio_client, fd);
  if (!rio_readlineb(&rio_client, buf, MAXLINE)){
    //no request
    return;
  }
  
  //split first line into method, url and version
  //parse url of request
  sscanf(buf, "%s %s %s", method, uri, version);
  if (!parse_url(uri, host, port, path)){
    //failed parse, invalid url
    return;
  }

  //check to see if it is GET method, otherwise we cannot handle it
  if (strcasecmp(method, "GET")){
    //method is not GET
    return;
  }

  //do we have this cached?
  node = find(uri, &myCacheList);
  if (node != NULL){
    //load this in
    rio_writen(fd, node->item_p, node->size);
    return;
  }
  
  //create response headers
  sprintf(firstLine, "GET %s HTTP/1.0\r\n", path);
  while(rio_readlineb(&rio_client, buf, MAXLINE) > 2){
    printf("%s", buf);
    //iterate through all headers, buf is the header
    //skip over user-agent, connection, proxy-connection, if-modified-since, and if-none-match
    if (!!strncasecmp(buf, "User-Agent", 10) && !!strncasecmp(buf, "Connection", 10) && !!strncasecmp(buf, "Proxy-Connection", 16) && !!strncasecmp(buf, "If-Modified-Since", 18) && !!strncasecmp(buf, "If-None-Match", 13)){
      if (!strncasecmp(buf, "Host", 4)){
        hostFound = 1;
      }
      sprintf(headers, "%s%s", headers, buf);
    }
  }
  
  //open up the web file descriptor and begin reading into it
  fd_web = open_clientfd(host, port);
  rio_readinitb(&rio_web, fd_web);
  if (!hostFound){
    sprintf(headers, "%sHost: %s\r\n", headers, host);
  }
  sprintf(headers, "%sUser-Agent: %s", headers, user_agent_hdr);
  sprintf(headers, "%sProxy-Connection: close\r\n", headers);
  sprintf(headers, "%sConnection: close\r\n\r\n", headers);
  printf("\n\nHeaders: %s\n", headers);
  //forward these headers
  rio_writen(fd_web, firstLine, strlen(firstLine));
  rio_writen(fd_web, headers, strlen(headers));
  //headers have been forwarded with desired headers
  
  //begin reading response from server
  strcpy(content, "");
  rio_readinitb(&rio_web, fd_web);
  printf("\nResponse:\n");
  while((n = (rio_readlineb(&rio_web, buf_web, MAXLINE))) > 0){
    //add n to total size of request
    size += n;
    if (size <= MAX_OBJECT_SIZE){
      //add line to content
      strcat(content, buf_web);
      rio_writen(fd, buf_web, n);
    }
    //check if Content-Length exists 
    if (!strncasecmp(buf, "Content-Length", 14)){
      lengthFound = 1;
      sscanf(buf, "%s %s", lengthHeader, lengthString);
      printf("length: %d\n", atoi(lengthString));
    }
    //check for 200 OK
    if (!strncasecmp(buf_web, "HTTP", 4)){
      sscanf(buf, "%s %s %s", type, code, status);
      if (strncasecmp(code, "200", 3) || strncasecmp(status, "OK", 2)){
        noCache = 1;
      }      
    }
    printf("%s\n", buf_web);
  }
  printf("no cache: %d\n", noCache);
  printf("length found: %d\n", lengthFound);
  //end reading server response
  
  //check to see if 4 conditions for cache are met
  if (size > MAX_OBJECT_SIZE){
    noCache = 1;
  }
  if (!lengthFound){
    noCache = 1;
  }
  if (lengthFound && atoi(lengthString) != size){
    noCache = 1;
  }
  
  //read binary
  while ((n = rio_readnb(&rio_web, buf_web, MAXLINE)) != 0){
    rio_writen(fd_web, buf_web, size);
  }

  //does it satisfy the 4 constraints for adding to the cache?
  if (!noCache){
    //add to cache
    cache_URL(uri, headers, content, size, &myCacheList);
  }
}


int main(int argc, char **argv){
  cache_init(&myCacheList);
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE], path[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr; 
  Signal(SIGPIPE, SIG_IGN);
  listenfd = Open_listenfd(argv[1]);
  while (1){
    clientlen = sizeof(clientaddr);
    connfd = accept(listenfd, (SA *) &clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd, hostname, port, path);
    close(connfd);
  } 
  printf("%s", user_agent_hdr);
  return 0;
}
