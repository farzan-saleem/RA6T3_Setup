################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/fsp/src/bsp/mcu/all/bsp_clocks.c \
../ra/fsp/src/bsp/mcu/all/bsp_common.c \
../ra/fsp/src/bsp/mcu/all/bsp_delay.c \
../ra/fsp/src/bsp/mcu/all/bsp_group_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_guard.c \
../ra/fsp/src/bsp/mcu/all/bsp_io.c \
../ra/fsp/src/bsp/mcu/all/bsp_irq.c \
../ra/fsp/src/bsp/mcu/all/bsp_macl.c \
../ra/fsp/src/bsp/mcu/all/bsp_register_protection.c \
../ra/fsp/src/bsp/mcu/all/bsp_rom_registers.c \
../ra/fsp/src/bsp/mcu/all/bsp_sbrk.c \
../ra/fsp/src/bsp/mcu/all/bsp_sdram.c \
../ra/fsp/src/bsp/mcu/all/bsp_security.c 

C_DEPS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.d \
./ra/fsp/src/bsp/mcu/all/bsp_common.d \
./ra/fsp/src/bsp/mcu/all/bsp_delay.d \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_guard.d \
./ra/fsp/src/bsp/mcu/all/bsp_io.d \
./ra/fsp/src/bsp/mcu/all/bsp_irq.d \
./ra/fsp/src/bsp/mcu/all/bsp_macl.d \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.d \
./ra/fsp/src/bsp/mcu/all/bsp_rom_registers.d \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.d \
./ra/fsp/src/bsp/mcu/all/bsp_sdram.d \
./ra/fsp/src/bsp/mcu/all/bsp_security.d 

OBJS += \
./ra/fsp/src/bsp/mcu/all/bsp_clocks.o \
./ra/fsp/src/bsp/mcu/all/bsp_common.o \
./ra/fsp/src/bsp/mcu/all/bsp_delay.o \
./ra/fsp/src/bsp/mcu/all/bsp_group_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_guard.o \
./ra/fsp/src/bsp/mcu/all/bsp_io.o \
./ra/fsp/src/bsp/mcu/all/bsp_irq.o \
./ra/fsp/src/bsp/mcu/all/bsp_macl.o \
./ra/fsp/src/bsp/mcu/all/bsp_register_protection.o \
./ra/fsp/src/bsp/mcu/all/bsp_rom_registers.o \
./ra/fsp/src/bsp/mcu/all/bsp_sbrk.o \
./ra/fsp/src/bsp/mcu/all/bsp_sdram.o \
./ra/fsp/src/bsp/mcu/all/bsp_security.o 

SREC += \
RA6T3_MCILV1_SPM_ENCD_FOC_TUNER_E2S_V101.srec 

MAP += \
RA6T3_MCILV1_SPM_ENCD_FOC_TUNER_E2S_V101.map 


# Each subdirectory must supply rules for building sources it contributes
ra/fsp/src/bsp/mcu/all/%.o: ../ra/fsp/src/bsp/mcu/all/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RA_CORE=CM33 -D_RENESAS_RA_ -D_RA_ORDINAL=1 -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner/rm_motor_driver" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/identification" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner/rm_motor_speed" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc/api" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc/instances" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_gen" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_cfg/fsp_cfg/bsp" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_cfg/fsp_cfg" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/main" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/user_interface/ics" -I"." -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

