Download: https://github.com/jorisdejong/Scheduler/releases

Scheduler is a helper app to trigger clips in Resolume at a specific time. You change the name of a clip in Resolume to the time you want it triggered. The app will keep a list of these clips. It will then use the system clock to trigger each clip at the right moment. At midnight, it will reset and start all over again.

It expects the following:
- you have Resolume's OSC input open at the default port 7000
- you rename the file as follows: hhmm. So to trigger it a two thirty in the afternoon, rename it to 1430
- you have the composition saved

It will not work when you don't use the correct renaming convention:
- already naming the actual file with the time will not work, it really needs to be renamed in Resolume
- using a colon (14:30) or any other extra character will not work
- using any extra number for days (01012019) or adding seconds (143010) will not work 
- using a non existent time (2569) will cause the universe to end

It assumes Resolume will correctly trigger the file. It does a basic check if the OSC message has been sent. but if some reason, the message does not arrive correctly in Resolume, it will not try to trigger it again.

When the list of files becomes too long, the last ones will fall off the screen. They should trigger correctly.

All potential problems are documented in the code with a 'todo' and can be tackled in a few minutes to a few hours depending on the problem. I've tried to comment as verbose as possible.

To build, the repo needs Juce and HybridAPI installed in the same root directory
- https://github.com/WeAreROLI/JUCE
- https://github.com/jorisdejong/HybridApi

Scheduler was kept alive by a kind donation from Life Is Art Studios - https://www.liastudios.com/






