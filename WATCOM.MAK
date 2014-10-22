
# MAKE file for WATCOM.MAK

# Revised:  1996-04-29

# Copyright ¸ 1987-1996  Prominare Inc.

# Macro definitions

C_SW=-ms -4s -bd -s -ox -zp4 -w1 

Text3D.Dll: Text3D.Obj Text3D.Res
 WLink @WATCOM.Lnk
 rc Text3D.Res Text3D.Dll

Text3D.Obj: Text3D.C
 Wcc386 $(C_SW) -fo=$*.Obj Text3D.C

Text3D.Res: Text3D.Rc
 RC -r Text3D.Rc $*.Res

