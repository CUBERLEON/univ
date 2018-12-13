package soap;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import database.Result;
import dbconsoleviewer.DBConsoleViewer;

import javax.xml.namespace.QName;
import javax.xml.ws.Service;
import java.net.URL;
import java.util.Scanner;

public class DatabaseClient {
    public static void main(String[] args) throws Exception {
        URL url = new URL("http://localhost:7779/ws/database?wsdl");

        // 1st argument service URI, refer to wsdl document above
        // 2nd argument is service name, refer to wsdl document above
        QName qname = new QName("http://soap/", "QueryDatabaseService");
        Service service = Service.create(url, qname);
        IQueryDatabase databaseService = service.getPort(IQueryDatabase.class);

        while (true) {
            System.out.print("> ");
            Scanner scanner = new Scanner(System.in);
            String command = scanner.nextLine();

            Gson gson = new GsonBuilder().create();
            Result result = gson.fromJson(databaseService.query(command), Result.class);
            DBConsoleViewer.processResult(result);
        }
    }
}
