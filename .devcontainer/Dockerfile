FROM ros:melodic

# non-root user info
ARG USERNAME=guest
ARG UID=1000
ARG GID=$UID

# dependencies
ARG PKGS="python-catkin-tools iputils-ping net-tools htop psmisc ros-$ROS_DISTRO-rqt ros-$ROS_DISTRO-rqt-common-plugins ros-$ROS_DISTRO-rqt-robot-plugins"

# install dependencies and clean up afterwards
RUN apt-get update \
	&& apt-get -y install --no-install-recommends $PKGS \
	&& apt-get -y autoremove \
	&& apt-get -y clean \
	&& rm -rf /var/lib/apt/lists/*

# adapted from https://code.visualstudio.com/docs/remote/containers-advanced
RUN groupadd --gid $GID $USERNAME \
	&& useradd --uid $UID --gid $GID --create-home --shell /bin/bash $USERNAME \
	&& echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
	&& chmod 0440 /etc/sudoers.d/$USERNAME

# swap to non-root user
USER $USERNAME
WORKDIR /home/$USERNAME
ENV WS_PATH=/home/guest/iw_ws

# auto source ros stuff
RUN echo "source /opt/ros/$ROS_DISTRO/setup.bash" >> ~/.bashrc
# lazy
RUN rosdep update
RUN echo 'test -f $WS_PATH/devel/setup.bash && source $WS_PATH/devel/setup.bash' >> ~/.bashrc
RUN sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
#RUN curl -sSL 'http://keyserver.ubuntu.com/pks/lookupop=get&search=0xC1CF6E31E6BADE8868B172B4F42ED6FBAB17C654' | sudo apt-key add -
RUN sudo apt update
RUN echo 'cd $WS_PATH && rosdep install --from-paths src --ignore-src --rosdistro=${ROS_DISTRO} -r -y --os=ubuntu:bionic' >> ~/.bashrc
