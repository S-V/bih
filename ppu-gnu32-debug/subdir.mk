################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../bih.cpp \
../bihnode.cpp \
../color.cpp \
../face.cpp \
../image.cpp \
../main.cpp \
../object.cpp \
../ray.cpp \
../rect.cpp \
../scene.cpp \
../vec.cpp \
../vertex.cpp 

OBJS += \
./bih.o \
./bihnode.o \
./color.o \
./face.o \
./image.o \
./main.o \
./object.o \
./ray.o \
./rect.o \
./scene.o \
./vec.o \
./vertex.o 

CPP_DEPS += \
./bih.d \
./bihnode.d \
./color.d \
./face.d \
./image.d \
./main.d \
./object.d \
./ray.d \
./rect.d \
./scene.d \
./vec.d \
./vertex.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: PPU GNU 32 bit C++ Compiler with Debug Options'
	ppu-g++ -O0 -g3 -Wall -c -fmessage-length=0 -Winline -Wextra -fno-inline -m32 -mabi=altivec -maltivec -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


