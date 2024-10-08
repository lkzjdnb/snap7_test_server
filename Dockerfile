FROM alpine:latest

WORKDIR /build

RUN apk update && apk upgrade && apk add --no-cache make g++ 7zip

# ADD https://sourceforge.net/projects/snap7/files/1.4.2/snap7-full-1.4.2.7z/download snap7-full.7z
RUN wget https://sourceforge.net/projects/snap7/files/1.4.2/snap7-full-1.4.2.7z/download -O snap7-full.7z

RUN 7z x snap7-full.7z
RUN mv snap7-full-1.4.2 snap7

WORKDIR ./snap7/build/unix

RUN make -f x86_64_linux.mk install

WORKDIR /app

COPY main.cpp ./main.cpp

RUN g++ main.cpp -I/build/snap7/release/Wrappers/c-cpp /build/snap7/release/Wrappers/c-cpp/snap7.cpp -lsnap7 -o server

EXPOSE 102

CMD ["./server"]
