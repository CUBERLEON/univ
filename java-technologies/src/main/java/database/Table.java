package database;

import java.util.*;
import java.util.function.Predicate;
import java.util.stream.Collectors;

public class Table {
    private String name;

    private List<Row> rows;
    private Map<String, Column> columns;

    Table(String name, Collection<Column> columns) {
        this.name = name;

        this.rows = new ArrayList<>();

        this.columns = new HashMap<>();
        for (Column col : columns) {
            this.columns.put(col.getName(), col);
        }
    }

    public void insert(Map<Column, String> values) throws Exception {
        ArrayList<Element> elements = new ArrayList<>();
        getColumns().forEach(column -> elements.add(new Element(values.get(column), column.getName())));

        Row row = new Row(elements);
        row.validate(this);

        rows.add(row);
    }

    public void update(Map<Column, String> values, Predicate<Row> predicate) throws Exception {
        for (Map.Entry<Column, String> entry : values.entrySet()) {
            new Element(entry.getValue(), entry.getKey().getName()).validate(this);
        }

        for (Row row : rows) {
            if (!predicate.test(row)) continue;

            for (Map.Entry<Column, String> entry : values.entrySet()) {
                row.getElement(entry.getKey()).setValue(entry.getValue());
            }
        }
    }

    public void delete(Predicate<Row> predicate) throws Exception {
        rows.removeIf(predicate);
    }

    public Collection<Row> select(Collection<Column> columns, Predicate<Row> predicate) throws Exception {
        if (columns.isEmpty()) throw new Exception("Columns collection is not allowed to be empty in a select query");

        ArrayList<Row> result = new ArrayList<>();

        for (Row row : rows) {
            if (!predicate.test(row)) continue;

            result.add(new Row(columns.stream().map(row::getElement).collect(Collectors.toCollection(ArrayList::new))));
        }

        return result;
    }

    public String getName() {
        return name;
    }

    public Collection<Row> getRows() {
        return rows;
    }

    public Column getColumn(String name) throws Exception {
        if (!columns.containsKey(name)) throw new Exception(String.format("A column with the name '%s' doesn't exist", name));
        return columns.get(name);
    }

    public Collection<Column> getColumns() {
        return columns.values();
    }

    public void deleteDuplicates() {
        rows = rows.stream().distinct().collect(Collectors.toList());
    }
}
