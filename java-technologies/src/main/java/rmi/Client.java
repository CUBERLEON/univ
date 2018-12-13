package rmi;

import database.Result;
import dbconsoleviewer.DBConsoleViewer;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Client {

    public static void main(String[] args) {
        try {
            Registry registry = LocateRegistry.getRegistry(12300);

            IDatabaseRemote stub = (IDatabaseRemote) registry.lookup("IDatabaseRemote");

            while (true) {
                System.out.print("> ");
                Scanner scanner = new Scanner(System.in);
                String command = scanner.nextLine();

                Result result = stub.query(command);

                DBConsoleViewer.processResult(result);
            }
        } catch (Exception e) {
            System.err.println("Client exception: " + e.toString());
            e.printStackTrace();
        }
    }
}