import 'dart:io';
import 'dart:convert';

class SocketUtil{
  Socket _socket;
  bool socketInit = false;
  static const String SERVER_IP = "10.0.2.2";
  static const int SERVER_PORT = 8080;


  Future<bool> sendMessage(String msg) async {
    try{
      _socket.add(utf8.encode(msg));
    }catch (e){
      print(e.toString());
      return false;
    }
    return true;
  }

  Future<bool> initSocket(
    String username) async {
      try{
        print('Connecting to socket...');
        _socket = await Socket.connect(SERVER_IP, SERVER_PORT, timeout: Duration(seconds: 10));
        _socket.add(utf8.encode(username));
        socketInit = true;
      } catch (e){
        print(e.toString());
        return false;
      }
      print('Connected!!');
      return true;
    }

    void addMessageListner(Function messageListner, Function connectionListner){
      connectionListner(socketInit);
      _socket.listen((List<int> event) {
          messageListner(utf8.decode(event));
      });
    }

    void closeSocket(){
      _socket.add(utf8.encode("exit"));
      _socket.close();
      _socket = null;
    }

    void cleanUp() {
      if (_socket != null)
        _socket.destroy();
    }
}