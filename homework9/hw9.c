#include "parse_url.h"
#include <stdio.h>
#include <string.h>
// returns 1 if the url starts with http:// (case insensitive)
// and returns 0 otherwise
// If the url is a proper http scheme url, parse out the three
// components of the url: host, port and path and copy
// the parsed strings to the supplied destinations.
// Remember to terminate your strings with 0, the null terminator.
// If the port is missing in the url, copy "80" to port.
// If the path is missing in the url, copy "/" to path.
int parse_url(const char *url, char *host, char *port, char *path) {
  char dest[8];
  //positions for where port and path begins in url. Default to -1 to state no position found
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
    } else if (pathPos!= -1) {
      //no port found but a path was found
      strncpy(host, url + 7, pathPos - 7);
      host[pathPos - 7] = '\0';
      strcpy(port, "80");
      port[2] = '\0';
    } else {
      //no port or path found, rest of url is the host, port is "80", path is "/"
      strncpy(host, url + 7, strlen(url) - 7);
      strcpy(port, "80");
      strcpy(path, "/");
      host[strlen(url) - 7] = '\0'; 
      path[1] = '\0';
      port[2] = '\0';
    }
    if (pathPos != -1){
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
    } else if (portPos != -1) {
      //no path found, but a port was so the rest of url is the port and the path is "/"
      strncpy(port, url + portPos, strlen(url) - portPos);
      port[strlen(url) - portPos] = '\0';
      strcpy(path, "/");
      path[1] = '\0';
    }
    return 1;
  }
  return 0;
}
