@echo off

echo ----------------------------------------------------
echo 清理这些垃圾东西
echo Press any key to delete all files with ending:
echo  *.aps *.idb *.ncp *.obj *.tmp *.sbr *.ipch *.tlog *.sdf *.tlh *.tli *.log
echo Visual c++/.Net junk 
echo ----------------------------------------------------
pause

echo del /F /Q /S *.aps *.idb *.ncp *.obj *.sbr *.tmp *.pdb *.bsc *.ilk *.res *.ncb *.opt *.suo *.manifest *.dep *.ipch *.tlog *.sdf *.tlh *.tli *.log *.lastbuildstate

del /F /Q /S /A *.tlog *.obj *.o.d *.tmp *.suo *.log ._.DS_Store .DS_Store ._* *.sdf vc100.* -H


pause


