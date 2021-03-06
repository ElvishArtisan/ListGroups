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
#include <expat.h>

#include "listgroups.h"

struct xml_data {
  unsigned grps_quan;
  char elem_name[256];
  char strbuf[1024];
  struct rd_group *grps;
};


unsigned __ListGroupReadBool(const char *val)
{
  if((strcasecmp(val,"true")==0)||(strcasecmp(val,"yes")==0)||
     (strcasecmp(val,"on")==0)) {
    return 1;
  }
  else {
    return 0;
  }
}

static void XMLCALL __ListGroupsElementStart(void *data, const char *el, 
					     const char **attr)
{
  unsigned i;
  struct xml_data *xml_data=(struct xml_data *)data;
  if(strcasecmp(el,"group")==0) {    // Allocate a new group entry
    xml_data->grps=realloc(xml_data->grps,
			   (xml_data->grps_quan+1)*sizeof(struct rd_group));
    (xml_data->grps_quan)++;
  }
  strncpy(xml_data->elem_name,el,256);
  memset(xml_data->strbuf,0,1024);
}


static void XMLCALL __ListGroupsElementData(void *data,const XML_Char *s,
					    int len)
{
  struct xml_data *xml_data=(struct xml_data *)data;

  memcpy(xml_data->strbuf+strlen(xml_data->strbuf),s,len);
}


static void XMLCALL __ListGroupsElementEnd(void *data, const char *el)
{
  struct xml_data *xml_data=(struct xml_data *)data;
  struct rd_group *grp=xml_data->grps+(xml_data->grps_quan-1);

  if(strcasecmp(el,"name")==0) {
    strncpy(grp->grp_name,xml_data->strbuf,10);
  }
  if(strcasecmp(el,"description")==0) {
    strncpy(grp->grp_desc,xml_data->strbuf,256);
  }
  if(strcasecmp(el,"defaultcarttype")==0) {
    if(strcasecmp(xml_data->strbuf,"audio")==0) {
      grp->grp_default_cart_type=0;
    }
    if(strcasecmp(xml_data->strbuf,"macro")==0) {
      grp->grp_default_cart_type=1;
    }
  }
  if(strcasecmp(el,"defaultlowcart")==0) {
    sscanf(xml_data->strbuf,"%u",&grp->grp_lo_limit);
  }
  if(strcasecmp(el,"defaulthighcart")==0) {
    sscanf(xml_data->strbuf,"%u",&grp->grp_hi_limit);
  }
  if(strcasecmp(el,"cutshelflife")==0) {
    sscanf(xml_data->strbuf,"%d",&grp->grp_shelf_life);
  }
  if(strcasecmp(el,"defaulttitle")==0) {
    strncpy(grp->grp_default_title,xml_data->strbuf,256);
  }
  if(strcasecmp(el,"enforcecartrange")==0) {
    grp->grp_enforce_range=__ListGroupReadBool(xml_data->strbuf);
  }
  if(strcasecmp(el,"reporttfc")==0) {
    grp->grp_report_tfc=__ListGroupReadBool(xml_data->strbuf);
  }
  if(strcasecmp(el,"reportmus")==0) {
    grp->grp_report_mus=__ListGroupReadBool(xml_data->strbuf);
  }
  if(strcasecmp(el,"color")==0) {
    strncpy(grp->grp_color,xml_data->strbuf,8);
  }
}


size_t __ListGroupsCallback(void *ptr, size_t size, size_t nmemb, void *userdata)
{
  XML_Parser p=(XML_Parser)userdata;

  XML_Parse(p,ptr,size*nmemb,0);
  
  return size*nmemb;
}


int RD_ListGroups(struct rd_group *grps[],
		  const char *hostname,const char *username,const char *passwd)
{
  char post[1500];
  char url[1500];
  CURL *curl=NULL;
  XML_Parser parser;
  struct xml_data xml_data;
  struct rd_group groups[20];
  unsigned i;

  /*
   * Setup the CURL call
   */
  memset(&xml_data,0,sizeof(xml_data));
  parser=XML_ParserCreate(NULL);
  XML_SetUserData(parser,&xml_data);
  XML_SetElementHandler(parser,__ListGroupsElementStart,
			__ListGroupsElementEnd);
  XML_SetCharacterDataHandler(parser,__ListGroupsElementData);
  snprintf(url,1500,"http://%s/rd-bin/rdxport.cgi",hostname);
  snprintf(post,1500,"COMMAND=4&LOGIN_NAME=%s&PASSWORD=%s",
	   username,passwd);
  if((curl=curl_easy_init())==NULL) {
    return -1;
  }
  curl_easy_setopt(curl,CURLOPT_WRITEDATA,parser);
  curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,__ListGroupsCallback);
  curl_easy_setopt(curl,CURLOPT_URL,url);
  curl_easy_setopt(curl,CURLOPT_POST,1);
  curl_easy_setopt(curl,CURLOPT_POSTFIELDS,post);
  curl_easy_setopt(curl,CURLOPT_NOPROGRESS,1);
  //  curl_easy_setopt(curl,CURLOPT_VERBOSE,1);
  if(curl_easy_perform(curl)!=CURLE_OK) {
    return -1;
  }

  *grps=xml_data.grps;
  return xml_data.grps_quan;
}
