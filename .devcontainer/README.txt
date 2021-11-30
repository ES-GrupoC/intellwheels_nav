Change directory to .devcontainer

Create a workspace by creating a folder and inside that folder a "src" folder.

Open Dockerfile and change line "ENV WS_PATH=<workspace-absolute-path>".

$ sudo docker build . -t ros-iw

# Change the path to your workspace folder
$ docker run -it -v <path-to-your-workspace>:/home/guest/iw_ws -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$DISPLAY -h $HOSTNAME -v $HOME/.Xauthority:/root/.Xauthority --name ros-iw ros-iw
OR
Install docker-compose then edit the docker-compose.yml inside Volumes to your workspace folder and run
$ docker-compose run --name ros-iw ros-iw bash

The container should run for the first time and start.

$ catkin build
$ source devel/setup.bash

commands:
$ docker start ros-iw # to start the container
$ docker stop ros-iw # to stop the container
$ docker exec -it ros-iw bash # attach a bash shell to the container

NOTE: the first time you run the container it'll install dependencies
