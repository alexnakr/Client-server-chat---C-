################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ChatClient.cpp \
../src/MThread.cpp \
../src/MessengerClient.cpp \
../src/TCPSocket.cpp \
../src/UDPSocket.cpp \
../src/main.cpp 

OBJS += \
./src/ChatClient.o \
./src/MThread.o \
./src/MessengerClient.o \
./src/TCPSocket.o \
./src/UDPSocket.o \
./src/main.o 

CPP_DEPS += \
./src/ChatClient.d \
./src/MThread.d \
./src/MessengerClient.d \
./src/TCPSocket.d \
./src/UDPSocket.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


