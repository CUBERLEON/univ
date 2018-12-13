package com.liubomyr.databasedemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import com.liubomyr.database.Database;
import com.liubomyr.database.DatabaseReader;
import com.liubomyr.database.Element;
import com.liubomyr.database.Result;
import com.liubomyr.database.Row;

public class MainActivity extends AppCompatActivity {
    private Database database;
    private EditText editText;
    private TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        editText = findViewById(R.id.editText);
        textView = findViewById(R.id.textView);

        if (ContextCompat.checkSelfPermission(getApplicationContext(), Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE},
                    123);

        }

        String databasePath = Environment.getExternalStorageDirectory() + "/test.json";
        System.out.println(databasePath);
        try {
            database = new DatabaseReader(databasePath).read();
        } catch (Exception e) {
            database = new Database(databasePath);
            e.printStackTrace();
        }
    }

    protected void query(View view) {
        String text = editText.getText().toString();
        System.out.println(text);
        String response = processResult(database.query(text));
        textView.setText(response);
        System.out.println(response);
    }

    protected String processResult(Result result) {
        StringBuilder sb = new StringBuilder();
        if (result.getStatus() == Result.Status.FAIL) {
            sb.append(String.format("FAIL\n%s\n", result.getReport()));
            return sb.toString();
        }

        if (result.getRows() == null) {
            sb.append("OK\n");
            return sb.toString();
        }

        if (result.getRows().isEmpty()) {
            sb.append("Nothing was found\n");
            return sb.toString();
        }

        for (Element element : result.getRows().iterator().next().getElements()) {
            sb.append(String.format("%18s", element.getColumn()));
        }
        sb.append("\n\n");

        for (Row row : result.getRows()) {
            for (Element element : row.getElements()) {
                sb.append(String.format("%18s", element.getAsString()));
            }
            sb.append("\n");
        }

        return sb.toString();
    }
}
