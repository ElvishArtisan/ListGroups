/* listgroups.h
 *
 * Header for the ListGroups Rivendell Access Library
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

#ifndef LIST_GROUPS_H
#define LIST_GROUPS_H

struct rd_group {
  char grp_name[11];
  char grp_desc[256];
  unsigned grp_default_cart_type;
  unsigned grp_lo_limit;
  unsigned grp_hi_limit;
  int grp_shelf_life;
  char grp_default_title[256];
  int grp_enforce_range;
  int grp_report_tfc;
  int grp_report_mus;
  int grp_now_next;
  char grp_color[8];
  char grp_reserved[457];
};

int RD_ListGroups(struct rd_group *grps[],
		  const char *hostname,const char *username,const char *passwd);

#endif  // LIST_GROUPS_H
