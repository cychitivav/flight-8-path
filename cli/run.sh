docker build -t flight-8-path:latest .
export DISPLAY=$(ip route list default | awk '{print $3}'):0
export LIBGL_ALWAYS_INDIRECT=1
docker run -it --rm --name flight-8-path -v /tmp/.X11-unix:/tmp/.X11-unix -v $PWD:/home/mrs/workspace/src/flight-8-path -e DISPLAY=$DISPLAY flight-8-path:latest

# docker exec -it flight-8-path bash
