/* telnetd2.cmd - daemon / inetd entry to telnet login replacement */

/* Author:  Kai Uwe Rommel <rommel@ars.muc.de>
 * Created: Sun Sep 18 1994
 *
 * $Id: telnetd2.cmd,v 1.2 1997/01/20 13:24:55 rommel Exp $
 * $Revision: 1.2 $
 */

/*
 * $Log: telnetd2.cmd,v $
 * Revision 1.2  1997/01/20 13:24:55  rommel
 * %etc% does no longer point into \tcpip tree
 *
 * Revision 1.1  1994/09/18 19:43:25  rommel
 * Initial revision
 * 
 */

Call SetLocal
'@echo off'

tcpip = Value('ETC',,'OS2ENVIRONMENT')
tcpip = FileSpec('drive',tcpip) || '\tcpip'

'set telnet_comspec=%comspec%'
'set comspec='tcpip'bin\login2.exe'
'set ttysocket=%1'
'telnetd %1 %2 %3 %4 %5 %6 %7 %8 %9'

Call EndLocal 

/* end of telnetd2.cmd */
