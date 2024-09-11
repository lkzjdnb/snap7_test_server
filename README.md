# Snap7 test server
Basic snap7 server that emulate a S7 PLC, mostly taken from the snap7 library examples.

## Usage
Easiest way to run is through docker, which allows compiling without polluting your environment and allows publishing on another port than 102.

### Compilation
`docker build -t s7-test-server .`

### Execution
`docker run -p 1102:102 --name snap7-test-server snap7-test-server:1`
