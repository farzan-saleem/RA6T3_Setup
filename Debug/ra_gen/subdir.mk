################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/elc_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/elc_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/elc_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
RA6T3_MCILV1_SPM_ENCD_FOC_TUNER_E2S_V101.srec 

MAP += \
RA6T3_MCILV1_SPM_ENCD_FOC_TUNER_E2S_V101.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RA_CORE=CM33 -D_RENESAS_RA_ -D_RA_ORDINAL=1 -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner/rm_motor_driver" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/identification" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner/rm_motor_speed" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc/api" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc/instances" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_gen" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_cfg/fsp_cfg/bsp" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_cfg/fsp_cfg" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/main" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/user_interface/ics" -I"." -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

