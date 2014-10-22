
# MAKE file for Borland.MAK

# Revised:  1996-04-29

# Copyright ¸ 1987-1996  Prominare Inc.

# Macro definitions

C_SW=-sd -Ox -v- -w -c 


Text3D.DLL: Text3D.Obj Text3D.Res
 TLink @OS2PM_2x.Lnk
 rc Text3D.Res Text3D.Dll

Text3D.Obj: Text3D.C
 Bcc $(C_SW) -o$*.Obj Text3D.C

Text3D.Res: Text3D.Rc
 RC -r Text3D.Rc $*.Res

