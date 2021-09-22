/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* #undef _ALL_SOURCE */
#endif

/* Define if the closedir function returns void instead of int.  */
#define CLOSEDIR_VOID 1

/* Define to empty if the keyword does not work.  */
#define const 

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define if you support file names longer than 14 characters.  */
#define HAVE_LONG_FILE_NAMES 1

/* Define if your struct stat has st_blksize.  */
/* #undef HAVE_ST_BLKSIZE */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if utime(file, NULL) sets file's timestamp to the present.  */
#define HAVE_UTIME_NULL 1

/* Define if you have <vfork.h>.  */
/* #undef HAVE_VFORK_H */

/* Define if on MINIX.  */
/* #undef _MINIX */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef mode_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define if the system does not provide POSIX.1 features except
   with this defined.  */
#define _POSIX_1_SOURCE 2

/* Define if you need to in order for stat and other things to work.  */
#undef _POSIX_SOURCE
#define _POSIX_SOURCE 1

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if the `S_IS*' macros in <sys/stat.h> do not work properly.  */
/* #undef STAT_MACROS_BROKEN */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define vfork as fork if vfork does not work.  */
#define vfork fork

/* Define if you have MIT Kerberos version 4 available.  */
/* #undef HAVE_KERBEROS */

/* Define if you have GSSAPI with MIT Kerberos version 5 available.  */
/* #undef HAVE_GSSAPI */

/* Define if GSS_C_NT_HOSTBASED_SERVICE is defined in the gssapi.h
   header file.  Only relevant when using GSSAPI.  */
/* #undef HAVE_GSS_C_NT_HOSTBASED_SERVICE */

/* Define if you want CVS to be able to be a remote repository client.  */
#define CLIENT_SUPPORT 1

/* Define if you want CVS to be able to serve repositories to remote
   clients.  */
/* #undef SERVER_SUPPORT */

/* Define if you want to use the password authenticated server.  */
/* #undef AUTH_SERVER_SUPPORT */

/* Define if you want encryption support.  */
/* #undef ENCRYPTION */

/* Define if you have the connect function.  */
/* #undef HAVE_CONNECT */

/* Define if this system supports chown(), link(), and friends.  */
#define PRESERVE_PERMISSIONS_SUPPORT 1

/* Define if you have memchr (always for CVS).  */
#define HAVE_MEMCHR 1

/* Define if you have strchr (always for CVS).  */
#define HAVE_STRCHR 1

/* Define if utime requires write access to the file (true on Windows,
   but not Unix).  */
/* #undef UTIME_EXPECTS_WRITABLE */

/* Define if setmode is required when writing binary data to stdout.  */
/* #undef USE_SETMODE_STDOUT */

/* Define if the diff library should use setmode for binary files.
   FIXME: Why two different macros for setmode?  */
/* #undef HAVE_SETMODE */

/* Define if you have the crypt function.  */
/* #undef HAVE_CRYPT */

/* Define if you have the fchdir function.  */
/* #undef HAVE_FCHDIR */

/* Define if you have the fchmod function.  */
/* #undef HAVE_FCHMOD */

/* Define if you have the fsync function.  */
/* #undef HAVE_FSYNC */

/* Define if you have the ftime function.  */
/* #undef HAVE_FTIME */

/* Define if you have the ftruncate function.  */
/* #undef HAVE_FTRUNCATE */

/* Define if you have the getpagesize function.  */
/* #undef HAVE_GETPAGESIZE */

/* Define if you have the getspnam function.  */
/* #undef HAVE_GETSPNAM */

/* Define if you have the initgroups function.  */
/* #undef HAVE_INITGROUPS */

/* Define if you have the krb_get_err_text function.  */
/* #undef HAVE_KRB_GET_ERR_TEXT */

/* Define if you have the mktemp function.  */
#define HAVE_MKTEMP 1

/* Define if you have the putenv function.  */
#define HAVE_PUTENV 1

/* Define if you have the readlink function.  */
/* #undef HAVE_READLINK */

/* Define if you have the sigaction function.  */
#define HAVE_SIGACTION 1

/* Define if you have the sigblock function.  */
/* #undef HAVE_SIGBLOCK */

/* Define if you have the sigprocmask function.  */
/* #undef HAVE_SIGPROCMASK */

/* Define if you have the sigsetmask function.  */
/* #undef HAVE_SIGSETMASK */

/* Define if you have the sigvec function.  */
/* #undef HAVE_SIGVEC */

/* Define if you have the tempnam function.  */
/* #undef HAVE_TEMPNAM */

/* Define if you have the timezone function.  */
/* #undef HAVE_TIMEZONE */

/* Define if you have the tzset function.  */
/* #undef HAVE_TZSET */

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define if you have the wait3 function.  */
/* #undef HAVE_WAIT3 */

/* Define if you have the <direct.h> header file.  */
/* #undef HAVE_DIRECT_H */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <errno.h> header file.  */
#define HAVE_ERRNO_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <gssapi.h> header file.  */
/* #undef HAVE_GSSAPI_H */

/* Define if you have the <gssapi/gssapi.h> header file.  */
/* #undef HAVE_GSSAPI_GSSAPI_H */

/* Define if you have the <gssapi/gssapi_generic.h> header file.  */
/* #undef HAVE_GSSAPI_GSSAPI_GENERIC_H */

/* Define if you have the <io.h> header file.  */
/* #undef HAVE_IO_H */

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <memory.h> header file.  */
/* #undef HAVE_MEMORY_H */

/* Define if you have the <ndbm.h> header file.  */
/* #undef HAVE_NDBM_H */

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <sys/bsdtypes.h> header file.  */
/* #undef HAVE_SYS_BSDTYPES_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/file.h> header file.  */
/* #undef HAVE_SYS_FILE_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/resource.h> header file.  */
#define HAVE_SYS_RESOURCE_H 1

/* Define if you have the <sys/select.h> header file.  */
/* #undef HAVE_SYS_SELECT_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/timeb.h> header file.  */
/* #undef HAVE_SYS_TIMEB_H */

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <utime.h> header file.  */
#define HAVE_UTIME_H 1

/* Define if you have the crypt library (-lcrypt).  */
/* #undef HAVE_LIBCRYPT */

/* Define if you have the gen library (-lgen).  */
/* #undef HAVE_LIBGEN */

/* Define if you have the inet library (-linet).  */
/* #undef HAVE_LIBINET */

/* Define if you have the nsl library (-lnsl).  */
/* #undef HAVE_LIBNSL */

/* Define if you have the nsl_s library (-lnsl_s).  */
/* #undef HAVE_LIBNSL_S */

/* Define if you have the sec library (-lsec).  */
/* #undef HAVE_LIBSEC */

/* Define if you have the socket library (-lsocket).  */
/* #undef HAVE_LIBSOCKET */
