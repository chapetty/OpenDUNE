@REM Build OpenDUNE for DOS using WATCOM C
@MKDIR OBJS
@COPY Makefile.wc OBJS\MAKEFILE
@CD OBJS
@WMAKE
