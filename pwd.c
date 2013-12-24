/* pwd.c - /etc/passwd access library
 *
 * Author:  Kai Uwe Rommel <rommel@ars.muc.de>
 * Created: Mon Mar 28 1994
 */

static char *rcsid =
"$Id: pwd.c,v 1.1 1994/09/16 08:32:48 rommel Exp $";
static char *rcsrev = "$Revision: 1.1 $";

/*
 * $Log: pwd.c,v $
 * Revision 1.1  1994/09/16 08:32:48  rommel
 * Initial revision
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <pwd.h>

static struct passwd *getpwent(char *name, int uid)
{
  static struct passwd pw;
  static char buffer[256];
  char *ptr;
  FILE *passwd;
  int found = 0;

  if ((ptr = getenv("ETC")) == NULL)
    return NULL;

  strcpy(buffer, ptr);
  strcat(buffer, "/passwd");

  if ((passwd = fopen(buffer, "r")) == NULL)
    return NULL;

  while (!found && fgets(buffer, sizeof(buffer), passwd) != NULL)
  {
    if (buffer[strlen(buffer) - 1] == '\n')
      buffer[strlen(buffer) - 1] = 0;

    if (buffer[0] == '#')
      continue;

    pw.pw_dir = ""; /* these are optional */
    pw.pw_shell = "";

    pw.pw_name = buffer;

    if ((ptr = strchr(buffer, ':')) != NULL)
      *ptr++ = 0;
    else
      continue;

    pw.pw_passwd = ptr;

    if ((ptr = strchr(ptr, ':')) != NULL)
      *ptr++ = 0;
    else
      continue;

    pw.pw_uid = atoi(ptr);

    if ((ptr = strchr(ptr, ':')) != NULL)
      *ptr++ = 0;
    else
      continue;

    pw.pw_gid = atoi(ptr);

    if ((ptr = strchr(ptr, ':')) != NULL)
      *ptr++ = 0;
    else
      continue;

    if (name != NULL && stricmp(name, pw.pw_name) == 0 ||
	uid != -1 && uid == pw.pw_uid)
      found = 1;

    pw.pw_gecos = ptr;

    if ((ptr = strchr(ptr, ':')) != NULL)
      *ptr++ = 0;
    else
      continue;

    pw.pw_dir = ptr;

    if ((ptr = strchr(ptr, ':')) != NULL)
      *ptr++ = 0;
    else
      continue;

    pw.pw_shell = ptr;

    for (ptr = pw.pw_dir; (ptr = strchr(ptr, ';')) != NULL; *ptr = ':');
    for (ptr = pw.pw_shell; (ptr = strchr(ptr, ';')) != NULL; *ptr = ':');
  }

  fclose(passwd);

  return found ? &pw : NULL;
}

struct passwd *getpwnam(char *name)
{
  return getpwent(name, -1);
}

struct passwd *getpwuid(int uid)
{
  return getpwent(NULL, uid);
}

int setpwent(struct passwd *pw)
{
  char real[256], new[256], bak[256], line[256], buffer[256], newent[256];
  char *ptr, *name;
  FILE *passwd, *newpasswd;
  int found = 0;

  if ((ptr = getenv("ETC")) == NULL)
    return -1;

  strcpy(real, ptr);
  strcat(real, "/passwd");
  strcpy(new, real);
  strcat(new, ".new");
  strcpy(bak, real);
  strcat(bak, ".bak");

  if ((passwd = fopen(real, "r")) == NULL)
    /* ignore */;

  if ((newpasswd = fopen(new, "w")) == NULL)
    return -1;

  sprintf(newent, "%s:%s:%d:%d:%s:", pw->pw_name, pw->pw_passwd,
	  pw->pw_uid, pw->pw_gid, pw->pw_gecos);

  strcpy(line, pw->pw_dir);
  for (ptr = line; (ptr = strchr(ptr, ':')) != NULL; *ptr = ';');

  strcat(newent, line);
  strcat(newent, ":");

  strcpy(line, pw->pw_shell);
  for (ptr = line; (ptr = strchr(ptr, ':')) != NULL; *ptr = ';');

  strcat(newent, line);
  strcat(newent, "\n");

  while (passwd && fgets(buffer, sizeof(buffer), passwd) != NULL)
  {
    strcpy(line, buffer);

    if (buffer[strlen(buffer) - 1] == '\n')
      buffer[strlen(buffer) - 1] = 0;

    if (buffer[0] != '#')
    {
      name = buffer;

      if ((ptr = strchr(buffer, ':')) != NULL)
      {
	*ptr++ = 0;

	if (strcmp(pw->pw_name, name) == 0)
	{
	  found = 1;

	  if (pw->pw_uid != -1) /* that would mean 'delete' */
	    fputs(newent, newpasswd);

	  continue;
	}
      }
    }

    fputs(line, newpasswd);
  }

  if (!found && pw->pw_uid != -1) /* append new entry */
    fputs(newent, newpasswd);

  fclose(newpasswd);

  if (passwd)
    fclose(passwd);

  unlink(bak);
  rename(real, bak);

  if (rename(new, real))
    return -1;

  return 0;
}

/* end of pwd.c */
