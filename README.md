# Dron-Firefly

Development and programming of a drone, in a real time embedded system.

The drone consists of a raspberry pi and a navio.

The project is developed in cpp and consists of 4 modules:

  * Motors
  * Controller
  * PID
  * Sensors

We use the libraries provided by navio and in addition we use RTIMULIB to apply a kalman filter of the sensors.
