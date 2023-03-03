import java.io.IOException;

public class main {
	public static void main(String[] args) throws IOException {
		TCPClient tcpClient = new TCPClient();
		byte[] toServer = "UPDATE account SET balance=balance + 100 WHERE acc_no='1337-420';".getBytes();
		byte[] fromServer = tcpClient.query("192.168.1.100", 1337, toServer);
		System.out.println(new String(fromServer));
	}
}