import java.net.*;
import java.io.*;

public class TCPClient {
	public TCPClient(){
	}

	public byte[] query(String hostname, int port, byte[] toServer) throws IOException {
		// create static and dynamic buffers for storing data from server
		byte[] staticServerBuffer = new byte[1024];
		ByteArrayOutputStream dynamicServerBuffer = new ByteArrayOutputStream();

		// create socket and connect to server "hostname" on port "port"
		Socket clientSocket = new Socket(hostname, port);

		// send data to server
		clientSocket.getOutputStream().write(toServer);

		// store data from server. When no more data is received, read() returns -1
		while (true) {
			int lenServer = clientSocket.getInputStream().read(staticServerBuffer);
			if(lenServer != -1)
				dynamicServerBuffer.write(staticServerBuffer, 0, lenServer);
			else
				break;
		}

		// close socket
		clientSocket.close();

		// return data from server
		return dynamicServerBuffer.toByteArray();
	}
}
