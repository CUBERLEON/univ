package database;

import java.io.Serializable;
import java.util.Collection;

public class Result implements Serializable {
    public enum Status {
        OK,
        FAIL
    }

    private Status status;
    private String report;
    private Collection<Row> rows;

    Result(Status status) {
        this.status = status;
        report = "";
        rows = null;
    }

    public Status getStatus() {
        return status;
    }

    public String getReport() {
        return report;
    }

    Result setReport(String report) {
        this.report = report;
        return this;
    }

    public Collection<Row> getRows() {
        return rows;
    }

    Result setRows(Collection<Row> rows) {
        this.rows = rows;
        return this;
    }
}
