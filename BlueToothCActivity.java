package com.example.lock;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import android.R.menu;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

public class BlueToothCActivity extends Activity {

	static Boolean Isconnect = Boolean.valueOf(false);
	public static final int MESSAGE_TOAST = 5;
	private static final int REQUEST_CONNECT_DEVICE = 1; // �궨���ѯ�豸���
	private static final int REQUEST_ENABLE_BT = 0;
	private final static String MY_UUID = "00001101-0000-1000-8000-00805F9B34FB"; // SPP����UUID��
	public static final String TOAST = "toast";
	public static BluetoothDevice device;
	final static int MENU_BLUETOOTH = Menu.FIRST;
	final static int MENU_REFRESH = Menu.FIRST + 1;
	final static int MENU_QUIT = Menu.FIRST + 2;
	private EditText editText1;
	private TextView textView2;
	private Button button1;
	private Button button2;
	private Button button3;
	private Button button4;
	private Button button5;
	private Button button6;
	private Button button7;
	private Button button8;
	private Button button9;
	private Button button0;
	private Button buttonBack;
	private Button buttonEnter;
	private Button buttonConnect;
	private Button buttonFresh;
	private byte[] byte1 = "ON1".getBytes();
	private byte[] byte2 = "ON2".getBytes();
	private byte[] byte3 = "ON3".getBytes();
	private byte[] byte4 = "ON4".getBytes();
	private byte[] byte5 = "ON5".getBytes();
	private byte[] byte6 = "ON6".getBytes();
	private byte[] byte7 = "ON7".getBytes();
	private byte[] byte8 = "ON8".getBytes();
	private byte[] byte9 = "ON9".getBytes();
	private byte[] byte0 = "ONE".getBytes();
	private byte[] byteBack = "ONA".getBytes();
	private byte[] byteEnter = "ONC".getBytes();
	// private byte[] bytedown = "ONB".getBytes();
	// private byte[] bytef = "ONF".getBytes();
	// private byte[] byteleft = "ONC".getBytes();
	// private byte[] byteright = "OND".getBytes();
	// private byte[] byteup = "ONA".getBytes();

	private InputStream is; // ������������������������
	// private TextView text0; //��ʾ������
	private EditText edit0; // ��������������
	private TextView dis; // ����������ʾ���
	private ScrollView sv; // ��ҳ���
	private String smsg = ""; // ��ʾ�����ݻ���
	private String fmsg = ""; // ���������ݻ���

	BluetoothDevice _device = null; // �����豸
	BluetoothSocket _socket = null; // ����ͨ��socket
	boolean _discoveryFinished = false;
	boolean bRun = true;
	boolean bThread = false;

	private BluetoothAdapter _bluetooth = BluetoothAdapter.getDefaultAdapter(); // ��ȡ�����������������������豸

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		editText1 = (EditText) findViewById(R.id.editText1);
		textView2 = (TextView) findViewById(R.id.textView2);
		button1 = (Button) findViewById(R.id.button1);
		button2 = (Button) findViewById(R.id.button2);
		button3 = (Button) findViewById(R.id.button3);
		button4 = (Button) findViewById(R.id.button4);
		button5 = (Button) findViewById(R.id.button5);
		button6 = (Button) findViewById(R.id.button6);
		button7 = (Button) findViewById(R.id.button7);
		button8 = (Button) findViewById(R.id.button8);
		button9 = (Button) findViewById(R.id.button9);
		button0 = (Button) findViewById(R.id.button0);
		buttonBack = (Button) findViewById(R.id.buttonBack);
		buttonEnter = (Button) findViewById(R.id.buttonEnter);
		buttonConnect = (Button) findViewById(R.id.buttonConnect);
		buttonFresh = (Button) findViewById(R.id.buttonFresh);

		// ����򿪱��������豸���ɹ�����ʾ��Ϣ����������
		if (_bluetooth == null) {
			Toast.makeText(this, "�޷����ֻ���������ȷ���ֻ��Ƿ����������ܣ�", Toast.LENGTH_LONG)
					.show();
			finish();
			return;
		}

		// �����豸���Ա�����
		new Thread() {
			public void run() {
				if (_bluetooth.isEnabled() == false) {
					_bluetooth.enable();
				}
			}
		}.start();

