package database;

import java.io.Serializable;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class Row implements Serializable {
    private Map<String, Element> elements;

    Row(Collection<Element> elements) {
        this.elements = new HashMap<>();
        for (Element element : elements) {
            this.elements.put(element.getColumn(), element);
        }
    }

    public Element getElement(String columnName) {
        return elements.get(columnName);
    }

    public Element getElement(Column column) {
        return getElement(column.getName());
    }

    public Collection<Element> getElements() {
        return elements.values();
    }

    public void validate(Table table) throws Exception {
        for (Element element : getElements()) {
            element.validate(table);
        }
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Row row = (Row) o;

        if (row.getElements().size() != this.getElements().size()) return false;

        for (Element element : this.getElements()) {
            if (!row.getElements().contains(element)) return false;
            if (!row.getElement(element.getColumn()).equals(element)) return false;
        }

        return true;
    }

    @Override
    public int hashCode() {
        return Objects.hash(elements);
    }
}