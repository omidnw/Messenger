// import 'package:chat_app/pages/chat.dart';
import 'package:chat_app/pages/login.dart';
import 'package:flutter/material.dart';
import 'package:chat_app/pages/chatterScreen.dart';
import 'pages/splash.dart';

void main() => runApp(ChatterApp());

class ChatterApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Messanger',
      
      theme: ThemeData(textTheme: TextTheme(bodyText1: TextStyle(fontFamily: 'Poppins'),),),
      // home: ChatterHome(),
      initialRoute: '/',
      routes: {
        '/':(context)=>ChatterHome(),
        '/login':(context)=>ChatterLogin(),
        '/chat':(context)=>ChatterScreen(),
      },
    );
  }
}

