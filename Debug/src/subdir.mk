################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CommandBuilder.cpp \
../src/CommandBuilder_test.cpp \
../src/GetInfoCmd.cpp \
../src/GetInfoCmd_test.cpp \
../src/Main.cpp \
../src/PortInfo.cpp \
../src/ReceiveMessagesCmd.cpp \
../src/ReceiveMessagesCmd_test.cpp \
../src/RtMidiCommand.cpp \
../src/SetPortsCmd.cpp \
../src/StringTokenizer.cpp \
../src/TransmitMessageCmd.cpp \
../src/TransmitMessageCmd_test.cpp \
../src/midiprobe.cpp 

OBJS += \
./src/CommandBuilder.o \
./src/CommandBuilder_test.o \
./src/GetInfoCmd.o \
./src/GetInfoCmd_test.o \
./src/Main.o \
./src/PortInfo.o \
./src/ReceiveMessagesCmd.o \
./src/ReceiveMessagesCmd_test.o \
./src/RtMidiCommand.o \
./src/SetPortsCmd.o \
./src/StringTokenizer.o \
./src/TransmitMessageCmd.o \
./src/TransmitMessageCmd_test.o \
./src/midiprobe.o 

CPP_DEPS += \
./src/CommandBuilder.d \
./src/CommandBuilder_test.d \
./src/GetInfoCmd.d \
./src/GetInfoCmd_test.d \
./src/Main.d \
./src/PortInfo.d \
./src/ReceiveMessagesCmd.d \
./src/ReceiveMessagesCmd_test.d \
./src/RtMidiCommand.d \
./src/SetPortsCmd.d \
./src/StringTokenizer.d \
./src/TransmitMessageCmd.d \
./src/TransmitMessageCmd_test.d \
./src/midiprobe.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


