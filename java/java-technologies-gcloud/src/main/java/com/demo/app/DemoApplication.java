package com.demo.app;

import com.database.Database;
import com.database.DatabaseReader;
import com.database.Result;
//import org.springframework.beans.factory.annotation.Value;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.core.io.Resource;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.*;

@SpringBootApplication
@Controller
public class DemoApplication {

	public static void main(String[] args) {
		SpringApplication.run(DemoApplication.class, args);
	}

    private Database database;

//    @Value("gs://database-demo-222122.appspot.com/test.json")
//    private Resource databaseFile;

    public DemoApplication() {
//        String databasePath = databaseFile.getFilename();
//        System.out.println(databaseFile);
//        String databaseFile = "test.json";
        try {
//            database = new DatabaseReader(databaseFile).read();
        } catch (Exception e) {
//            database = new Database(databaseFile);
            e.printStackTrace();
        }
    }

    @GetMapping("/")
    public String queryForm(@RequestParam(name="name", required=false, defaultValue="guest") String name,
                            Model model) {
        model.addAttribute("name", name);
        return "database";
    }

    @PostMapping(value = "/database", produces = { MediaType.APPLICATION_JSON_VALUE })
    @ResponseBody
    public Result query(@RequestParam(name="query", required=true) String query)
    {
        System.out.println(query);
        return database.query(query);
    }
}
