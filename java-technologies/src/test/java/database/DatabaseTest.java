package database;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.assertNotNull;
import static org.junit.jupiter.api.Assertions.assertNull;
import static org.junit.jupiter.api.Assertions.assertSame;

class DatabaseTest {

    @Test
    void createTable() {
        Database database = new Database(null);
        assertSame(Result.Status.OK, database.query("create table test1 (INT id, STR name)").getStatus());
        assertSame(Result.Status.OK, database.query("create table test2 (INT id, STR name)").getStatus());

        Result result = database.query("list tables");
        assertSame(Result.Status.OK, result.getStatus());
        assertSame(2, result.getRows().size());
    }

    @Test
    void strRange() {
        Database database = new Database(null);
        assertSame(Result.Status.OK, database.query("create table test1 (STR_RANGE str_range)").getStatus());

        assertSame(Result.Status.OK, database.query("insert into test1 (str_range) values(abc...zxy)").getStatus());
        assertSame(Result.Status.FAIL, database.query("insert into test1 (str_range) values(z...a)").getStatus());
    }

    @Test
    void removeDuplicates() {
        Database database = new Database(null);
        assertSame(Result.Status.OK, database.query("create table test1 (INT id, STR name)").getStatus());

        assertSame(Result.Status.OK, database.query("insert into test1 (id, name) values(1, abc)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id, name) values(1, abc)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id, name) values(1, abc)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id, name) values(1, abc)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id) values(1)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id) values(1)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id) values(1)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (name) values(xyz)").getStatus());

        assertSame(Result.Status.OK, database.query("delete duplicates test1").getStatus());

        Result result = database.query("select * from test1");
        assertSame(Result.Status.OK, result.getStatus());
        assertSame(3, result.getRows().size());
    }

    @Test
    void projectTable() {
        Database database = new Database(null);
        assertSame(Result.Status.OK, database.query("create table test1 (INT id, STR name)").getStatus());

        assertSame(Result.Status.OK, database.query("insert into test1 (id, name) values(1, abc)").getStatus());
        assertSame(Result.Status.OK, database.query("insert into test1 (id, name) values(2, xyz)").getStatus());

        Result result = database.query("select id from test1");
        assertSame(Result.Status.OK, result.getStatus());
        assertSame(2, result.getRows().size());
        assertNotNull(result.getRows().iterator().next().getElement("id"));
        assertNull(result.getRows().iterator().next().getElement("name"));

        result = database.query("select name,id from test1");
        assertSame(Result.Status.OK, result.getStatus());
        assertSame(2, result.getRows().size());
        assertNotNull(result.getRows().iterator().next().getElement("id"));
        assertNotNull(result.getRows().iterator().next().getElement("name"));
    }
}