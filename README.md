# VGAansturing


## Description

This project contains the software to manage a VGA screen using an STM32F407G-disc microcontroller via UART.

### Dependencies

• STMCubeIDE version 1.19 or above

### Installing

To install, download the .zip file from the latest master branch and import the extracted folder as project into your STMCubeIDE.\
Flash your program to desired microcontroller and connect said controller to a VGA screen and a UART interpreter (e.g. Termite)

### Executing program

Send any commands as listed below:\
• lijn, x, y, x’, y’, kleur, dikte\
• rechthoek, x_lup, y_lup, breedte, hoogte, kleur, gevuld (1,0)\
• tekst, x, y, kleur, tekst, fontnaam (arial, consolas), fontgrootte (1,2), fontstijl (normaal, vet, cursief)\
• bitmap, nr, x-lup, y-lup \
• clearscherm, kleur\
• cirkel, x, y, radius, kleur\
• figuur, x1,y1, x2,y2, x3,y3, x4,y4, x5,y5, kleur\
(See doxygen documentation for specifics per command)

## Help

See doxygen documentation for a list of error types 

## Authors

Xander Perry\
Daniël Wit-Ariza\
Tom Veldkamp

## Version History

1.0

## License

This project is licensed under the Hogeschool Utrecht License

## Acknowledgments

Dominique Pizzie for the README template
