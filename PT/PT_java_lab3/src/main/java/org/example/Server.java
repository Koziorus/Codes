package org.example;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Server
{


    public static void main(String[] args) throws IOException
    {
        ServerSocket server_socket;
        Socket client_socket;
        PrintWriter out;
        BufferedReader in;
        int port = Integer.parseInt(args[0]);
        server_socket = new ServerSocket(port);
        client_socket = server_socket.accept();
        out = new PrintWriter(client_socket.getOutputStream());
        in = new BufferedReader(new InputStreamReader(client_socket.getInputStream()));

    }
}
