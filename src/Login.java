import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.util.Arrays;
import java.util.Objects;

// TODO: Find better solution than to set window invisible when moving to transfer page
// TODO: Look up prepared statement

public class Login extends JFrame implements ActionListener {
	private JLabel userName;
	private JLabel password;
	private JTextField userField;
	private JPasswordField passwordField;
	private JButton button;
	private JLabel feedback;

	Login(int width, int height){
		this.setSize(width, height);
		this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null);
		this.setResizable(false);
		JPanel panel = new JPanel();
		this.add(panel);
		panel.setLayout(null);

		JLabel bankName = new JLabel();
		bankName.setText("Login to Breadbank");
		bankName.setFont(new Font("Averia Sans Libre", Font.BOLD, 20));
		bankName.setBounds(30, 10, 500, 30);
		bankName.setHorizontalTextPosition(JLabel.LEFT); 	// puts bank name left of icon
		panel.add(bankName);

		userName = new JLabel("Username:");
		userName.setBounds(10, 55, 150, 25);
		panel.add(userName);

		password = new JLabel("Password:");
		password.setBounds(10, 90, 150, 25);
		panel.add(password);

		userField = new JTextField();
		userField.setBounds(85, 55, 170, 25);
		panel.add(userField);

		passwordField = new JPasswordField();
		passwordField.setBounds(85, 90, 170, 25);
		panel.add(passwordField);

		button = new JButton("Login");
		button.setBounds(85, 130, 100, 25);
		panel.add(button);
		button.addActionListener(this);

		feedback = new JLabel();
		panel.add(feedback);

		this.revalidate();
		this.repaint();
		this.setVisible(true);
	}

	public void actionPerformed(ActionEvent e){
		String username = userField.getText();
		char[] password = passwordField.getPassword();
		String stringPass = new String(password);

		TCPClient tcpClient = new TCPClient();
		String q = "LOGIN SELECT * FROM login WHERE username='" + username + "' AND password='" + stringPass + "';\0";

		byte[] fromServer;
		try {
			fromServer = tcpClient.query("LOCALHOST", 1337, q.getBytes());
		} catch (IOException ex) {
			throw new RuntimeException(ex);
		}
		String resultServer = new String(fromServer);
		System.out.println(resultServer);
		if(!resultServer.equals("")) {
			String[] accountInfo = resultServer.split(" ");
			feedback.setBounds(80, 160, 260, 25);
			feedback.setText("Login successful!");
			this.setVisible(false);
			TransferPage transferPage = new TransferPage(accountInfo);
		}
		else {
			feedback.setBounds(7, 160, 260, 25);
			feedback.setText("Wrong username or password. Try again.");
		}
	}
}
