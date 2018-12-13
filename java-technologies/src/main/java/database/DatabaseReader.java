package database;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.FileReader;

public class DatabaseReader {
    private String filePath;

    public DatabaseReader(String filePath) {
        this.filePath = filePath;
    }

    public String getFilePath() {
        return filePath;
    }

    public void setFilePath(String filePath) {
        this.filePath = filePath;
    }

    public Database read() throws Exception {
        FileReader reader = new FileReader(filePath);
        Gson gson = new GsonBuilder().create();
        return gson.fromJson(reader, Database.class);
    }
}
