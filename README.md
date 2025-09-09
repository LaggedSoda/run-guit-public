# run-guit

A Qt-based GUI to view and manage runit services on Linux.
This project was built as a learning project to explore runit-based systems, service management, and basic GUI development.

## Features (Current)
- Lists services from /etc/sv
- Small and lightweight (~115KB)
- Built with Qt5
- Searchbar filtering
- Pkexec implementation

## Limitations
- No consistent live updates
- No start/stop/restart functionality

## Future Plans
- Implement features from the section above
- Better security
- Improved UI
- Live Updates

## Ambitious Future Plans
- Create a finished version
- Package for simple installation
- Widget implementation


## Current State
run-guit must be run in the terminal using "sudo ./run-guit"
The program is able to list all services in /etc/sv
It can stop and start services that are detected as "Running" or "Stopped". Stopping a tty service does update to "Stopped", but bluetoothd does not when disabled (visual bug). Stopping dbus causes desktop environment to vanish (as expected)
If run without sudo, all services have a status of "Unknown"
