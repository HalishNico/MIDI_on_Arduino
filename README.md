# !Work in Progress!


# Purpose/Goal
To create code for computer and arduino to simulate key/touch responsive MIDI communication between a computer and arduino micro-controller over a serial port. 

# MIDI 

Midi communication is identical to 8N1 Serial communication with the exception that it uses a baudrate of 31250 and one complete MIDI message is three bytes sent through the line. These three individual bytes can be distinguished from one another by their first bit. The first byte, the status byte, holds the information for the pitch of the note and the following two data bytes contain information regarding the envelope/volume of the sound. 


# Computer/linux


# Arduino 
The code written for the micro-processor has largely been replicated from the SoftwareSerial.h library, the code must be modified to include the check and storage of 3 byte messages distinguished by an additional start bit instead of the one 1 byte message following 8N1 formatting. 
