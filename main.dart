import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

void main() {
  runApp(BluetoothCarApp());
}

class BluetoothCarApp extends StatefulWidget {
  @override
  _BluetoothCarAppState createState() => _BluetoothCarAppState();
}

class _BluetoothCarAppState extends State<BluetoothCarApp> {
  BluetoothState _bluetoothState = BluetoothState.UNKNOWN;
  BluetoothConnection? connection;
  BluetoothDevice? selectedDevice;
  List<BluetoothDevice> pairedDevices = [];

  @override
  void initState() {
    super.initState();
    FlutterBluetoothSerial.instance.state.then((state) {
      setState(() {
        _bluetoothState = state;
      });
    });
    _getPairedDevices();
  }

  void _getPairedDevices() async {
    List<BluetoothDevice> devices = await FlutterBluetoothSerial.instance.getBondedDevices();
    setState(() {
      pairedDevices = devices;
    });
  }

  void _connectToDevice(BluetoothDevice device) async {
    try {
      connection = await BluetoothConnection.toAddress(device.address);
      setState(() {
        selectedDevice = device;
      });
    } catch (e) {
      print('Connection error: $e');
    }
  }

  void _sendCommand(String command) {
    if (connection != null && connection!.isConnected) {
      connection!.output.add(Uint8List.fromList(command.codeUnits));
      connection!.output.allSent;
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(title: Text('Bluetooth Car Controller')),
        body: Column(
          children: [
            DropdownButton<BluetoothDevice>(
              hint: Text('Select Bluetooth Device'),
              value: selectedDevice,
              onChanged: (BluetoothDevice? device) {
                _connectToDevice(device!);
              },
              items: pairedDevices.map((device) {
                return DropdownMenuItem(
                  value: device,
                  child: Text(device.name ?? 'Unknown'),
                );
              }).toList(),
            ),
            Text('Status: ${selectedDevice != null ? 'Connected to ${selectedDevice!.name}' : 'Not Connected'}'),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                ElevatedButton(onPressed: () => _sendCommand('F'), child: Text('Forward')),
                ElevatedButton(onPressed: () => _sendCommand('B'), child: Text('Backward')),
              ],
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                ElevatedButton(onPressed: () => _sendCommand('L'), child: Text('Left')),
                ElevatedButton(onPressed: () => _sendCommand('R'), child: Text('Right')),
              ],
            ),
            ElevatedButton(onPressed: () => _sendCommand('S'), child: Text('Stop')),
          ],
        ),
      ),
    );
  }
}
