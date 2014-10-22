
# MAKE file for IBM.MAK

# Revised:  1996-04-29

# Copyright ¸ 1987-1996  Prominare Inc.

# Macro definitions

C_SW=-G4e- -O+ -W1 -C


Text3D.DLL: Text3D.Obj Text3D.Res
 ILink /EXEPACK /ALIGN:4 /BASE:0x14000000 @IBM.Lnk
 Rc Text3D.Res Text3D.Dll

Text3D.Obj: Text3D.C
 Icc $(C_SW) -Fo$*.Obj Text3D.C

Text3D.Res: Text3D.Rc
 RC -r Text3D.Rc $*.Res

