#!/bin/bash
# --rm Automatically remove the container when it exits
# -e Set environment variables
# -v Bind mount a volume
DOCKER_IMAGE=quay.io/pypa/manylinux1_x86_64
PLAT=manylinux1_x86_64
docker pull $DOCKER_IMAGE
docker run --rm -e PLAT=$PLAT -v `pwd`:/io $DOCKER_IMAGE bash /io/travis/build-wheels.sh

# docker exec -it -v /home/dominik/projects/code/dlplan:/dlplan -w /dlplan quay.io/pypa/manylinux2014_x86_64 /bin/bash
