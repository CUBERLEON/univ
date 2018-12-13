package rest;

import database.Database;
import database.DatabaseReader;
import database.Result;
import org.springframework.web.bind.annotation.*;

import java.util.Map;
import java.util.Optional;

@RestController
public class DatabaseController {

    private Database database;

    public DatabaseController() {
        try {
            database = new DatabaseReader("test.json").read();
        } catch (Exception e) {
            database = new Database("test.json");
            e.printStackTrace();
        }
    }

    @GetMapping(value = "/database/tables")
    public Result tables() {
        return database.query("list tables");
    }

    @DeleteMapping(value = "/database/tables/delete/{tableName}")
    public Result dropTable(@PathVariable String tableName) {
        return database.query(String.format("drop table %s", tableName));
    }

    @PostMapping(value = "/database/tables/create/{tableName}/{columns}")
    public Result createTable(@PathVariable String columns,
                              @PathVariable String tableName) {
        return database.query(String.format("create table %s (%s)", tableName, columns));
    }

    @GetMapping(value = "/database/{tableName}/select/{columns}/{condition}")
    public Result selectCondition(@PathVariable String columns,
                                  @PathVariable String tableName,
                                  @PathVariable String condition) {
        return database.query(String.format("select %s from %s where %s", columns, tableName, condition));
    }

    @GetMapping(value = "/database/{tableName}/select/{columns}")
    public Result select(@PathVariable String columns,
                         @PathVariable String tableName) {
        return database.query(String.format("select %s from %s", columns, tableName));
    }

    @PostMapping(value = "/database/{tableName}/insert/{columns}/{values}")
    public Result insert(@PathVariable String columns,
                         @PathVariable String tableName,
                         @PathVariable String values) {
        return database.query(String.format("insert into %s (%s) values (%s)", tableName, columns, values));
    }

    @DeleteMapping(value = "/database/{tableName}/delete/{condition}")
    public Result delete(@PathVariable String tableName,
                         @PathVariable String condition) {
        return database.query(String.format("delete from %s where %s", tableName, condition));
    }

    @DeleteMapping(value = "/database/{tableName}/delete_dublicates")
    public Result deleteDuplicates(@PathVariable String tableName) {
        return database.query(String.format("delete duplicates %s", tableName));
    }
}