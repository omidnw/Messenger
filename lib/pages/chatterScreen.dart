import '../constants.dart';
import '../socketUtil.dart';
import 'package:flutter/material.dart';

String messageText;
String username;
SocketUtil socketUtil = SocketUtil();

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

  String _status;

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
      _status = (connected ? 'Connected' : 'Failed to Connect to server!');
    });
  }

  @override
  void initState() {
    super.initState();
    _status = "";
    _messages = List<String>();
    socketUtil.addMessageListner(messageListner,connectionListner);
  }

  @override
  void dispose(){
    print('Disconnected!!');
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final Args args = ModalRoute.of(context).settings.arguments;
    username = args.username;

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
              accountName: Text('Hello' + username),
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
                socketUtil.closeSocket();
                socketUtil.cleanUp();
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
                List<String> _msg = _messages[index].split(":");

                String _sender = _msg[0].trim();
                String _msgText = _msg[1];
                bool _user = (_sender==username) ? true : false;

                return MessageBubble(
                  msgSender: _sender,
                  msgText: _msgText,
                  user: _user
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
                    socketUtil.sendMessage(chatMsgTextController.text).then(
                      (bool sent){
                        if(sent)
                          setState(() {
                            String _msg = username+":"+chatMsgTextController.text;
                            _messages.add(_msg);
                            chatMsgTextController.clear();
                        });
                      }
                    );
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
