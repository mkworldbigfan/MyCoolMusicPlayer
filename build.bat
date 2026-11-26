"C:\devkitPro\msys2\usr\bin\bash.exe" -lc "cd \"$(cygpath -u '%~dp0')\" && make %*"

if %ERRORLEVEL% equ 0 (
    echo.
  OK
    ) else (
    echo.
    echo ERR
)
endlocal
