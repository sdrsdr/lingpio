# lingpio
sysfs based linux gpio "library" 

A "library" comprised from bits collected all over the web, structured to my liking and made to work together

The "upstream" documentation: https://www.kernel.org/doc/Documentation/gpio/

when compiling you can select the board with CFLAGS+=-D'GPIO_BOARD()=BOARD_XXXXX'  if board_XXXXX.h is provided 
and integrated in board.h. See board_chip.h for example.

