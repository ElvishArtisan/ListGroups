/* listgroups_test.c
 *
 * Test the listgroups library.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <listgroups.h>

int main(int argc,char *argv[])
{
  int i;
  struct rd_group *grps=0;

  //
  // Call the function
  //
  int result=RD_ListGroups(&grps,"localhost","user","");
  if(result<0) {
    fprintf(stderr,"Something went wrong!\n");
    exit(256);
  }

  //
  // List the results
  //
  for(i=0;i<result;i++) {
    printf("%s:\n",grps[i].grp_name);
    printf("        Description: %s\n",grps[i].grp_desc);
    printf("  Default cart type: %u\n",grps[i].grp_default_cart_type);
    printf("           Low Cart: %06u\n",grps[i].grp_lo_limit);
    printf("          High Cart: %06u\n",grps[i].grp_hi_limit);
    printf("         Shelf Life: %d\n",grps[i].grp_shelf_life);
    printf("      Default Title: %s\n",grps[i].grp_default_title);
    printf(" Enforce Cart Range: %u\n",grps[i].grp_enforce_range);
    printf("   Incl. in Traffic: %u\n",grps[i].grp_report_tfc);
    printf("     Incl. in Music: %u\n",grps[i].grp_report_mus);
    printf("    Send Now & Next: %u\n",grps[i].grp_now_next);
    printf("              Color: %s\n",grps[i].grp_color);
    printf("\n");
  }

  //
  // Free the group list when finished with it
  //
  free(grps);

  exit(0);
}
