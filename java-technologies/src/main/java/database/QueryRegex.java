package database;

public enum QueryRegex {
    DELETE_ROWS("^\\s*DELETE\\s+FROM\\s+([^\\s]+)(?:\\s+WHERE\\s+([^\\s]+))?\\s*$"),
    INSERT_ROW("^\\s*INSERT\\s+INTO\\s+([^\\s]+)\\s+\\(([^\\)]+)\\)\\s+VALUES\\s*\\(([^\\)]+)\\)s*$"),
    UPDATE_ROWS("^\\s*UPDATE\\s+([^\\s]+)\\s+SET\\s*([^\\s]+)(?:\\s+WHERE\\s+([^\\s]+))?\\s*$"),
    SELECT_ROWS("^\\s*SELECT\\s+(.+)\\s+FROM\\s+([^\\s]+)(?:\\s+WHERE\\s+([^\\s]+))?\\s*$"),

    CREATE_TABLE("^\\s*CREATE\\s+TABLE\\s+([^\\s]+)\\s+\\(([^\\)]+)\\)\\s*$"),
    DROP_TABLE("^\\s*DROP\\s+TABLE\\s+([^\\s]+)\\s*$"),
    LIST_TABLES("^\\s*(LIST\\s+TABLES)\\s*$"),

    DELETE_DUPLICATES("^\\s*DELETE\\s+DUPLICATES\\s+([^\\s]+)\\s*$");

    private String regex;
    public final String SUFFIX = "/i";

    QueryRegex(String regex) {
        this.regex = regex;
    }

    public String getRegex() {
        return regex + SUFFIX;
    }
}
