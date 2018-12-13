package dbviewer;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import database.*;

public class MainController {
    @FXML
    private TextField queryText;
    @FXML
    private Button runButton;
    @FXML
    private TextArea resultText;

    private Database database;

    public MainController() {
        String databasePath = "test.json";
        try {
            database = new DatabaseReader(databasePath).read();
        } catch (Exception e) {
            database = new Database(databasePath);
            e.printStackTrace();
        }
    }

    public void run() {
        Result result = database.query(queryText.getText());

        if (result.getStatus() == Result.Status.FAIL) {
            resultText.setText("FAIL\n" + result.getReport());
            return;
        }

        if (result.getRows() == null) {
            resultText.setText("OK");
            return;
        }

        if (result.getRows().isEmpty()) {
            resultText.setText("Nothing was found");
            return;
        }

        StringBuilder lines = new StringBuilder();
        for (Element element : result.getRows().iterator().next().getElements()) {
            lines.append(element.getColumn()).append("\t\t");
        }
        lines.append("\n\n");

        for (Row row : result.getRows()) {
            for (Element element : row.getElements()) {
                lines.append(element.getAsString()).append("\t\t");
            }
            lines.append("\n");
        }

        resultText.setText(lines.toString());
    }
}
