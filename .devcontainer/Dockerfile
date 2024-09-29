FROM osrf/ros:noetic-desktop-full

SHELL ["/bin/bash", "-c"]

# Install misc tools
RUN apt-get update && apt-get install -y python3-catkin-tools python3-pip nautilus curl

# Install MRS packages
RUN curl https://ctu-mrs.github.io/ppa-stable/add_ppa.sh | bash
RUN apt install ros-noetic-mrs-uav-system-full -y

# Create user
RUN useradd -m -s /bin/bash mrs
RUN usermod -aG sudo mrs

# Allow the user to run sudo without a password
RUN echo "mrs ALL=(ALL) NOPASSWD:ALL" >>/etc/sudoers

# Add ROS source to bashrc
RUN echo "source /opt/ros/noetic/setup.bash" >>/home/mrs/.bashrc
RUN echo "source /home/mrs/workspace/devel/setup.bash" >>/home/mrs/.bashrc

USER mrs

WORKDIR /home/mrs/workspace

RUN catkin init

RUN catkin config --profile debug --cmake-args -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS='-std=c++17 -Og' -DCMAKE_C_FLAGS='-Og' \
  catkin config --profile release --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS='-std=c++17' \
  catkin config --profile reldeb --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS='-std=c++17' \
  catkin profile set reldeb

RUN echo "source /opt/ros/noetic/setup.bash" >>~/.bashrc

CMD ["bash"]
