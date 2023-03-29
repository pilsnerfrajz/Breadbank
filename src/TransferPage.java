import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;

public class TransferPage extends JFrame  {
	private JTextField transferAmountField;
	private JButton transferButton;
	private JLabel transferAmount;
	private JTextField acc2Field;
	private JLabel acc2Label;
	private JTextField acc1Field;
	private JLabel acc1Label;

	public TransferPage(){
		this.setSize(400, 400);
		this.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null);
		this.setResizable(false);
		JPanel panel = new JPanel();
		this.add(panel);
		panel.setLayout(null);

		acc1Label = new JLabel("From account");
		acc1Label.setBounds(10, 10, 150, 25);
		panel.add(acc1Label);

		acc1Field = new JTextField();
		acc1Field.setBounds(100, 10, 150, 25);
		panel.add(acc1Field);

		acc2Label = new JLabel("To account");
		acc2Label.setBounds(10, 50, 150, 25);
		panel.add(acc2Label);

		acc2Field = new JTextField();
		acc2Field.setBounds(100, 50, 150, 25);
		panel.add(acc2Field);

		transferAmount = new JLabel("Amount");
		transferAmount.setBounds(10, 90, 150, 25);
		panel.add(transferAmount);

		transferAmountField = new JTextField();
		transferAmountField.setBounds(100, 90, 150, 25);
		panel.add(transferAmountField);

		transferButton = new JButton("Transfer");
		transferButton.setBounds(122, 130, 100, 25);
		panel.add(transferButton);
		transferButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					String acc1 = acc1Field.getText();
					String acc2 = acc2Field.getText();
					double amount = Double.parseDouble(transferAmountField.getText());

					TCPClient tcpClient = new TCPClient();
					String transferString = "UPDATE account SET balance=balance + -" + amount + " WHERE acc_no='" + acc1 + "';"
							+ "UPDATE account SET balance=balance + " + amount + " WHERE acc_no='" + acc2 + "';";
					byte[] toServer = transferString.getBytes();
					byte[] serverBytes = tcpClient.query("192.168.1.100", 1337, toServer);
					String serverOutput = new String(serverBytes);
					System.out.println(serverOutput);
				} catch (IOException ex) {
					throw new RuntimeException(ex);
				}
			}
		});
		setVisible(true);
	}
/*
	@Override
	public void actionPerformed(ActionEvent e) {
		try {
			String acc1 = acc1Field.getText();
			String acc2 = acc2Field.getText();
			double amount = Double.parseDouble(transferAmountField.getText());

			TCPClient tcpClient = new TCPClient();
			String transferString = "UPDATE account SET balance=balance + -" + amount + " WHERE acc_no='" + acc1 + "';"
								+ "UPDATE account SET balance=balance + " + amount + " WHERE acc_no='" + acc2 + "';";
			byte[] toServer = transferString.getBytes();
			byte[] serverBytes = tcpClient.query("192.168.1.100", 1337, toServer);
			String serverOutput = new String(serverBytes);
			System.out.println(serverOutput);
		} catch (IOException ex) {
			throw new RuntimeException(ex);
		}
	}

 */
}
