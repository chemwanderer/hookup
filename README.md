'Hookup' is being developed as an easily extandable application for hardware control.<br />
The app's work is based on concurrency: all the processes, e. g. data exchange with external devices, auxiliary operations with the controlled parameters, GUI updating loop, are performed in their own threads synchronized via the central bank of data (see [engine/engine_terminal.h](https://github.com/chemwanderer/hookup/blob/main/engine/engine_terminal.h) & [engine/routine/engine_routine_base.h](https://github.com/chemwanderer/hookup/blob/main/engine/routine/engine_routine_base.h)).<br />
The following code includes some simple examples of implementation of tools for interaction with an external microcontroller ([engine/routine/mbed/engine_routine_mbed_server.h](https://github.com/chemwanderer/hookup/blob/main/engine/routine/mbed/engine_routine_mbed_server.h)), automated time-dependent change of a controlled parameter (e. g. voltage, see [engine/routine/inner/engine_routine_inner_oscillation.h](https://github.com/chemwanderer/hookup/blob/main/engine/routine/inner/engine_routine_inner_oscillation.h)) and remote access to an instance of this application on another PC ([engine/routine/udp/engine_routine_udp_client.h](https://github.com/chemwanderer/hookup/blob/main/engine/routine/udp/engine_routine_udp_client.h) & [engine/routine/udp/engine_routine_udp_server.h](https://github.com/chemwanderer/hookup/blob/main/engine/routine/udp/engine_routine_udp_server.h)).<br />
The configuration of the app is customizable. The list of controlled parameters and procedures used, as well as the structure of the graphical user interface, can be set up via input JSON files.<br />
