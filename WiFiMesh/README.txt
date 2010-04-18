WiFi Mesh compilation guide
---------------------------

Compilation prerequisites
-------------------------
1. Visual Studio 2008
2. Qt for VS2008 4.6.0 or above (tested with 4.6.2)

Installing prerequisites
------------------------
1. Install Visual Studio 2008 (default installation settings are good enough)
2. Install Qt for VS2008 to some directory (e.g. C:\Qt\4.6.2)
3. Define environment variable named 'QtDir' with value of the directory above (e.g. C:\Qt\4.6.2).
   Please note, step 3 should be performed BEFORE RUNNING VISUAL STUDIO, since a process reads environment
   variables upon creation.
   
Compiling
---------
1. Open solution WiFiMesh.sln in VS2008
2. Build solution
