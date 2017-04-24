// Copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by zzu_softboy on 24/04/2017.

#ifndef LIBNOTIFY_GLOBAL_H
# include "notify/kernel/Global.h"
#endif

#ifndef NOTIFY_SYSTEMDETECTION_H
#define NOTIFY_SYSTEMDETECTION_H

/*
 * The operating system, must be one of: (NOTIFY_OS_x)
 *
 * DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
 * MACOS    - macOS
 * MSDOS    - MS-DOS and windows
 * OS2      - OS/2
 * OS2EMX   - XFree86 on OS/2
 * WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
 * CYGWIN   - Cygwin
 * SOLARIS  - Sun Solaris
 * HPUX     - HP-UX
 * ULTRIX   - DEC Ultrix
 * LINUX    - Linux
 * FREEBSD  - FreeBSD [has variants]
 * NETBSD   - NetBSD
 * OPENBSD  - OpenBSD
 * BSDI     - BSD/OS
 * INTERIX  - Interix
 * IRIX     - SGI Irix
 * OSF      - HP Tru64 UNIX
 * SCO      - SCO OpenServer 5
 * UNIXWARE - UnixWare 7, Open UNIX 8
 * AIX      - AIX
 * HURD     - GNU Hurd
 * DGUX     - DG/UX
 * RELIANT  - Reliant UNIX
 * DYNIX    - DYNIX/ptx
 * QNX      - QNX [has variants]
 * QNX6     - QNX RTP 6.1
 * LYNX     - LynxOS
 * BSD4     - Any BSD 4.4 system
 * UNIX     - Any UNIX BSD/SYSV system
 * HAIKU    - Haiku
 *
 * The following operating system have variants:
 *    FREEBSD  - NOTIFY_OS_FREEBSD is defined only when building for FreeBSD with a BSD userland
 *             - NOTIFY_OS_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
 */
#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#     define NOTIFY_OS_DARWIN
#     define NOTIFY_OS_BSD4
#     ifdef __LP64__
#        define NOTIFY_OS_DARWIN64
#     else
#        define NOTIFY_OS_DARWIN32
#     endif
#     ifndef TARGET_OS_IPHONE
#        define NOTIFY_OS_MACOS
#     endif
#  else
#     error "libnotify has not been ported to this Apple platform"
#  endif
#endif

#endif //NOTIFY_SYSTEMDETECTION_H
