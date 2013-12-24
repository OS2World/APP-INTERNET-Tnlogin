Replacement Telnet Login for IBM TCP/IP for OS/2
------------------------------------------------

This login enhancement can be used as an substitute for the login.exe
included with IBM TCP/IP for telnet logins. It consists of login.exe
(dummy, doesn't do anything), login2.exe (called by the telnet daemon)
which performs the actual login according to the passwd file and
telnetd2.cmd. Copy them into the \tcpip\bin directory and change the
telnet line in %etc%\inetd.lst to call telnetd2 instead of telnetd.
Also use telnetd2 to start the telnet, daemon if you do that manually,
instead of telnetd. If you accidentially still use telnetd and not
telnetd2, you cannot login to the system via telnet. There is also a
new passwd.exe program for managing the %etc%\passwd file. Some people
may find it easier to use than IBM's. In any case, it (and the login
replacement) should be compatible with the passwd file and passwd
program included with the TCP/IP 2.0 base kit CSD UN56401 and NFS kit
CSD UN57064 or higher.

This telnet login replacement does also work with TCP/IP 3.0 and newer
as included with Warp Connect, Warp Server/SMP and Warp 4.0. The only
difference is that you have to rename login.exe as tnlogin.exe to
replace the default tnlogin.exe. There is an APAR even for TCP/IP 2.0
that renames the default login.exe to tnlogin.exe and thus requires
you to do the same.

This version has the following advantages:

- uses per-user passwords, encrypted, in the passwd file
- different "home" directory supported for every user as specified in
  the passwd file 
- different "shell" program supported for every user as specified in
  the passwd file 
- sets the USER and LOGNAME environment variable with the login name
- sets the FULLNAME environment variable with the full name from the
  passwd file 
- sets the UNIX.UID and UNIX.GID environment variables
- sets the HOME environment variable
- sets the SHELL environment variable to the shell actually used
- does not allow login if no home directory and/or no shell are
  specified for a user (thus you can use the passwd file for all your
  PC-NFS users but allow only a few of them to login to the OS/2
  server, by providing this information or these few users only).
- for users not allowed to login as well as non-existent users the
  same error message is printed as if the password were wrong.
- the shell program field in the passwd file can have arguments
  (for example, a /k argument for cmd.exe, to have a kind of "autoexec")
- supports a \tcpip\etc\issue file (displayed before prompting for
  login)
- supports a \tcpip\etc\motd file (displayed after successful login,
  before shell program is started)
- 5 seconds delay after failed login attempts
- connection closed after 5 failed login attempts
- failed login attempts are logged in %etc%\login.bad
- successful logins/logouts are logged in %etc%\login.log

For the shell, you can use cmd.exe, 4os2.exe, ksh.exe, bash.exe etc. as
you like. You must specify full pathnames, though. Attention: for the 
colons required after the driver letter in full pathnames, use the 
semicolon (;) instead, because the colon (:) is the field separator in
passwd files. This is also compatible with IBM's above mentioned TCP/IP
CSD's which introduced a passwd program too.

The passwd file format in general is:

  user name:encrypted password:user id:group id:full name:home directory:shell

for example:

  forbes:pcVIHDf6s2cAy:22:17:Peter Forbes:d;/work:c;/os2/cmd.exe

The user and group ID's must be numerical, of course.

A big note to keep in mind: THIS SOFTWARE DOES NOT ADD ANY MORE *REAL*
SECURITY TO TELNET SERVICES PROVIDED BY AN OS/2 MACHINE, it just makes
telnet a bit easier to use and manage. Specifically, it does not add
file protection to telnet dial-in sessions.

Use on your own risk. All source code included except getopt.c was
written by me and is in the public domain.

To recompile, you need two things:

- IBM C Set++

  Well, some other compiler may do, but it was not tested.

- GNU ufc (ultra fast crypt) library

  This is available from sites in the US which archive GNU source code
  (look for GNU glibc) and from sites outside the US too. For example,
  you can find it on ftp.informatik.tu-muenchen.de in the file
  /pub/comp/os/os2/crypt/gnuufc.zip. It works unchanged, just unpack
  it into a directory ufc below the current  directory. GNU ufc was 
  written outside the US and can thus be used world wide, it just must
  not be downloaded from an US site by non-US user.

Kai Uwe Rommel

--
--
/* Kai Uwe Rommel      ARS Computer & Consulting GmbH, Muenchen, Germany *
 * rommel@ars.de             CompuServe 100265,2651, Fax +49 89 324 4524 *
 * rommel@leo.org (http://www.leo.org/pub/comp/os/os2 maintenance)       */

DOS ... is still a real mode only non-reentrant interrupt
handler, and always will be.                -Russell Williams


History:

06/20/94: initial release
09/03/94: bug fix for passwd.exe (use ; for : in home directory)
09/16/94: bug fix for ; in home directory, no longer endless login loop
09/18/94: replaced telnetd2.cmd with smarter REXX procedure
01/20/97: document change login.exe->tnlogin.exe
