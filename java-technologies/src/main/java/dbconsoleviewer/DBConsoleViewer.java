package dbconsoleviewer;

import database.*;

import java.util.Scanner;

public class DBConsoleViewer {

    private static Database database;

    public static void main(String[] args) {
        String databasePath = "test.json";
        try {
            database = new DatabaseReader(databasePath).read();
        } catch (Exception e) {
            database = new Database(databasePath);
            e.printStackTrace();
        }

        while (true) {
            System.out.print("> ");
            Scanner scanner = new Scanner(System.in);
            String command = scanner.nextLine();

            Result result = database.query(command);

            processResult(result);
        }
    }

    public static void processResult(Result result) {
        if (result.getStatus() == Result.Status.FAIL) {
            System.out.println("FAIL");
            System.out.println(result.getReport());
            return;
        }

        if (result.getRows() == null) {
            System.out.println("OK");
            return;
        }

        if (result.getRows().isEmpty()) {
            System.out.println("Nothing was found");
            return;
        }

        for (Element element : result.getRows().iterator().next().getElements()) {
            System.out.print(String.format("%15s", element.getColumn()));
        }
        System.out.println();
        System.out.println();

        for (Row row : result.getRows()) {
            for (Element element : row.getElements()) {
                System.out.print(String.format("%15s", element.getAsString()));
            }
            System.out.println();
        }
    }
}
