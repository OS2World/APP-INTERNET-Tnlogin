/* login2.c - "login" program using /etc/passwd
 *
 * Author:  Kai Uwe Rommel <rommel@ars.muc.de>
 * Created: Mon Mar 28 1994
 */

static char *rcsid =
"$Id: login2.c,v 1.2 1994/09/18 19:42:21 rommel Exp rommel $";
static char *rcsrev = "$Revision: 1.2 $";

/*
 * $Log: login2.c,v $
 * Revision 1.2  1994/09/18 19:42:21  rommel
 * forbid calling the program directly
 *
 * Revision 1.1  1994/09/16 08:32:48  rommel
 * Initial revision
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <process.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>
#include <time.h>

extern char *crypt(char *, char*);

extern char *getpass(char *);
char *getline(const char *prompt);

int log(int error, char *message, char *arg)
{
  char buffer[256], *ptr;
  FILE *file;
  time_t now;
  struct tm *tm;

  if ((ptr = getenv("ETC")) == NULL)
    return -1;

  strcpy(buffer, ptr);
  strcat(buffer, "/");
  strcat(buffer, error ? "login.bad" : "login.log");

  if ((file = fopen(buffer, "a")) == NULL)
    return -1;

  time(&now);
  tm = localtime(&now);
  strftime(buffer, sizeof(buffer), "%d/%m/%y %H:%M:%S", tm);

  fprintf(file, "%s: %s: %s\n", buffer, message, arg);

  fclose(file);

  return 0;
}

int showfile(char *name)
{
  char buffer[256], *ptr;
  FILE *file;

  if ((ptr = getenv("ETC")) == NULL)
    return -1;

  strcpy(buffer, ptr);
  strcat(buffer, "/");
  strcat(buffer, name);

  if ((file = fopen(buffer, "r")) == NULL)
    return -1;

  while (fgets(buffer, sizeof(buffer), file) != NULL)
    fputs(buffer, stdout);

  fclose(file);

  return 0;
}

void session(struct passwd *pw)
{
  char user[64], logname[64], fullname[256], uid[32], gid[32], home[256];
  char shell[256], cmd[512], *args;

  if (showfile("motd"))
    printf("login successful.\n");

  sprintf(user, "USER=%s", pw->pw_name);
  putenv(user);
  sprintf(logname, "LOGNAME=%s", pw->pw_name);
  putenv(logname);
  sprintf(fullname, "FULLNAME=%s", pw->pw_gecos);
  putenv(fullname);

  sprintf(uid, "UNIX.UID=%d", pw->pw_uid);
  putenv(uid);
  sprintf(gid, "UNIX.GID=%d", pw->pw_gid);
  putenv(gid);

  sprintf(home, "HOME=%s", pw->pw_dir);
  putenv(home);

  sprintf(shell, "SHELL=%s", pw->pw_shell);
  putenv(shell);

  if (isalpha(pw->pw_dir[0]) && pw->pw_dir[1] == ':')
    _chdrive(toupper(pw->pw_dir[0]) - 'A' + 1);
  _chdir(pw->pw_dir);

  strcpy(cmd, pw->pw_shell);
  if ((args = strchr(cmd, ' ')) != NULL)
    *args++ = 0;

  spawnle(P_WAIT, cmd, cmd,
	  /* bug in IBM C Set++: */ /*args ? cmd : NULL, */
	  args, NULL, environ);

  putenv("USER");
  putenv("LOGNAME");
  putenv("FULLNAME");
  putenv("UNIX.UID");
  putenv("UNIX.GID");
  putenv("HOME");
  putenv("SHELL");
}

void ignore(int sig)
{
  signal(SIGINT, ignore);
  signal(SIGBREAK, ignore);
  msleep(5 * 1000);
}

int main(int argc, char **argv)
{
  struct passwd *pw;
  char user[64], password[64], crypted[64], *cp;
  static char buffer[256];
  int cnt, failed = 0;

  signal(SIGINT, ignore);
  signal(SIGBREAK, ignore);

  if ((cp = getenv("TELNET_COMSPEC")) == NULL)
  {
    printf("\nThis program cannot be called directly, only by the telnet daemon.\n");
    exit(1);
  }

  sprintf(buffer, "COMSPEC=%s", cp);
  putenv(buffer);
  putenv("TELNET_COMSPEC=");

  log(0, "telnet login started", "");

  for (;;)
  {
    if (showfile("issue"))
      printf("\nOS/2 %d.%02d (%s)\n", 
	     _osmajor / 10, _osminor, getenv("HOSTNAME"));

    for (;;)
    {
      strcpy(user, getline("login: "));
      strcpy(password, getpass("password: "));
      failed++;

      if ((pw = getpwnam(user)) == NULL)
      {
	memset(password, 0, sizeof(password));
	printf("login failed.\n\n");
	log(1, "no such user", user);
      }
      else
      {
	strcpy(crypted, crypt(password, pw->pw_passwd));
	memset(password, 0, sizeof(password));

	if (strcmp(pw->pw_passwd, crypted) != 0)
	{
	  printf("login failed.\n\n");
	  log(1, "wrong password for user", user);
	}
	else if (strlen(pw->pw_dir) == 0 || strlen(pw->pw_shell) == 0)
	{
	  printf("login failed.\n\n");
	  log(1, "login not permitted", user);
	}
	else
	{
	  log(0, "user login", user);
	  session(pw);
	  log(0, "user logout", user);
	  failed = 0;
	  return 0;
	}
      }

      if (failed >= 5)
      {
	log(1, "telnet login aborted after maximum number of failed login attempts", "");
	return 1;
      }

      msleep(5 * 1000);
    }
  }
  
  return 0;
}

/* end of login.c */
