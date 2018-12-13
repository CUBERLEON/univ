package springmvc;

import database.Database;
import database.DatabaseReader;
import database.Result;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
public class DatabaseController {
    private Database database;

    public DatabaseController() {
        try {
            database = new DatabaseReader("test.json").read();
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
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
        return database.query(query);
    }
}