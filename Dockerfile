FROM alpine
RUN apk update
RUN apk add g++ git cmake gtest gtest-dev make doxygen
COPY . safe
RUN mkdir -p safe/build
WORKDIR safe/build
RUN cmake .. 
RUN make -j$(nproc) safe_tests
