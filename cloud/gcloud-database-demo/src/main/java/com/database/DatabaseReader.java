package com.database;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import org.springframework.core.io.Resource;

import java.io.FileReader;
import java.io.InputStreamReader;

public class DatabaseReader {
    private Resource resource;

    public DatabaseReader(Resource resource) {
        this.resource = resource;
    }

    public Database read() throws Exception {
        Gson gson = new GsonBuilder().create();
        return gson.fromJson(new InputStreamReader(resource.getInputStream()), Database.class);
    }
}
