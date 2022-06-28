cmake_minimum_required(VERSION 3.16.3)
project(bsy_p)

set(CMAKE_CXX_STANDARD 20)

add_executable(bsy_p modules/UltrasonicSensor.h modules/UltrasonicSensor.cpp modules/StepperMotor.cpp modules/StepperMotor.h modules/LEDMatrix.cpp modules/LEDMatrix.h main.cpp Semaphore.h)
target_link_libraries(bsy_p wiringPi pthread)