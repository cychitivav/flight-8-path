FROM osrf/ros:noetic-desktop-full

SHELL ["/bin/bash", "-c"]

# Install catkin tools
RUN apt-get update && \
  apt-get install python3-catkin-tools -y

RUN echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc

# Install pip tools 
RUN apt-get install -y python3-pip

# Install misc tools
RUN apt-get install -y nautilus curl

# Install MRS packages
RUN curl https://ctu-mrs.github.io/ppa-stable/add_ppa.sh | bash
RUN apt install ros-noetic-mrs-uav-system-full -y

WORKDIR /root/workspace

RUN catkin init

RUN catkin config --profile debug --cmake-args -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS='-std=c++17 -Og' -DCMAKE_C_FLAGS='-Og' \
  catkin config --profile release --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS='-std=c++17' \
  catkin config --profile reldeb --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS='-std=c++17' \
  catkin profile set reldeb  

CMD ["bash"]

# docker build -t flight-8-path:latest .
# export DISPLAY=$(ip route list default | awk '{print $3}'):0
# export LIBGL_ALWAYS_INDIRECT=1
# docker run -it --rm --name flight-8-path -v /tmp/.X11-unix:/tmp/.X11-unix -v $PWD:/root/catkin_ws/src/flight-8-path -e DISPLAY=$DISPLAY flight-8-path:latest