mkdir build
arm-none-eabi-gcc -mthumb-interwork -specs=gba.specs src/main.s -o build/main.o
arm-none-eabi-objcopy -O binary build/main.o build/demo.gba
gbafix build/demo.gba
