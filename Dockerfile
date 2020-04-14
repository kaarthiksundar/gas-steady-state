# Build on Centos/RHEL 8.
FROM centos:8 AS coinbrew

# Set environment variables.
ENV LD_LIBRARY_PATH=/usr/local/coinbrew/dist/lib
ENV DYLD_LIBRARY_PATH=/usr/local/coinbrew/dist/lib

# Install dependencies.
RUN yum install -y 'dnf-command(config-manager)' \
 && yum config-manager --set-enabled PowerTools \
 && yum install -y \
    blas-devel \
    cmake \
    diffutils \
    file \
    gcc-c++ \
    gcc-gfortran \
    gcc \
    git \
    lapack-devel \
    make \
    pkg-config \
    openmpi-devel \
    patch \
    wget \
    which

# Download coinbrew.
WORKDIR /usr/local
RUN git clone https://www.github.com/coin-or/coinbrew

# Download and build HSL (only if you have a license to do so)
#WORKDIR /usr/local/coinbrew
#RUN git clone https://github.com/coin-or-tools/ThirdParty-HSL.git
#RUN mkdir -p ThirdParty
#RUN mv ThirdParty-HSL ThirdParty/HSL
#WORKDIR /usr/local/coinbrew/ThirdParty/HSL
#COPY vendor/coinhsl-2014.01.10 coinhsl
#RUN ./configure prefix=/usr/local/coinbrew/dist/lib
#RUN make
#RUN make install

# Download and build Ipopt.
WORKDIR /usr/local/coinbrew
RUN which wget
RUN echo $PATH 
RUN ./coinbrew build Ipopt:releases/3.13.0 --no-prompt --verbosity=2 --test --enable-debug

# Build gas-steady-state.
WORKDIR /usr/src/gas-steady-state
ENV IPOPT_ROOT_DIR=/usr/local/coinbrew/dist
ENV IPOPT_INCLUDE_DIRS=/usr/local/coinbrew/dist/include
ENV IPOPT_LIBRARIES=/usr/local/coinbrew/dist/lib/
RUN ls -al /usr/local/coinbrew/dist/include
RUN ln -s $IPOPT_INCLUDE_DIRS/coin-or $IPOPT_INCLUDE_DIRS/coin
COPY cmake cmake
COPY include include
COPY src src
COPY third_party third_party
COPY CMakeLists.txt CMakeLists.txt
RUN cmake . \
 && make

# Clean CentOS image without all the build tools.
FROM centos:8

# Set environment variables needed for linking libraries.
ENV LD_LIBRARY_PATH=/usr/local/lib
ENV DYLD_LIBRARY_PATH=/usr/local/lib

# Install dependencies.
RUN yum install -y \
    'dnf-command(config-manager)' \
    epel-release \
 && yum config-manager --set-enabled PowerTools \
 && yum install -y \
    blas \
    lapack \
    MUMPS \
    openmpi

# Copy dist package and binary from build image.
WORKDIR /usr/local
COPY --from=coinbrew /usr/local/coinbrew/dist .
COPY --from=coinbrew /usr/src/gas-steady-state/bin/* bin/

# Run binary.
ENTRYPOINT ["bin/gas-steady-state"]

# Command parameters.
CMD ["-h"]