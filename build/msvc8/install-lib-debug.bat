@echo off

echo Installing to %COINDIR%...

if not "%COINDIR%"=="" goto coindirset
echo The COINDIR environment variable must be set to point to a directory
echo to be able to perform the installation procedure.
exit
:coindirset
if exist %COINDIR%\*.* goto coindirexists
echo The COINDIR environment variable must point to an existing directory
echo to be able to perform the installation procedure.
exit
:coindirexists

pushd %COINDIR%

REM **********************************************************************
REM * Create all the directories for the installed files

if exist bin\*.* goto binexists
echo mkdir %COINDIR%\bin
mkdir bin
:binexists
if exist data\*.* goto dataexists
echo mkdir %COINDIR%\data
mkdir data
:dataexists
chdir data
if exist draggerDefaults\*.* goto draggerdefaultsexists
echo mkdir %COINDIR%\data\draggerDefaults
mkdir draggerDefaults
:draggerdefaultsexists
chdir ..
if exist lib\*.* goto libexists
echo mkdir %COINDIR%\lib
mkdir lib
:libexists
if exist include\*.* goto includeexists
echo mkdir %COINDIR%\include
mkdir include
:includeexists
chdir include
if exist Inventor\*.* goto inventorexists
echo mkdir %COINDIR%\include\Inventor
mkdir Inventor
:inventorexists
chdir Inventor
if exist C\*.* goto cexists
echo mkdir %COINDIR%\include\Inventor\C
mkdir C
:cexists
chdir C
if exist base\*.* goto cbaseexists
echo mkdir %COINDIR%\include\Inventor\C\base
mkdir base
:cbaseexists
if exist errors\*.* goto cerrorsexists
echo mkdir %COINDIR%\include\Inventor\C\errors
mkdir errors
:cerrorsexists
if exist glue\*.* goto cglueexists
echo mkdir %COINDIR%\include\Inventor\C\glue
mkdir glue
:cglueexists
if exist threads\*.* goto cthreadsexists
echo mkdir %COINDIR%\include\Inventor\C\threads
mkdir threads
:cthreadsexists
chdir ..
if exist MPEG\*.* goto mpegexists
echo mkdir %COINDIR%\include\Inventor\MPEG
mkdir MPEG
:mpegexists
if exist VRMLnodes\*.* goto vrmlnodesexists
echo mkdir %COINDIR%\include\Inventor\VRMLnodes
mkdir VRMLnodes
:vrmlnodesexists
if exist actions\*.* goto actionsexists
echo mkdir %COINDIR%\include\Inventor\actions
mkdir actions
:actionsexists
if exist annex\*.* goto annexexists
echo mkdir %COINDIR%\include\Inventor\annex
mkdir annex
:annexexists
chdir annex
if exist HardCopy\*.* goto hardcopyexists
echo mkdir %COINDIR%\include\Inventor\annex\HardCopy
mkdir HardCopy
:hardcopyexists
if exist ForeignFiles\*.* goto foreignfilesexists
echo mkdir %COINDIR%\include\Inventor\annex\ForeignFiles
mkdir ForeignFiles
:foreignfilesexists
chdir ..
if exist bundles\*.* goto bundlesexists
echo mkdir %COINDIR%\include\Inventor\bundles
mkdir bundles
:bundlesexists
if exist caches\*.* goto cachesexists
echo mkdir %COINDIR%\include\Inventor\caches
mkdir caches
:cachesexists
if exist collision\*.* goto collisionexists
echo mkdir %COINDIR%\include\Inventor\collision
mkdir collision
:collisionexists
if exist details\*.* goto detailsexists
echo mkdir %COINDIR%\include\Inventor\details
mkdir details
:detailsexists
if exist draggers\*.* goto draggersexists
echo mkdir %COINDIR%\include\Inventor\draggers
mkdir draggers
:draggersexists
if exist elements\*.* goto elementsexists
echo mkdir %COINDIR%\include\Inventor\elements
mkdir elements
:elementsexists
if exist engines\*.* goto enginesexists
echo mkdir %COINDIR%\include\Inventor\engines
mkdir engines
:enginesexists
if exist errors\*.* goto errorsexists
echo mkdir %COINDIR%\include\Inventor\errors
mkdir errors
:errorsexists
if exist events\*.* goto eventsexists
echo mkdir %COINDIR%\include\Inventor\events
mkdir events
:eventsexists
if exist fields\*.* goto fieldsexists
echo mkdir %COINDIR%\include\Inventor\fields
mkdir fields
:fieldsexists
if exist lists\*.* goto listsexists
echo mkdir %COINDIR%\include\Inventor\lists
mkdir lists
:listsexists
if exist lock\*.* goto lockexists
echo mkdir %COINDIR%\include\Inventor\lock
mkdir lock
:lockexists
if exist manips\*.* goto manipsexists
echo mkdir %COINDIR%\include\Inventor\manips
mkdir manips
:manipsexists
if exist misc\*.* goto miscexists
echo mkdir %COINDIR%\include\Inventor\misc
mkdir misc
:miscexists
if exist nodekits\*.* goto nodekitsexists
echo mkdir %COINDIR%\include\Inventor\nodekits
mkdir nodekits
:nodekitsexists
if exist nodes\*.* goto nodesexists
echo mkdir %COINDIR%\include\Inventor\nodes
mkdir nodes
:nodesexists
if exist projectors\*.* goto projectorsexists
echo mkdir %COINDIR%\include\Inventor\projectors
mkdir projectors
:projectorsexists
if exist sensors\*.* goto sensorsexists
echo mkdir %COINDIR%\include\Inventor\sensors
mkdir sensors
:sensorsexists
if exist system\*.* goto systemexists
echo mkdir %COINDIR%\include\Inventor\system
mkdir system
:systemexists
if exist threads\*.* goto threadsexists
echo mkdir %COINDIR%\include\Inventor\threads
mkdir threads
:threadsexists

popd

REM **********************************************************************
REM * Copy files

echo Installing header files...
call install-headers.bat

echo Installing data files...
xcopy ..\..\data\draggerDefaults\*.iv %COINDIR%\data\draggerDefaults\ /R /Y

echo Installing binaries...
xcopy coin3sd.lib %COINDIR%\lib\ /R /Y

echo Done.

