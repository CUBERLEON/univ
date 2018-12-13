package rmi;

import database.Database;
import database.DatabaseReader;
import database.Result;

public class DatabaseRemote implements IDatabaseRemote {

    private Database database;

    public DatabaseRemote(String databasePath) {
        try {
            database = new DatabaseReader(databasePath).read();
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    public Result query(String msg) {
        System.out.println("Accepted query: " + msg);
        return database.query(msg);
    }
}