		buttonFresh.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������
					String ran = "";
					for (int j = 0; j < 6; j++) {
						ran = ran + ""
								+ String.valueOf((int) (Math.random() * 9 + 0));
					}
					editText1.setText(ran);
					// byte[] bos = ("P" +
					// editText1.getText().toString()).getBytes();
					byte[] bos = ("P" + ran).getBytes();
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button1.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte1;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button2.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte2;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button3.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte3;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button4.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte4;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button5.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte5;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button6.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte6;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button7.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte7;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button8.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte8;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button9.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte9;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		button0.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byte0;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		buttonBack.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byteBack;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});

		buttonEnter.setOnClickListener(new View.OnClickListener() {

			public void onClick(View v) {
				// TODO Auto-generated method stub
				int i = 0;
				int n = 0;
				try {
					OutputStream os = _socket.getOutputStream(); // �������������

					byte[] bos = byteEnter;
					for (i = 0; i < bos.length; i++) {
						if (bos[i] == 0x0a)
							n++;
					}
					byte[] bos_new = new byte[bos.length + n];
					n = 0;
					for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
						if (bos[i] == 0x0a) {
							bos_new[n] = 0x0d;
							n++;
							bos_new[n] = 0x0a;
						} else {
							bos_new[n] = bos[i];
						}
						n++;
					}

					os.write(bos_new);
				} catch (IOException e) {
				}
			}
		});
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);

		switch (requestCode) {
		case REQUEST_CONNECT_DEVICE: // ���ӽ������DeviceListActivity���÷���
			// ��Ӧ���ؽ��
			if (resultCode == Activity.RESULT_OK) { // ���ӳɹ�����DeviceListActivity���÷���
				// MAC��ַ����DeviceListActivity���÷���
				String address = data.getExtras().getString(
						DeviceListActivity.EXTRA_DEVICE_ADDRESS);
				// �õ������豸���
				_device = _bluetooth.getRemoteDevice(address);

				// �÷���ŵõ�socket
				try {
					_socket = _device.createRfcommSocketToServiceRecord(UUID
							.fromString(MY_UUID));
				} catch (IOException e) {
					Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT).show();
				}
				// ����socket
				// Button btn = (Button)
				// findViewById(R.id.button3);////////////////////////
				try {
					_socket.connect();
					Toast.makeText(this, "����" + _device.getName() + "�ɹ���",
							Toast.LENGTH_SHORT).show();
					buttonConnect.setText("�Ͽ�");
				} catch (IOException e) {
					try {
						Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT)
								.show();
						_socket.close();
						_socket = null;
					} catch (IOException ee) {
						Toast.makeText(this, "����ʧ�ܣ�", Toast.LENGTH_SHORT)
								.show();
					}

					return;
				}

				// �򿪽����߳�
				try {
					is = _socket.getInputStream(); // �õ���������������
				} catch (IOException e) {
					Toast.makeText(this, "��������ʧ�ܣ�", Toast.LENGTH_SHORT).show();
					return;
				}
				if (bThread == false) {
					ReadThread.start();
					bThread = true;
				} else {
					bRun = true;
				}
			}
			break;
		default:
			break;
		}
	}

	// ���������߳�
	Thread ReadThread = new Thread() {

		public void run() {
			int num = 0;
			byte[] buffer = new byte[1024];
			byte[] buffer_new = new byte[1024];
			int i = 0;
			int n = 0;
			bRun = true;
			// �����߳�
			while (true) {
				try {
					while (is.available() == 0) {
						while (bRun == false) {
						}
					}
					while (true) {
						num = is.read(buffer); // ��������
						n = 0;

						String s0 = new String(buffer, 0, num);
						fmsg += s0; // �����յ�����
						for (i = 0; i < num; i++) {
							if ((buffer[i] == 0x0d) && (buffer[i + 1] == 0x0a)) {
								buffer_new[n] = 0x0a;
								i++;
							} else {
								buffer_new[n] = buffer[i];
							}
							n++;
						}
						String s = new String(buffer_new, 0, n);
						smsg += s; // д����ջ���
						if (is.available() == 0)
							break; // ��ʱ��û�����ݲ�����������ʾ
					}
					// ������ʾ��Ϣ��������ʾˢ��
					handler.sendMessage(handler.obtainMessage());
				} catch (IOException e) {
				}
			}
		}

	};

	// ��Ϣ�������
	Handler handler = new Handler() {
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			dis.setText(smsg); // ��ʾ����
			sv.scrollTo(0, dis.getMeasuredHeight()); // �����������һҳ
		}
	};

	// �رճ�����ô�����
	public void onDestroy() {
		super.onDestroy();
		if (_socket != null) // �ر�����socket
			try {
				_socket.close();
			} catch (IOException e) {
			}
		// _bluetooth.disable(); //�ر���������
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		menu.add(0, MENU_BLUETOOTH, 0, "���������豸");
		menu.add(0, MENU_REFRESH, 1, "ˢ�¶�̬����");
		menu.add(0, MENU_QUIT, 2, "�˳�");
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		switch (id) {
		case MENU_BLUETOOTH:
			if (_bluetooth.isEnabled() == false) { // ����������񲻿�������ʾ
				Toast.makeText(this, " ��������...", Toast.LENGTH_LONG).show();
				break;
			}

			// ��δ�����豸���DeviceListActivity�����豸����
			// Button btn = (Button) findViewById(R.id.Button03);
			if (_socket == null) {
				Intent serverIntent = new Intent(this, DeviceListActivity.class); // ��ת��������
				startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE); // ���÷��غ궨��
			} else {
				// �ر�����socket
				try {

					is.close();
					_socket.close();
					_socket = null;
					bRun = false;
					buttonConnect.setText("����");

				} catch (IOException e) {
				}
			}
			break;

		case MENU_REFRESH:
			int i = 0;
			int n = 0;
			try {
				OutputStream os = _socket.getOutputStream(); // �������������
				String ran = "";
				for (int j = 0; j < 6; j++) {
					ran = ran + ""
							+ String.valueOf((int) (Math.random() * 9 + 0));
				}
				editText1.setText(ran);
				byte[] bos = editText1.getText().toString().getBytes();
				for (i = 0; i < bos.length; i++) {
					if (bos[i] == 0x0a)
						n++;
				}
				byte[] bos_new = new byte[bos.length + n];
				n = 0;
				for (i = 0; i < bos.length; i++) { // �ֻ��л���Ϊ0a,�����Ϊ0d 0a���ٷ���
					if (bos[i] == 0x0a) {
						bos_new[n] = 0x0d;
						n++;
						bos_new[n] = 0x0a;
					} else {
						bos_new[n] = bos[i];
					}
					n++;
				}

				os.write(bos_new);
			} catch (IOException e) {
			}
			break;
		case MENU_QUIT:
			finish();
			break;
		}
		return false;
	}

}
