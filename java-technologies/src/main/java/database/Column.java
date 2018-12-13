package database;

public class Column {
    public enum Type {
        INT,
        FLOAT,
        CHAR,
        STR,
        STR_RANGE,
        HTML,
        TIME,
        TIME_RANGE
    }

    private Type type;
    private String name;
    private boolean nullAllowed = true;
    private boolean unique = false;

    public Column(Type type, String name) {
        this.type = type;
        this.name = name;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public boolean isNullAllowed() {
        return nullAllowed;
    }

    public void setNullAllowed(boolean nullAllowed) {
        this.nullAllowed = nullAllowed;
    }

    public boolean isUnique() {
        return unique;
    }

    public void setUnique(boolean unique) {
        this.unique = unique;
    }
}
