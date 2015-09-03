/* listgroups.c
 *
 * Implementation of the ListGroups Rivendell Access Library
 *
 * (C) Copyright 2015 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2 as
 *   published by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "listgroups.h"

char groups[64000];
size_t total_size=0;

size_t __ListGroupsCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  memcpy(groups+total_size,ptr,size*nmemb);
  total_size+=size;

  return size*nmemb;
}

const char *RD_ListGroups(const char *hostname,const char *username,
			  const char *passwd)
{
  char post[1500];
  char url[1500];
  CURL *curl=NULL;

  memset(groups,0,sizeof(groups));
  snprintf(url,1500,"http://%s/rd-bin/rdxport.cgi",hostname);
  snprintf(post,1500,"COMMAND=4&LOGIN_NAME=%s&PASSWORD=%s",
	   username,passwd);
  printf("URL: %s\n",url);
  printf("POST: %s\n",post);
  if((curl=curl_easy_init())==NULL) {
    return 0;
  }
  curl_easy_setopt(curl,CURLOPT_WRITEDATA,&groups);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,__ListGroupsCallback);
  curl_easy_setopt(curl,CURLOPT_URL,url);
  curl_easy_setopt(curl,CURLOPT_POST,1);
  curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post);
  curl_easy_setopt(curl,CURLOPT_NOPROGRESS,1);
  curl_easy_setopt(curl,CURLOPT_VERBOSE,1);

  if(curl_easy_perform(curl)!=CURLE_OK) {
    return 0;
  }

  return groups;
}
