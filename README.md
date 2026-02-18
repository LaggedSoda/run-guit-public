# run-guit

A Qt-based GUI to view and manage runit services on Linux.
This project was built as a learning project to explore runit-based systems, service management, and basic GUI development.

## Features (Current)
- Lists services from /etc/sv
- Small and lightweight executable (~115KiB), build folder is (<1MiB)
- Built with Qt5
- Searchbar filtering
- Pkexec implementation
- Basic start/stop/restart functionality for detected services

## Limitations
- No consistent live updates

## Future Plans
- Better security
- Improved UI
- Live Updates
- Quick actions (keybinds)

## Ambitious Future Plans
- Create a finished version
- Package for simple installation
- Widget implementation


## Current State
The program is able to list all services in "/var/service"
It can start, stop, and restart services that are detected as "Running" or "Stopped"
If the program is run without sudo, all services have a status of "Unknown"
It unforunately must be run in a terminal with either "sudo ./run-guit" or by using the included "run-guit-launcher.sh" script
