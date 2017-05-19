# CardioEssential
This repo has the code for controlling the MCU on CardioWear. MCU listens to different commands through UART, and start sampling different signals.
command = "A" : sampling and transmitting ECG signal @ 200Hz
command = "B" : sampling and transmitting temperature signal 
command = "C" : stop sampling, enter idle mode
