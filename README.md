# run-guit

A Qt-based GUI to view and manage runit services on Linux.
This project was built as a learning project to explore runit-based systems, service management, and basic GUI development.

## Features (Current)
- Lists services from /var/service
- Small and lightweight executable (~120KiB), build folder is (<1MiB)
- Built with Qt5
- Searchbar filtering
- Pkexec implementation (via run-guit-launcher.sh)
- Basic start/stop/restart functionality for detected services
- CTRL + R for refreshing

## Limitations
- No consistent live updates

## Future Plans
- Better security
- Improved UI
- Live Updates (perhaps via a config file to refresh every x seconds)
- Config file (for changing service directory and other future features) -> maybe settings menu

## Ambitious Future Plans
- Create a finished version
- Package for simple installation
- Widget implementation


## Current State
The program is able to list all services in "/var/service"
It can start, stop, and restart services that are detected as "Running" or "Stopped"
If the program is run without sudo, all services have a status of "Unknown"
It unforunately must be run in a terminal with either "sudo ./run-guit" or by using the included "run-guit-launcher.sh" script
