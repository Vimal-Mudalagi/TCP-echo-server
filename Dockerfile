# Use a base image with GCC installed
FROM gcc:latest

# Set the working directory in the container
WORKDIR /app

# Copy the server.c file into the container
COPY server.c .

# Compile the C code
RUN gcc server.c -o server

# Expose the port that your server listens on
EXPOSE 8080

# Command to run the server
CMD ["./server"]
