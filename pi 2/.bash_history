ls
ls -l
ls
ls -la
cd
ls -l
ls -l .*
ls -l *
ls .*
more .bashrc
ls -l
rm .bash_history
ls
cd w8s2pi/
ls
vim w8s2p3-pi.cpp
exit
ls
cd w8s2pi/
cd
ls /dev/ttyUSB*
gcc w8s2p3-pi.cpp serial.cpp -pthread -o w8s2p3-pi
cd w8s2p3
cd w8s2p3/
ls
cd w8s2pi/
gcc w8s2p3-pi.cpp serial.cpp -pthread -o w8s2p3-pi
./w8s2p3-pi
gcc w8s2p3-pi.cpp serial.cpp -pthread -o w8s2p3-pi
./w8s2p3-pi
gcc w8s2p3-pi.cpp serial.cpp -pthread -o w8s2p3-pi
./w8s2p3-pi
EXIT
exit
sudo vi /etc/wpa_supplicant/wpa_supplicant.conf
sudo vi /etc/wpa_supplicant
sudo vi /etc/wpa_supplicant.conf
sudo vi /etc/wpa_supplicant/wpa_supplicant.conf
gcc Alex-pi.cpp serial.cpp serialize.cpp –pthread –o Alex-pi
ls
cd w8s2pi/
lsA
gcc Alex-pi.cpp serial.cpp serialize.cpp –pthread –o Alex-pi
cd w8s2p3/
ls
cd ..
cc Alex-pi.cpp serial.cpp serialize.cpp –pthread –o Alex-pi
gcc Alex-pi.cpp serial.cpp serialize.cpp –pthread –o Alex-pi
gcc Alex-pi.cpp serial.cpp serialize.cpp -pthread -o Alex-pi
ls
cd Alex/
ls
cd ..
cd w9s2/
ls
gcc Alex-pi.cpp serial.cpp serialize.cpp -pthread -o Alex-pi
ls
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o Alex-pi
./Alex-pi
cd w8s2pi/
cd w9s2/
ls
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o Alex-pi
./Alex-pi
ls
scp ~/alex.csr nic132000@DESKTOP-G93IV07:~
scp ~/alex.csr nic132000@172.17.194.73:~
scp ~/alex.csr ni132000@172.17.194.73:~
exit
ls
scp pi@172.17.194.73 ni132000@172.28.240.205:~
scp p ./alex.csr ni132000@172.28.240.205:~
scp ./alex.csr ni132000@172.28.240.205:~
scp ./alex.csr nic132000@172.28.240.205:~
scp ./alex.csr ni132000@172.28.240.205:~
exit
cd w8s2pi
ls
cd w9s2
gcc Alex-pi.cpp serial.cpp serialize.cpp -pthread -o Alex-pi
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
openssl version
openssl genrsa -out alex.key 4096
openssl req -new -key alex.key -out alex.csr
sudo apt-get install libssl-dev
ls
g++ test_tls_server.cpp tls_server.lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp -pthread -lssl -lcrypto -o test_tls_server
g++ test_tls_server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp -pthread -lssl -lcrypto -o test_tls_server
./test_tls_server
g++ test_tls_server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp -pthread -lssl -lcrypto -o test_tls_server
./test_tls_server
openssl genrsa -des3 -out signing.key 1024
openssl req -x509 -new -nodes -key signing.key -sha256 -days 1024 -out signing.pem
openssl genrsa -out alex.key 2048
openssl req -new -key alex.key -out alex.csr
openssl x509 -req -in alex.csr -CA signing.pem -CAkey signing.key -CAcreateserial -out alex.crt -days 500 -sha256
./test_tls_client
./test_tls_server 
g++ tls-alex-server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp serial.cpp serialize.cpp -pthread -lssl -lcrypto -o tls-alex-server 
./tls-alex-server
./tls-alex-server 
g++ tls-alex-server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp serial.cpp serialize.cpp -pthread -lssl -lcrypto -o tls-alex-server 
./tls-alex-server 
cd w8s2pi
cd w9s2
./Alex-pi
gcc Alex-pi.cpp
gcc alex-pi.cpp
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi 
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi 
hostname -I
./tls-alex-server
cd w8s2pi
ls
cd w9s2
ls
./ALex-pi
./Alex-pi
exit
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
cd w8s2pi/
cd w9s2/
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
gedit .bashrc
geany .bashrc
hostname -I
ifconfig
ifconfig
cd w8s2pi/
cd w9s2/
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
g++ tls-alex-server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp serial.cpp serialize.cpp -pthread -lssl -lcrypto -o tls-alex-server 
roscd
ls
cd
source /opt/ros/kinetic/setup.bash
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
roscd
source ~/.bashrc
source /opt/ros/kinetic/setup.bash
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
source export ~/.bashrc
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt install curl # if you haven't already installed curl
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
sudo apt install ros-noetic-desktop-full
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
cd w8s2pi/
cd w9s2/
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
cd w8s2pi/
cd w9s2/
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi
geany .bashrc
source .bashrc
geany .bashrc
roslaunch rplidar_ros rplidar.launch
ls
ls slam
ls Desktop/slam/devel
source ~/Desktop/slam/devel/setup.badh
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
geany .bashrc
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
geany .bashrc
./tls-alex-server 
cd w8s2pi/
cd w9s2/
gcc alex-pi.cpp serial.cpp serialize.cpp -pthread -o alex-pi
./alex-pi 
q
./alex-pi 
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
source .bashrc
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
./tls-alex-server 
cd
cd w8s2pi/
cd w9s2/
./alex-pi 
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
source .bashrc
source ~/Desktop/slam/devel/setup.bash
roslaunch rplidar_ros rplidar.launch
                   sudo apt-get update
sudo apt-get install realvnc-vnc-server
