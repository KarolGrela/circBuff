@ECHO OFF

::set input=%1
::@echo=%input%

set path=.\build\results\
set file=TestcircularBuffer_t.txt

::FOR /f "tokens=1,2,3,4 delims=:"  %%a IN (%path%%file%) DO

SETLOCAL EnableDelayedExpansion

set /A pass=0
set /A fail=0
set /A ignore = 0
set /A counter = 0

FOR /f "tokens=1,2,3,4 delims=:"  %%a IN (%path%%file%) DO (
    :: @echo %%d
    if "%%d"=="PASS" (SET /A pass+=1     && SET /A counter+=1)
    if "%%d"=="FAIL" (SET /A fail+=1     && SET /A counter+=1)
    if "%%d"=="IGNORE" (SET /A ignore+=1 && SET /A counter+=1)
)

@echo -------------
@echo TEST RESULTS for %file%:
@echo -------------
@echo [92m%pass%/%counter% TESTS PASS[0m
@echo [91m%fail%/%counter% TESTS FAIL[0m
@echo [93m%ignore%/%counter% TESTS are ignored[0m
