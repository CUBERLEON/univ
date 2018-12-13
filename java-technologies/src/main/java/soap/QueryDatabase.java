package soap;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import database.Database;
import database.DatabaseReader;

import javax.jws.WebService;

// Service Implementation
@WebService(endpointInterface = "soap.IQueryDatabase")
public class QueryDatabase implements IQueryDatabase {
    private Database database;

    public void loadDatabase(String databasePath) {
        try {
            database = new DatabaseReader(databasePath).read();
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    @Override
    public String query(String msg) {
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        return gson.toJson(database.query(msg));
    }
}