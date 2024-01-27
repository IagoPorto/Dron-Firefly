#!/bin/bash

../build/kalmanFilter &
../build/controller &
../build/pid_process &
sudo ../build/motors &
