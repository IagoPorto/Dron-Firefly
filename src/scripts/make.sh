cd ..
rm -rf build
mkdir build
cd build
cmake ..
make motors pid_process controller sensors kalmanFilter changePIDinFlightTime
cp ../sensor/RTIMULib.ini ./
