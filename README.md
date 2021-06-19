# Reading IBUS protocol with a Raspberry Pi Pico (RP2040)
This is an example of how to decode FlySky IBUS protocol using a Raspberry Pi Pico and Arduino libraries.

It uses hardware Serial1 to read from pin GPIO1 and displays the first 6 channels to the USB Serial

Prerequisites:
- Raspberry Pi Pico
- Arduino IDE
- Arduino MBED Core libraries for Raspberry Pi Pico
- FlySky transmitter (tested with FS-I6)
- FlySky receiver (tested with FS-IA6B)

Connections:
- from Pico VSYS (pin39) to receiver VCC
- from Pico GND (pin38) to receiver GND
- from receiver IBUS signal to Pico UART0 RX (pin2 - GPIO1)
