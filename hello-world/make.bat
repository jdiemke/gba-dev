mkdir build
arm-none-eabi-gcc -c src/main.cpp -mthumb-interwork -mthumb -O2 -o build/main.o
arm-none-eabi-gcc build/main.o -mthumb-interwork -mthumb -specs=gba.specs -o build/main.elf
arm-none-eabi-objcopy -v -O binary build/main.elf  build/main.gba
gbafix build/main.gba