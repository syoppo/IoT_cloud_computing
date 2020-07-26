package com.example.led_remote_control;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;

public class MainActivity extends AppCompatActivity {
    Button onBtn;
    Button offBtn;

    FirebaseDatabase db = FirebaseDatabase.getInstance();
    DatabaseReference myRef = db.getReference("LED_STATUS");    //파이어베이스의 LED_STATUS를 가져와서 myRef에 넣어줌.

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        onBtn = (Button)findViewById(R.id.onBtn);
        offBtn = (Button)findViewById(R.id.offBtn);
    }

    @Override
    protected void onStart(){
        super.onStart();
        onBtn.setOnClickListener(new View.OnClickListener() {   //on버튼을 누르면
            @Override
            public void onClick(View v) {
                myRef.setValue("on");   //myRef의 값이 on이 됨
            }
        });
        offBtn.setOnClickListener(new View.OnClickListener() { //off버튼을 누르면
            @Override
            public void onClick(View v) {
                myRef.setValue("off");  //myRef의 값이 off가 됨
            }
        });
    }
}
