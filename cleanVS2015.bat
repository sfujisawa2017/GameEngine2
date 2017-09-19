@echo off
echo VS2015の不要ファイルを削除します

call :delfiles

echo 削除が完了しました
pause

exit /b

:sub
cd %1
call :delfiles
cd ..
exit /b

:delfiles
if exist Debug rmdir /s /q Debug
if exist Release rmdir /s /q Release
if exist cModels rmdir /s /q cModels
if exist ipch rmdir /s /q ipch
if exist .vs rmdir /s /q .vs
if exist *.ncb del *.ncb
if exist *.suo del /A:H *.suo
if exist *.user del *.user
if exist *.sdf del *.sdf
if exist *.vcproj.* del *.vcproj*
if exist *.max del *.max
if exist *.WRL del *.WRL
if exist *.cmo del *.cmo
if exist *.dds del *.dds
if exist *.cso del *.cso
for /d %%i in (.\*) do  (	
	call :sub %%i
)

exit /b
