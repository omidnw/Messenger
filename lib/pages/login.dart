import 'package:chat_app/widgets/custombutton.dart';
import 'package:chat_app/widgets/customtextinput.dart';
import 'package:flutter/material.dart';
import 'package:edge_alert/edge_alert.dart';
import 'package:modal_progress_hud/modal_progress_hud.dart';
import './chatterScreen.dart';

class ChatterLogin extends StatefulWidget {
  @override
  _ChatterLoginState createState() => _ChatterLoginState();
}

class _ChatterLoginState extends State<ChatterLogin> {
  String username;
  bool loggingin = false;
  @override
  Widget build(BuildContext context) {
    return ModalProgressHUD(
      inAsyncCall: loggingin,
      child: Scaffold(
        // backgroundColor: Colors.transparent,
        body: SingleChildScrollView(
          child: Container(
            height: MediaQuery.of(context).size.height,
            // margin: EdgeInsets.only(top:MediaQuery.of(context).size.height*0.2),
            child: Center(
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: <Widget>[
                  Hero(
                    tag: 'heroicon',
                    child: Icon(
                      Icons.message,
                      size: 120,
                      color: Colors.pink[400],
                    ),
                  ),
                  SizedBox(
                    height: MediaQuery.of(context).size.height * 0.02,
                  ),
                  Hero(
                    tag: 'HeroTitle',
                    child: Text(
                      'Messenger',
                      style: TextStyle(
                          color: Colors.pink[400],
                          fontFamily: 'Poppins',
                          fontSize: 26,
                          fontWeight: FontWeight.w700),
                    ),
                  ),
                  SizedBox(
                    height: MediaQuery.of(context).size.height * 0.01,
                  ),
                  CustomTextInput(
                    hintText: 'Username',
                    leading: Icons.supervised_user_circle,
                    obscure: false,
                    userTyped: (val) {
                      username = val;
                    },
                  ),
                  SizedBox(
                    height: 30,
                  ),
                  Hero(
                    tag: 'loginbutton',
                    child: CustomButton(
                      text: 'Enter Room',
                      accentColor: Colors.white,
                      mainColor: Colors.pink,
                      onpress: () async {
                        if(username == null)
                          EdgeAlert.show(
                            context,
                            title: 'Uh - oh',
                            description: 'Username cannot be empty!!',
                            gravity: EdgeAlert.BOTTOM,
                            icon: Icons.error,
                            backgroundColor: Colors.pink[400]
                          );
                        else
                          Navigator.pushReplacementNamed(
                            context,
                            '/chat',
                            arguments: Args(username.trim()));
                      },
                    ),
                  ),
                  SizedBox(
                    height: MediaQuery.of(context).size.height * 0.1,
                  ),
                  Hero(
                    tag: 'footer',
                    child: Text('Front-end by Aria Khoshnood\nInspired by ishandeveloper design',
                      textAlign: TextAlign.center,
                      style: TextStyle(fontFamily: 'Poppins', color: Colors.pink[200]),
                    ),
                  )
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
