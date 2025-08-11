################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/application/user_interface/ics/ICS2_RA6T3.o \
../src/application/user_interface/ics/ICS2_RA6T3_Built_in.o 

C_SRCS += \
../src/application/user_interface/ics/r_mtr_ics.c 

C_DEPS += \
./src/application/user_interface/ics/r_mtr_ics.d 

OBJS += \
./src/application/user_interface/ics/r_mtr_ics.o 

SREC += \
RA6T3_MCILV1_SPM_ENCD_FOC_TUNER_E2S_V101.srec 

MAP += \
RA6T3_MCILV1_SPM_ENCD_FOC_TUNER_E2S_V101.map 


# Each subdirectory must supply rules for building sources it contributes
src/application/user_interface/ics/%.o: ../src/application/user_interface/ics/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -gdwarf-4 -D_RA_CORE=CM33 -D_RENESAS_RA_ -D_RA_ORDINAL=1 -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner/rm_motor_driver" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/identification" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/tuner/rm_motor_speed" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc/api" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/fsp/inc/instances" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_gen" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_cfg/fsp_cfg/bsp" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra_cfg/fsp_cfg" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/main" -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/src/application/user_interface/ics" -I"." -I"D:/Work/0-Zanotic/Renesas/Sample Code/r01an6841xx0111-ra-encoder-foc/repo/RA6T3_Setup/ra/arm/CMSIS_6/CMSIS/Core/Include" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

