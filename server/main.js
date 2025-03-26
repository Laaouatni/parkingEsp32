import WebSocket from 'ws';

// Create a WebSocket server on port 8080
const WebSocketServer = new WebSocket.Server({ port: 8080 });

// Event handler for when a client connects
WebSocketServer.on('connection', (clientSocket) => {
  console.log('A client connected'); // Log when a client connects

  // Event handler for when the server receives a message from a client
  clientSocket.on('message', (message) => {
    console.log(`Received: ${message}`); // Log the received message
    clientSocket.send(`You said: ${message}`); // Send a response to the client
  });

  // Event handler for when a client disconnects
  clientSocket.on('close', () => {
    console.log('Client disconnected'); // Log when a client disconnects
  });
});