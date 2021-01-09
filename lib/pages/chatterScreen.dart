import 'dart:io';
import 'dart:convert';
import '../constants.dart';
import 'package:flutter/material.dart';
import 'package:edge_alert/edge_alert.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

String messageText;

class SocketUtil{
  Socket _socket;
  bool socketInit = false;
  static const String SERVER_IP = "10.0.0.2";
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
    Function connectionListner, Function messageListner) async {
      try{
        print('Connecting to socket...');
        _socket = await Socket.connect(SERVER_IP, SERVER_PORT);
        connectionListner(true);
        _socket.listen((List<int> event) {
          messageListner(utf8.decode(event));
        });
        socketInit = true;
      } catch (e){
        print(e.toString());
        connectionListner(false);
        return false;
      }
      print('Connected!!');
      return true;
    }

    void closeSocket(){
      _socket.close();
      _socket = null;
    }

    void cleanUp() {
      if (_socket != null)
        _socket.destroy();
    }
}

class ChatterScreen extends StatefulWidget {
  @override
  _ChatterScreenState createState() => _ChatterScreenState();
}

class Args{
  final String username;

  Args(this.username);
}

class _ChatterScreenState extends State<ChatterScreen> with
WidgetsBindingObserver{
  final chatMsgTextController = TextEditingController();
  WebSocketChannel channel;

  String _status;
  SocketUtil _socketUtil;

  List<String> _messages;
  int userSelected = 1;

  void messageListner(String msg){
    print('Adding message');
    setState(() {
      _messages.add(msg);
    });
  }

  void connectionListner(bool connected){
    setState(() {
      _status = 'Status: ' + (connected ? 'Connected' : 'Failed to Connect to server!');
    });
  }

  @override
  void initState() {
    super.initState();
    _status = "";
    _messages = List<String>();
    _socketUtil = SocketUtil();
    _socketUtil.initSocket(connectionListner, messageListner);
  }

  @override
  void dispose(){
    channel.sink.close();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final Args args = ModalRoute.of(context).settings.arguments;

    return Scaffold(
      appBar: AppBar(
        iconTheme: IconThemeData(color: Colors.pink),
        elevation: 0,
        bottom: PreferredSize(
          preferredSize: Size(25, 10),
          child: Container(
            child: LinearProgressIndicator(
              valueColor: AlwaysStoppedAnimation<Color>(Colors.white),
              backgroundColor: Colors.pink[100],
            ),
            decoration: BoxDecoration(
                // color: Colors.pink[300],

                // borderRadius: BorderRadius.circular(20)
                ),
            constraints: BoxConstraints.expand(height: 1),
          ),
        ),
        backgroundColor: Colors.white10,
        // leading: Padding(
        //   padding: const EdgeInsets.all(12.0),
        //   child: CircleAvatar(backgroundImage: NetworkImage('https://cdn.clipart.email/93ce84c4f719bd9a234fb92ab331bec4_frisco-specialty-clinic-vail-health_480-480.png'),),
        // ),
        title: Row(
          children: <Widget>[
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: <Widget>[
                Text(
                  'Messenger',
                  style: TextStyle(
                      fontFamily: 'Poppins',
                      fontSize: 16,
                      color: Colors.pink),
                ),
              ],
            ),
          ],
        ),
      ),
      drawer: Drawer(
        child: ListView(
          children: <Widget>[
            UserAccountsDrawerHeader(
              decoration: BoxDecoration(
                color: Colors.pinkAccent,
              ),
              accountName: Text('Hello ${args.username}'),
              accountEmail: Text('Sample@email.com'),
              currentAccountPicture: CircleAvatar(
                child: Icon(Icons.supervised_user_circle,size: 72,color: Colors.white,),
                backgroundColor: Colors.pinkAccent,
              ),
            ),
            ListTile(
              leading: Icon(Icons.exit_to_app),
              title: Text("Logout"),
              subtitle: Text("Exit this room"),
              onTap: () async {
                Navigator.pushReplacementNamed(context, '/');
              },
            ),
          ],
        ),
      ),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.spaceBetween,
        crossAxisAlignment: CrossAxisAlignment.stretch,
        children: <Widget>[
          Expanded(
            child: ListView.builder(
              reverse: false,
              itemCount: null == _messages ? 0 : _messages.length,
              itemBuilder: (BuildContext ctx, int index){
                return MessageBubble(
                  msgSender: args.username,
                  msgText: _messages[index],
                  user: true
                );
              },
            )
          ),
          Container(
            padding: EdgeInsets.symmetric(vertical:10,horizontal: 10),
            decoration: kMessageContainerDecoration,
            child: Row(
              crossAxisAlignment: CrossAxisAlignment.center,
              children: <Widget>[
                Expanded(
                  child: Material(
                    borderRadius: BorderRadius.circular(50),
                    color: Colors.white,
                    elevation:5,
                    child: Padding(
                      padding: const EdgeInsets.only(left:8.0,top:2,bottom: 2),
                      child: TextField(
                        onChanged: (value) {
                          messageText = value;
                        },
                        controller: chatMsgTextController,
                        decoration: kMessageTextFieldDecoration,
                      ),
                    ),
                  ),
                ),
                MaterialButton(
                  shape: CircleBorder(),
                  color: Colors.pink[300],
                  onPressed: () {
                    if(chatMsgTextController.text.isEmpty)
                      return;
                    _socketUtil.sendMessage(chatMsgTextController.text).then(
                      (bool sent){
                        if(sent)
                          chatMsgTextController.clear();
                      }
                    );
                    /* setState(() {
                      _messages.add(chatMsgTextController.text);
                      chatMsgTextController.clear();
                    }); */
                  },
                  child:Padding(
                    padding: const EdgeInsets.all(10.0),
                    child: Icon(Icons.send,color: Colors.white,),
                  ) 
                  // Text(
                  //   'Send',
                  //   style: kSendButtonTextStyle,
                  // ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}


class MessageBubble extends StatelessWidget {
  final String msgText;
  final String msgSender;
  final bool user;
  MessageBubble({this.msgText, this.msgSender, this.user});

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(12.0),
      child: Column(
        crossAxisAlignment:
            user ? CrossAxisAlignment.end : CrossAxisAlignment.start,
        children: <Widget>[
          Container(
            padding: EdgeInsets.symmetric(horizontal: 10),
            child: Text(
              msgSender,
              style: TextStyle(
                  fontSize: 13, fontFamily: 'Poppins', color: Colors.black87),
            ),
          ),
          Material(
            borderRadius: BorderRadius.only(
              bottomLeft: Radius.circular(50),
              topLeft: user ? Radius.circular(50) : Radius.circular(0),
              bottomRight: Radius.circular(50),
              topRight: user ? Radius.circular(0) : Radius.circular(50),
            ),
            color: user ? Colors.pink[300] : Colors.white,
            elevation: 5,
            child: Padding(
              padding: const EdgeInsets.symmetric(vertical: 10, horizontal: 20),
              child: Text(
                msgText,
                style: TextStyle(
                  color: user ? Colors.white : Colors.pink,
                  fontFamily: 'Poppins',
                  fontSize: 15,
                ),
              ),
            ),
          ),
        ],
      ),
    );
  }
}
